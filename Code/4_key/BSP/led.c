#include "led.h"

/**
  * @brief      LED初始化
  */
void LED_Init(void)
{
  //LED1 初始化
  P1DIR |= BIT0;
  P1SEL &= ~(BIT0);
  P1OUT &= ~(BIT0);
  
  //LED2 初始化
  P4DIR |= BIT7;
  P4SEL &= ~(BIT7);
  P4OUT &= ~(BIT7);
}


