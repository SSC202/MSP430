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
//******************************************************************************
//!
//!   ADC12_A - Sample A0 Input, AVcc Ref, LED ON if A0 > 0.5*AVcc
//!   MSP430F552x Demo
//!
//!   Description: A single sample is made on A0 with reference to AVcc.
//!   Software sets ADC12SC to start sample and conversion - ADC12SC
//!   automatically cleared at EOC. ADC12_A internal oscillator times sample (16x)
//!   and conversion. In Mainloop MSP430 waits in LPM0 to save power until ADC12_A
//!   conversion complete, ADC12_A_ISR will force exit from LPM0 in Mainloop on
//!   reti. If A0 > 0.5*AVcc, P1.0 set, else reset.
//!
//!                 MSP430F552x
//!             -----------------
//!         /|\|                 |
//!          | |                 |
//!          --|RST       P6.0/A0|<- Vin
//!            |             P1.0|--> LED
//!            |                 |
//!
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - ADC12_A peripheral
//! - GPIO Port peripheral
//! - A0
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you must add these interrupt handlers to your
//! vector table.
//! - ADC12_A_VECTOR
//!
//******************************************************************************

#include "driverlib.h"

void main (void)
{
    //Stop Watchdog Timer
    WDT_A_hold(WDT_A_BASE);

    //P6.0 ADC option select
    GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P6,
            GPIO_PIN0
            );

    GPIO_setAsOutputPin(
            GPIO_PORT_P1,
            GPIO_PIN0
            );

    //Initialize the ADC12_A Module
    /*
     * Base address of ADC12_A Module
     * Use internal ADC12_A bit as sample/hold signal to start conversion
     * USE MODOSC 5MHZ Digital Oscillator as clock source
     * Use default clock divider of 1
     */
    ADC12_A_init(ADC12_A_BASE,
            ADC12_A_SAMPLEHOLDSOURCE_SC,
            ADC12_A_CLOCKSOURCE_ADC12OSC,
            ADC12_A_CLOCKDIVIDER_1);

    ADC12_A_enable(ADC12_A_BASE);

    /*
     * Base address of ADC12_A Module
     * For memory buffers 0-7 sample/hold for 64 clock cycles
     * For memory buffers 8-15 sample/hold for 4 clock cycles (default)
     * Disable Multiple Sampling
     */
    ADC12_A_setupSamplingTimer(ADC12_A_BASE,
            ADC12_A_CYCLEHOLD_64_CYCLES,
            ADC12_A_CYCLEHOLD_4_CYCLES,
            ADC12_A_MULTIPLESAMPLESDISABLE);

    //Configure Memory Buffer
    /*
     * Base address of the ADC12_A Module
     * Configure memory buffer 0
     * Map input A0 to memory buffer 0
     * Vref+ = AVcc
     * Vr- = AVss
     * Memory buffer 0 is not the end of a sequence
     */
    ADC12_A_configureMemoryParam param = {0};
    param.memoryBufferControlIndex = ADC12_A_MEMORY_0;
    param.inputSourceSelect = ADC12_A_INPUT_A0;
    param.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
    param.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    param.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
    ADC12_A_configureMemory(ADC12_A_BASE ,&param);

    //Enable memory buffer 0 interrupt
    ADC12_A_clearInterrupt(ADC12_A_BASE,
            ADC12IFG0);
    ADC12_A_enableInterrupt(ADC12_A_BASE,
            ADC12IE0);

    while (1)
    {
        //Enable/Start sampling and conversion
        /*
         * Base address of ADC12_A Module
         * Start the conversion into memory buffer 0
         * Use the single-channel, single-conversion mode
         */
        ADC12_A_startConversion(ADC12_A_BASE,
                ADC12_A_MEMORY_0,
                ADC12_A_SINGLECHANNEL);

        //LPM0, ADC12_A_ISR will force exit
        __bis_SR_register(LPM0_bits + GIE);
        //for Debugger
        __no_operation();
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(ADC12_VECTOR)))
#endif
void ADC12_A_ISR (void)
{
    switch (__even_in_range(ADC12IV,34)){
        case  0: break;   //Vector  0:  No interrupt
        case  2: break;   //Vector  2:  ADC overflow
        case  4: break;   //Vector  4:  ADC timing overflow
        case  6:          //Vector  6:  ADC12IFG0
                 //Is Memory Buffer 0 = A0 > 0.5AVcc?
                 if (ADC12_A_getResults(ADC12_A_BASE,
                             ADC12_A_MEMORY_0)
                         >= 0x7ff){
                     //set P1.0
                     GPIO_setOutputHighOnPin(
                             GPIO_PORT_P1,
                             GPIO_PIN0
                             );
                 } else   {
                     //Clear P1.0 LED off
                     GPIO_setOutputLowOnPin(
                             GPIO_PORT_P1,
                             GPIO_PIN0
                             );
                 }

                 //Exit active CPU
                 __bic_SR_register_on_exit(LPM0_bits);
        case  8: break;   //Vector  8:  ADC12IFG1
        case 10: break;   //Vector 10:  ADC12IFG2
        case 12: break;   //Vector 12:  ADC12IFG3
        case 14: break;   //Vector 14:  ADC12IFG4
        case 16: break;   //Vector 16:  ADC12IFG5
        case 18: break;   //Vector 18:  ADC12IFG6
        case 20: break;   //Vector 20:  ADC12IFG7
        case 22: break;   //Vector 22:  ADC12IFG8
        case 24: break;   //Vector 24:  ADC12IFG9
        case 26: break;   //Vector 26:  ADC12IFG10
        case 28: break;   //Vector 28:  ADC12IFG11
        case 30: break;   //Vector 30:  ADC12IFG12
        case 32: break;   //Vector 32:  ADC12IFG13
        case 34: break;   //Vector 34:  ADC12IFG14
        default: break;
    }
}

