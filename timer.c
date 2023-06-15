#include <18F4550.h>
#fuses HS, NOWDT, NOPROTECT, NOLVP
#use delay(clock=8M)
#use standard_io(B)

#use rs232(rcv=pin_C7, xmit=pin_C6, baud=9600, bits=8, parity=n)

#BYTE PORTD=0xF83
#BYTE TRISD = 0xF95

int16 const display[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
int1 count = 0;
int value = 0;



#int_TIMER1
void TIMER1_isr(void)
{
   
   if(count == 1)
   {
      output_toggle(pin_b1);
      value++;
      if(value < 10 && value >= 0)
      {
         PORTD = display[value];
      }
   }
   set_timer1(3036); //recharge timer1
   count++;
}

void main(void)
{  
   
   TRISD = 0x00;
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_4);
   set_timer1(3036);
   enable_interrupts(INT_TIMER1);
   enable_interrupts(global);
   PORTD = display[value];
   while(true);

}
