
#include "io430.h"

void delay_ms(int ms)
{
  for(;ms > 0;ms--)
    __delay_cycles(1000);
}

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  P1DIR |= BIT0;        //设置1.0为输出
  P4DIR |= BIT7;
  P1OUT |= BIT0;        //设置1.0为高电平
  P4OUT |= BIT7;
  delay_ms(1000);
  P1OUT ^= BIT0;        //设置1.0翻转
  delay_ms(1000);
  
  while(1)
  {
    P4OUT ^= BIT7;
    delay_ms(1000);
  }
}
