#Include <18F4550.h>
#Device PASS_STRINGS = IN_RAM
#fuses INTRC, NOPROTECT, NOWDT, CPUDIV1, PLL4
#use I2C(MASTER, SDA = PIN_B0, SCL = PIN_B1, FAST = 400000, STREAM = SSD1306_STREAM)
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

void initialMessage();
void rpm_and_time_display();

void limpiar_palabra();
void imp();


Void Main()
{
   fprintf(TTL, "READY TO GO\r\n");
   limpiar_palabra();
   SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);   //initializing the oled
   SSD1306_ClearDisplay();    //clear oled screen
   SSD1306_Display();
   initialMessage();  //initial screen
   delay_ms(600);
   rpm_and_time_display();
   while(True)
   {
      for(i=0; i<5; i++)
      {
         number = fgetc(BTH);
         delay_us(10);
         output_high(pin_a1);
         if(number == '.')
            break;
         rpm[i] = number;
      
      }
      imp();
      rpm_and_time_display(); //display of rpm speed and time
      delay_ms(150);
      limpiar_palabra();
      output_low(pin_a1);
   
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
