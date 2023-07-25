#include "key.h"
#include "led.h"

/**
  * @brief      按键初始化函数
  */
void Key_Init(void)
{
  //P2.1       Key1
  P2DIR &= ~(BIT1);
  P2REN |= BIT1;
  P2OUT |= BIT1;
  P2IE |= BIT1;
  P2IES |= BIT1;
  P2IFG = 0x00;
  
  //P1.1       Key2
  P1DIR &= ~(BIT1);
  P1REN |= BIT1;
  P1OUT |= BIT1;
  P1IE |= BIT1;
  P1IES |= BIT1;
  P1IFG = 0x00;
  
  _EINT();
  
}

#pragma vector = PORT1_VECTOR   //设置中断向量
__interrupt void PORT1_Key()
{
  int Push_Key;
  _DINT();
  Push_Key = ~P1IN & P1IFG & ~P1DIR;
  if(Push_Key & BIT1)
  {  
      LED1_Toggle;
  }
  P1IFG = 0x00;
  _EINT();
}

#pragma vector = PORT2_VECTOR   //设置中断向量
__interrupt void PORT2_Key()
{     
  int Push_Key;
  _DINT();
  Push_Key = ~P2IN & P2IFG & ~P2DIR;
  if(Push_Key & BIT1)
  {  
      LED2_Toggle;
  }
  P2IFG = 0x00;
  _EINT();
}