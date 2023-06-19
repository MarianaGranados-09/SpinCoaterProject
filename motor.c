#Include <18f4550.h>
//#USE DELAY(CLOCK=4000000) //Crystal Interno 4MHz
#use delay(clock=8M)//Crystal Externo 4MHz
#FUSES INTRC,NOPROTECT,NOWDT,PUT,NOLVP

long int valor=400;

void main()
{
   enable_interrupts(global);
   enable_interrupts(int_timer1);
   setup_ccp1(CCP_PWM);
   setup_timer_2(T2_DIV_BY_1, 255,1);
  //400 rpm - valor = 0
  //4000 rpm - valor = 1023
      while(true)
      {
         //valor = 400;
         delay_us(10);
         set_pwm1_duty(valor);
         valor++;
         //delay_ms(2000);
         if(valor == 1023)
            valor=0;
         //valor=0;
      }
}


         
