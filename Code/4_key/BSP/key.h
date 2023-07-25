#include "msp430.h"

void Key_Init(void);
__interrupt void PORT1_Key();
__interrupt void PORT2_Key();