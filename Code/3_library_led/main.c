
#include "driverlib.h"

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDT_A_hold(WDT_A_BASE);
  
  GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
  GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
  
  while(1)
  {
    ;
  }
}
