#include <18F4550.h>
#fuses INTRC, NOPROTECT, NOWDT, NOLVP, CPUDIV1, PLL1        
#use delay (clock = 8M)

#use rs232(rcv=pin_C7, xmit=pin_C6, baud=9600, bits=8, parity=n, stream = BTH)
#use rs232(rcv=pin_B5, xmit=pin_B4, baud=9600, bits=8, parity=n, stream = TTL)

#BYTE TRISD = 0xF95
#BYTE PORTD = 0xF83

int8 i=0, j = 0,k=0;
char number;
char rpm[8];

void limpiar_palabra();

void main()
{
   fprintf(TTL, "READY TO GO\r\n");
   //letter = fgetc(BTH);
   limpiar_palabra();
   while(True)
   {
      
      for(i=0;i<8;i++)
      {
         number = fgetc(BTH);
         delay_us(10);
         output_high(pin_a1);
         if(number == '.')
            goto Imprimir;
         rpm[i] = number;
      }
      
      Imprimir:
      delay_us(10);
      fprintf(TTL, "Los RPM son: \r\n");
      for(j=0;j<8;j++)
      {
         fprintf(TTL,"%c", rpm[j]);
      }
      
    delay_ms(1500);
    output_low(pin_a1);
    limpiar_palabra();
   }
}

void limpiar_palabra()
{
   for(k=0;k<8;k++)
      {
         rpm[k] = 0;
      }
}
