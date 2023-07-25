#include "msp430.h"
#include "led.h"
#include "key.h"

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  LED_Init();
  Key_Init();

  while(1)
  {
    ;
  }
}
