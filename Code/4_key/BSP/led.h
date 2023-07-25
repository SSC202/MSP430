#include "msp430.h"

#define         LED1(x)                  do{(x == 1)?(P1OUT |= BIT0):(P1OUT &= ~(BIT0));}while(0) 
#define         LED2(x)                  do{(x == 1)?(P4OUT |= BIT7):(P4OUT &= ~(BIT7));}while(0)

#define         LED1_Toggle              do{P1OUT ^= BIT0;}while(0)
#define         LED2_Toggle              do{P4OUT ^= BIT7;}while(0)

void LED_Init(void);
