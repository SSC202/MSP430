
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
  
  P1DIR |= BIT0;        //����1.0Ϊ���
  P4DIR |= BIT7;
  P1OUT |= BIT0;        //����1.0Ϊ�ߵ�ƽ
  P4OUT |= BIT7;
  delay_ms(1000);
  P1OUT ^= BIT0;        //����1.0��ת
  delay_ms(1000);
  
  while(1)
  {
    P4OUT ^= BIT7;
    delay_ms(1000);
  }
}
