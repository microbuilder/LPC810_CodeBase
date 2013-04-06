/**************************************************************************/
/*!
    @file     main.c

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2013, K. Townsend (microBuilder.eu)
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
#include <stdio.h>
#include "LPC8xx.h"
#include "gpio.h"
#include "mrt.h"

#if defined(__CODE_RED)
  #include <cr_section_macros.h>
  #include <NXP/crp.h>
  __CRP const unsigned int CRP_WORD = CRP_NO_CRP ;
#endif

#define LED_LOCATION    (3)   /* LED is on pin 0.3 */

void configurePins()
{
  /* Enable SWM clock */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);

  /* Set everything to GPIO in the switch matrix           */
  /* This will also turn off the SWD pins and the external */
  /* crystal inputs (XTAL) since we need these pins for    */
  /* general GPIO use.                                     */
  LPC_SWM->PINENABLE0 = 0xffffffffUL;

  /* Pin I/O Configuration (internal resistors, hysteresis, etc.) */
  /* Default = pull-up resistor enabled (0x90) */
  /* LPC_IOCON->PIO0_0 = 0x90; */
  /* LPC_IOCON->PIO0_1 = 0x90; */
  /* LPC_IOCON->PIO0_2 = 0x90; */
  /* LPC_IOCON->PIO0_3 = 0x90; */
  /* LPC_IOCON->PIO0_4 = 0x90; */
  /* LPC_IOCON->PIO0_5 = 0x90; */
}

int main(void)
{
  SystemCoreClockUpdate();        /* Configure the core clock/PLL */
  gpioInit();                     /* Initialise the GPIO block */
  mrtInit(SystemCoreClock/1000);  /* Configure the multi-rate timer for 1ms ticks */
  configurePins();                /* Configure the switch matrix */

  /* Set the LED pin to output (1 = output, 0 = input) */
  LPC_GPIO_PORT->DIR0 |= (1 << LED_LOCATION);

  while(1)
  {
    /* Turn LED Off by setting the GPIO pin high */
    LPC_GPIO_PORT->SET0 = 1 << LED_LOCATION;
    mrtDelay(500);
    /* Turn LED On by setting the GPIO pin low */
    LPC_GPIO_PORT->CLR0 = 1 << LED_LOCATION;
    mrtDelay(500);
  }
}
