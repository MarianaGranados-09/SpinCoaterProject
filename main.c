#Include <18F4550.h>
#Device PASS_STRINGS = IN_RAM
#fuses INTRC, NOPROTECT, NOWDT, CPUDIV1, PLL4
#use I2C(MASTER, SDA = PIN_B0, SCL = PIN_B1, FAST = 400000, STREAM = SSD1306_STREAM)
#use delay(Clock=8M)

#define SSD1306_I2C_ADDRESS 0x78    //memory direction of oled display
#include <SSD1306_OLED.c>           //library to manage the oled
#include <string.h>
#include <stdio.h>

void initialMessage();
void rpm_and_time_display();
void convert_float_to_string();
int rpm = 205;
char str[20];

Void Main()
{
   SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);   //initializing the oled
   SSD1306_ClearDisplay();    //clear oled screen
   SSD1306_Display();
   initialMessage();  //initial screen
   rpm_and_time_display(); //display of rpm speed and time
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
   SSD1306_DrawText(14,1,"RPM: ", 1);
   SSD1306_DrawText(35,22,str,1);
   SSD1306_Display();
}

void convert_float_to_string()
{
   sprintf(str, "%d", rpm); //make int into string  
}
