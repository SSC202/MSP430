
#include "io430.h"

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  volatile unsigned i;
  P1DIR |= BIT0;
  while(1)
  {
    P1OUT ^= BIT0;
    for(i = 50000;i > 0;i++);
  }
  //return 0;
}
