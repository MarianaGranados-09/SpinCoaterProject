#Include <18F4550.h>
#Device PASS_STRINGS = IN_RAM
#fuses INTRC, NOPROTECT, NOWDT, CPUDIV1, PLL4
#use I2C(MASTER, SDA = PIN_B3, SCL = PIN_B2, FAST = 400000, STREAM = SSD1306_STREAM)
#use delay(Clock=8M)

#use rs232(rcv=pin_C7, xmit=pin_C6, baud=9600, bits=8, parity=n, stream = BTH)
#use rs232(rcv=pin_B5, xmit=pin_B4, baud=9600, bits=8, parity=n, stream = TTL)

#BYTE TRISD = 0xF95
#BYTE PORTD = 0xF83

#define SSD1306_I2C_ADDRESS 0x78    //memory direction of oled display
#include <SSD1306_OLED.c>           //library to manage the oled
#include <string.h>
#include <stdio.h>

int8 i=0, j=0, k=0;
char number;
char rpm[8];
char mrpm = '0';
char crpm = '0';
char drpm = '0';
char urpm = '0';
int8 flagmotor = 0;
int8 comfin = 0;
int16 seg = 0;

long int valor = 400;

char timeO[3];

void initialMessage();
void rpm_display();

void limpiar_palabra();
void imp();

void minCalc();
void timed(); 

#int_ext
void isr_rb0()
{
  flagmotor = 1; 
  output_high(pin_a3);
  valor = 1000;
}

#int_ext1
void isr_rb1()
{
   flagmotor = 0;
   comfin = 0;
   seg = 0;
   valor=0;
   output_low(pin_a3);
}

void temps()
{
   int cont = 0;
   output_toggle(pin_a1);
   
   while (cont < 2)
   {
      if(flagmotor == 1) //start motor, start count, set seg to sum
         seg++;
      if(flagmotor == 0) //stop motor, stop count, set seg to 0
         seg = 0;
         
      set_timer1(3036);
      while(get_timer1() >= 3036);
      cont++;
   }

}


void main()
{
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
   enable_interrupts(GLOBAL);
   enable_interrupts(int_ext);
   enable_interrupts(int_ext1);
   enable_interrupts(int_timer1);
   setup_ccp1(CCP_PWM);
   setup_timer_2(T2_DIV_BY_1,255,1);
   
   timeO[0] = '0';
   timeO[1] = '0';
   fprintf(TTL, "READY TO GO\r\n");
   limpiar_palabra();
   SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);   //initializing the oled
   SSD1306_ClearDisplay();    //clear oled screen
   SSD1306_Display();
   initialMessage();  //initial screen
   delay_ms(600);
   rpm_display();
   timed();
   while(True)
   {
      comfin =0;
      while(flagmotor != 1)
      {
          for(i=0; i<5; i++)
           {
               number = fgetc(BTH);
               delay_us(10);
               output_high(pin_a2);
               if(number == '.')
                  break;
               rpm[i] = number;
            }
         imp();
         rpm_display(); //display of rpm speed and time
         limpiar_palabra();
         output_low(pin_a2);
         comfin = 1; //condicion para iniciar el motor, si comfin == 1 y flagmotor ==1
         //encender motor, aun no implementada
      }
      if(flagmotor == 1 && comfin ==1)
      {
         temps();
         minCalc();
         timed();
         fprintf(TTL, "%ld\n", seg/2);
         delay_us(10);
          set_pwm1_duty(valor);
      }
      set_pwm1_duty(valor);
   }
}

void initialMessage()
{
   SSD1306_DrawText(14,1,"TECHNOLOGY  OF", 1);
   SSD1306_DrawText(35,22,"MATERIALS", 1);
   SSD1306_DrawText(0,50,"BY: MM", 1);
   SSD1306_Display();
   Delay_ms(3000);
   SSD1306_ClearDisplay();
   SSD1306_Display();
}

void rpm_display()
{
   
   SSD1306_DrawText(10,1,"RPM: ", 1);
   
   SSD1306_DrawChar(35,1, mrpm ,1);
   SSD1306_DrawChar(45,1, crpm ,1);
   SSD1306_DrawChar(50,1, drpm ,1);
   SSD1306_DrawChar(55,1, urpm ,1);
   
   SSD1306_Display();
}

void timed()
{
SSD1306_DrawText(10,20,"TIME: ", 1);
   SSD1306_DrawChar(45,20, timeO[1] ,1);
   SSD1306_DrawChar(55,20, timeO[0] ,1);
   SSD1306_DrawText(65,20,"min", 1);
   SSD1306_Display();
}

void limpiar_palabra()
{
   for(k=0;k<8;k++)
      {
         rpm[k] = 0;
      }
      
      mrpm = ' ';
      crpm = ' ';
      drpm = ' ';
      urpm = ' ';
}

void imp()
{
   Imprimir:
      if(i == 4)
      {
         mrpm = rpm[0];
         crpm = rpm[1];
         drpm = rpm[2];
         urpm = rpm[3]; 
      }
      else if(i == 3)
      {
         crpm = rpm[0];
         drpm = rpm[1];
         urpm = rpm[2];
      }
      else if(i == 2)
      {
         drpm = rpm[0];
         urpm = rpm[1];
      }
      else
      {
         urpm = rpm[0];
      }
}

void minCalc(){
   
      if(seg/2 < 59)
      {
         timeO[0] = '0';
         timeO[1] = '0';
      }
      if(seg/2 >59 && seg/2 <=119)
      {
         timeO[0] = '1';
         timeO[1] = '0';
      }
      if(seg/2 >119 && seg/2 <=179) //2 min
      {
         timeO[0] = '2';
         timeO[1] = '0';
      }
      if(seg/2 >179 && seg/2 <=239) //3 min
      {
         timeO[0] = '3';
         timeO[1] = '0';
      }
      if(seg/2 >239 && seg/2 <= 299) //4 min
      {
         timeO[0] = '4';
         timeO[1] = '0';
      }
      if(seg/2 >299 && seg/2 <=359) //5 min
      {
         timeO[0] = '5';
         timeO[1] = '0';
      }
      if(seg/2 >359 && seg/2 <=419) //6 min
      {
         timeO[0] = '6';
         timeO[1] = '0';
      }
      if(seg/2 >419 && seg/2 <=479) //7 min
      {
         timeO[0] = '7';
         timeO[1] = '0';
      }
      if(seg/2 >479 && seg/2 <=539) //8 min
      {
         timeO[0] = '8';
         timeO[1] = '0';
      }
      if(seg/2 >539 && seg/2 <=599) //9 min
      {
         timeO[0] = '9';
         timeO[1] = '0';
      }
      if(seg/2 >599 && seg/2 <=659) //10 min
      {
         timeO[0] = '0';
         timeO[1] = '1';
      }
      if(seg/2 >659 && seg/2 <=719) //10 min
      {
         timeO[0] = '1';
         timeO[1] = '1';
      }
      if(seg/2 >719 && seg/2 <=779) //10 min
      {
         timeO[0] = '2';
         timeO[1] = '1';
      }
}
