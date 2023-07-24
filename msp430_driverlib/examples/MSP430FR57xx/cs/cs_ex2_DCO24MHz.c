/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
#include "driverlib.h"

//******************************************************************************
//!   MSP430FR57xx Demo - Configure MCLK for 24MHz operation
//!
//!   Description: Configure ACLK = SMCLK = MCLK = 24MHz
//!   IMPORTANT NOTE: While the FR5739 is capable of operation using a 24MHz
//!   MCLK; the throughput of the device is dependednt on accesses to FRAM.
//!   The maximum speed for accessing FRAM is limited to 8MHz*.
//!   Wait states are inserted automatically when the FRAM is accessed at
//!   speeds higher than 8MHz to prevent timing violations.
//!   Refer to the FRAM Clock System chapter of the User's Guide.
//!   * Refer device D/s for typical numbers
//!
//!           MSP430FR57x
//!         ---------------
//!     /|\|               |
//!      | |               |
//!      --|RST            |
//!        |               |
//!        |               |
//!        |          P2.0 |---> ACLK = MCLk = 24MHz
//!
//!   Texas Instruments Inc.
//!   February 2012
//!   Built with IAR Embedded Workbench Version: 5.40 & Code Composer Studio V5.1
//******************************************************************************

	uint32_t clockValue;

void main(void)
{
  //Stop WDT
  WDT_A_hold(WDT_A_BASE);

  CS_setDCOFreq(CS_DCORSEL_1,CS_DCOFSEL_3);

  CS_initClockSignal(CS_ACLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
  CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);


  //Set P2.0 as Output Low (for setup as ACLK).
  /*

   * Select Port 2
   * Set Pin 0 to output Low.
   */
  GPIO_setOutputLowOnPin(
			GPIO_PORT_P2,
			GPIO_PIN0
	);

  //Set P2.0 as Ternary Module Function Output.
  /*

   * Select Port 2
   * Set Pin 0 to output Ternary Module Function, (ACLK).
   */
  GPIO_setAsPeripheralModuleFunctionOutputPin(
			GPIO_PORT_P2,
			GPIO_PIN0,
			GPIO_TERNARY_MODULE_FUNCTION
	);
    clockValue = CS_getSMCLK();
    clockValue = CS_getMCLK();
    clockValue = CS_getACLK();



  while(1);

}


