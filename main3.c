#include <18F4550.h>
#Device PASS_STRINGS = IN_RAM
#fuses INTRC, NOPROTECT, NOWDT, CPUDIV1, PLL4
#use I2C(MASTER, SDA = PIN_B2, SCL = PIN_B3, FAST = 400000, STREAM = SSD1306_STREAM)
#use delay(clock=8M)

#use rs232(rcv=pin_B5, xmit=pin_B4, baud=9600, bits=8, parity=n, stream = TTL)

#define SSD1306_I2C_ADDRESS 0x78    //memory direction of oled display
#include <SSD1306_OLED.c>  

int8 i=0, k=0;
char number;
char rpm[8];
char mrpm = ' ';
char crpm = ' ';
char drpm = ' ';
char urpm = ' ';

int seg = 0;
char timeO[3];
int8 flagmotor = 0;

void initialMessage();
void rpm_and_time_display();

void limpiar_palabra();
void flagOp();

void minCalc();
void temps();


#int_ext
void isr_rb0()
{
  flagmotor = 1; 
  output_high(pin_a3);
}

#int_ext1
void isr_rb1()
{
   flagmotor = 0;
   output_low(pin_a3);
}


void main(void)
{  
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
   enable_interrupts(GLOBAL);
   enable_interrupts(int_ext);
   enable_interrupts(int_ext1);
   fprintf(TTL, "READY TO GO\r\n");
   limpiar_palabra();
   SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);   //initializing the oled
   SSD1306_ClearDisplay();    //clear oled screen
   SSD1306_Display();
   
   initialMessage();  //initial screen
   delay_ms(600);
   rpm_and_time_display();
   
   while(true)
   {
      temps();
      fprintf(TTL, "%d\n", seg/2);
      minCalc();
      rpm_and_time_display();
      delay_ms(10);
      limpiar_palabra();
      //output_low(pin_a2);
  
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
      if(seg/2 >119 && seg/2 <=179)
      {
         timeO[0] = '2';
         timeO[1] = '0';
      }
      fprintf(TTL, "%c", timeO[1]);
      fprintf(TTL, "%c min\r\n", timeO[0]);
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

void initialMessage()
{
   SSD1306_DrawText(14,1,"TECHNOLOGY  OF", 1);
   SSD1306_DrawText(35,22,"MATERIALS", 1);
   SSD1306_DrawText(0,50,"BY: MM", 1);
   SSD1306_Display();
   Delay_ms(5000);
   SSD1306_ClearDisplay();
   SSD1306_Display();
}

void rpm_and_time_display()
{
 
   SSD1306_DrawText(10,1,"RPM: ", 1);
   SSD1306_DrawChar(35,1, mrpm ,1);
   SSD1306_DrawChar(45,1, crpm ,1);
   SSD1306_DrawChar(50,1, drpm ,1);
   SSD1306_DrawChar(55,1, urpm ,1);

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

