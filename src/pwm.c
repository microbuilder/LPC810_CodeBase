/**************************************************************************/
/*!
    @file     pwm.c

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2014, Mat Kattanek
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**************************************************************************/

/*
    @info
    setup SCT as 32bit timer.
    generate PWM depending on match0 (period) and match1 (duty cycle) register.
    using pio0_2 to flash led.
 */

#include <stdio.h>
#include "LPC8xx.h"
#include "gpio.h"
#include "mrt.h"
#include "uart.h"

extern void configurePins();       // main.c

#define ISP_BUTTON      (1)

#define MATCH_PERIOD (SystemCoreClock/2)   // 2hz
#define MATCH_DUTY   (SystemCoreClock/20)  // 10% duty cycle

void
sct_fsm_init (void)
{
  LPC_SCT->CONFIG = (LPC_SCT->CONFIG & ~0x00060001) | 0x00000001;   // UNIFIED  32bit

  //--- MATCH/CAPTURE registers
  LPC_SCT->REGMODE_L = 0x0000;           // L: 2x MATCH, 0x CAPTURE, 3 unused
  LPC_SCT->MATCH[0].U = MATCH_PERIOD;    // MATCH0
  LPC_SCT->MATCHREL[0].U = MATCH_PERIOD;
  LPC_SCT->MATCH[1].U = MATCH_DUTY;      // MATCH1
  LPC_SCT->MATCHREL[1].U = MATCH_DUTY;

  //--- OUTPUT registers
  LPC_SCT->OUT[0].SET = 0x00000001;      // Output_pin_0 set on event0
  LPC_SCT->OUT[0].CLR = 0x00000002;      //              clear on event1
  LPC_SCT->OUT[1].SET = 0;               // Unused outputs must not be affected by any event
  LPC_SCT->OUT[1].CLR = 0;
  LPC_SCT->OUT[2].SET = 0;
  LPC_SCT->OUT[2].CLR = 0;
  LPC_SCT->OUT[3].SET = 0;
  LPC_SCT->OUT[3].CLR = 0;

  //--- EVENT registers
  LPC_SCT->EVENT[0].CTRL = 0x00005000;    // L: --> state L_ENTRY
  LPC_SCT->EVENT[0].STATE = 0x00000001;
  LPC_SCT->EVENT[1].CTRL = 0x00005001;    // L: --> state L_ENTRY
  LPC_SCT->EVENT[1].STATE = 0x00000001;
  LPC_SCT->EVENT[2].STATE = 0;            // Unused events must not have any effect
  LPC_SCT->EVENT[3].STATE = 0;
  LPC_SCT->EVENT[4].STATE = 0;
  LPC_SCT->EVENT[5].STATE = 0;

  //--- STATE registers
  LPC_SCT->STATE_L = 0;
  LPC_SCT->STATE_H = 0;

  //--- CORE registers
  LPC_SCT->START_L = 0x0000;
  LPC_SCT->STOP_L =  0x0000;
  LPC_SCT->HALT_L =  0x0000;
  LPC_SCT->LIMIT_L = 0x0001;
  LPC_SCT->START_H = 0x0000;
  LPC_SCT->STOP_H =  0x0000;
  LPC_SCT->HALT_H =  0x0000;
  LPC_SCT->LIMIT_H = 0x0000;
  LPC_SCT->EVEN =    0x00000000;
}

#if 1

int
main(void)
{
  gpioInit();                            // Initialize the GPIO block

  uart0Init(115200);                     // Initialize uart for printf output

  mrtInit(__SYSTEM_CLOCK/1000);          // Configure the multi-rate timer for 1ms ticks

  configurePins();                       // Configure the switch matrix (setup pins UART and GPIO)

  LPC_SWM->PINASSIGN6 = 0x02FFFFFF;            // SCT OUTP_0 at P0.2 (led) on switch matrix
  LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 8);       // enable the SCT clock

  sct_fsm_init();                              // Initialize SCT
  LPC_SCT->CTRL_L &= ~(1<<2);                  // unhalt the SCT by clearing bit 2 in CTRL

  LPC_GPIO_PORT->DIR0 &= ~(1 << ISP_BUTTON);   // enable ISP button as input pin

  while(1) {

     mrtDelay(500);                                         //take a snooze

     if ( (LPC_GPIO_PORT->PIN0 & 0x02) == 0) {	            // pio0_1 ISP button pushed?
  	     if (LPC_SCT->MATCHREL[1].U < MATCH_PERIOD ) {
           LPC_SCT->MATCHREL[1].U += MATCH_DUTY;            // increase duty cycle
  	     }
  	     else {
           LPC_SCT->MATCHREL[1].U = 0;                      // rollover
  	     }
     }

    // Send some text (redirected to UART)
    printf("Hello, lpc810.pwm! dutycycle=%u \r\n", LPC_SCT->MATCHREL[1].U );
  }
}
#endif
