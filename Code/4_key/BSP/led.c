#include "led.h"

/**
  * @brief      LED��ʼ��
  */
void LED_Init(void)
{
  //LED1 ��ʼ��
  P1DIR |= BIT0;
  P1SEL &= ~(BIT0);
  P1OUT &= ~(BIT0);
  
  //LED2 ��ʼ��
  P4DIR |= BIT7;
  P4SEL &= ~(BIT7);
  P4OUT &= ~(BIT7);
}


