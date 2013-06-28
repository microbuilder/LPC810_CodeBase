/******************************************************************************
 * @file:    system_LPC8xx.c
 * @purpose: CMSIS Cortex-M0+ Device Peripheral Access Layer Source File
 *           for the NXP LPC8xx Device Series 
 * @version: V1.0
 * @date:    16. Aug. 2012
 *----------------------------------------------------------------------------
 *
 * Copyright (C) 2012 ARM Limited. All rights reserved.
 *
 * ARM Limited (ARM) is supplying this software for use with Cortex-M0+ 
 * processor based microcontrollers.  This file can be freely distributed 
 * within development tools that are supporting such ARM based processors. 
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/
#include <stdint.h>
#include "LPC8xx.h"

#include "system_LPC8xx.h"

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = __SYSTEM_CLOCK;/*!< System Clock Frequency (Core Clock)*/

/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
  uint32_t wdt_osc = 0;

  /* Determine clock frequency according to clock register values             */
  switch ((LPC_SYSCON->WDTOSCCTRL >> 5) & 0x0F) {
    case 0:  wdt_osc =       0; break;
    case 1:  wdt_osc =  500000; break;
    case 2:  wdt_osc =  800000; break;
    case 3:  wdt_osc = 1100000; break;
    case 4:  wdt_osc = 1400000; break;
    case 5:  wdt_osc = 1600000; break;
    case 6:  wdt_osc = 1800000; break;
    case 7:  wdt_osc = 2000000; break;
    case 8:  wdt_osc = 2200000; break;
    case 9:  wdt_osc = 2400000; break;
    case 10: wdt_osc = 2600000; break;
    case 11: wdt_osc = 2700000; break;
    case 12: wdt_osc = 2900000; break;
    case 13: wdt_osc = 3100000; break;
    case 14: wdt_osc = 3200000; break;
    case 15: wdt_osc = 3400000; break;
  }
  wdt_osc /= ((LPC_SYSCON->WDTOSCCTRL & 0x1F) << 1) + 2;
 
  switch (LPC_SYSCON->MAINCLKSEL & 0x03) {
    case 0:                             /* Internal RC oscillator             */
      SystemCoreClock = __IRC_OSC_CLK;
      break;
    case 1:                             /* Input Clock to System PLL          */
      switch (LPC_SYSCON->SYSPLLCLKSEL & 0x03) {
          case 0:                       /* Internal RC oscillator             */
            SystemCoreClock = __IRC_OSC_CLK;
            break;
          case 1:                       /* System oscillator                  */
            SystemCoreClock = __SYS_OSC_CLK;
            break;
          case 2:                       /* Reserved                           */
            SystemCoreClock = 0;
            break;
          case 3:                       /* CLKIN pin                          */
            SystemCoreClock = __CLKIN_CLK;
            break;
      }
      break;
    case 2:                             /* WDT Oscillator                     */
      SystemCoreClock = wdt_osc;
      break;
    case 3:                             /* System PLL Clock Out               */
      switch (LPC_SYSCON->SYSPLLCLKSEL & 0x03) {
          case 0:                       /* Internal RC oscillator             */
            SystemCoreClock = __IRC_OSC_CLK * ((LPC_SYSCON->SYSPLLCTRL & 0x01F) + 1);
            break;
          case 1:                       /* System oscillator                  */
            SystemCoreClock = __SYS_OSC_CLK * ((LPC_SYSCON->SYSPLLCTRL & 0x01F) + 1);
            break;
          case 2:                       /* Reserved                           */
            SystemCoreClock = 0;
            break;
          case 3:                       /* CLKIN pin                          */
            SystemCoreClock = __CLKIN_CLK * ((LPC_SYSCON->SYSPLLCTRL & 0x01F) + 1);
            break;
      }
      break;
  }

  SystemCoreClock /= LPC_SYSCON->SYSAHBCLKDIV;  

}


	// cpldcpu - added 2013/06/23
	// Delays by 3*ticks cycles
	__attribute__((always_inline)) void __delayticks(unsigned int ticks) {
	  	asm volatile(
	  			"loop%=: sub %[ctr],#1	\n"
	  			"		 bne loop%=\n"
	  			: [ctr] "+r" (ticks)
	  			);
	}


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void) {
  /* System clock to the IOCON & the SWM need to be enabled or
  most of the I/O related peripherals won't work. */
  LPC_SYSCON->SYSAHBCLKCTRL |= ( (0x1 << 7) | (0x1 << 18) );
	
#if (CLOCK_SETUP)                                 /* Clock Setup              */

#if ((SYSPLLCLKSEL_Val & 0x03) == 1)
  LPC_IOCON->PIO0_8 &= ~(0x3 << 3);
  LPC_IOCON->PIO0_9 &= ~(0x3 << 3);
  LPC_SWM->PINENABLE0 &= ~(0x3 << 4);
  LPC_SYSCON->PDRUNCFG     &= ~(0x1 << 5);        /* Power-up System Osc      */
  LPC_SYSCON->SYSOSCCTRL    = SYSOSCCTRL_Val;
  __delayticks(1024); 			// cpldcpu 2013/06/23 replaced inefficient delay loop
#endif
#if ((SYSPLLCLKSEL_Val & 0x03) == 3)
  LPC_IOCON->PIO0_1 &= ~(0x3 << 3);
  LPC_SWM->PINENABLE0 &= ~(0x1 << 7);
  __delayticks(1024); 			// cpldcpu 2013/06/23 replaced inefficient delay loop
#endif

  LPC_SYSCON->SYSPLLCLKSEL  = SYSPLLCLKSEL_Val;   /* Select PLL Input         */
  LPC_SYSCON->SYSPLLCLKUEN  = 0x01;								/* Update Clock Source      */
  while (!(LPC_SYSCON->SYSPLLCLKUEN & 0x01));     /* Wait Until Updated       */
#if ((MAINCLKSEL_Val & 0x03) == 3)                /* Main Clock is PLL Out    */
  LPC_SYSCON->SYSPLLCTRL    = SYSPLLCTRL_Val;
  LPC_SYSCON->PDRUNCFG     &= ~(0x1 << 7);        /* Power-up SYSPLL          */
  while (!(LPC_SYSCON->SYSPLLSTAT & 0x01));	      /* Wait Until PLL Locked    */
#endif

#if (((MAINCLKSEL_Val & 0x03) == 2) )
  LPC_SYSCON->WDTOSCCTRL    = WDTOSCCTRL_Val;
  LPC_SYSCON->PDRUNCFG     &= ~(0x1 << 6);        /* Power-up WDT Clock       */
  __delayticks(1024); 			// cpldcpu 2013/06/23 replaced inefficient delay loop
#endif

  LPC_SYSCON->MAINCLKSEL    = MAINCLKSEL_Val;     /* Select PLL Clock Output  */
  LPC_SYSCON->MAINCLKUEN    = 0x01;								/* Update MCLK Clock Source */
  while (!(LPC_SYSCON->MAINCLKUEN & 0x01));       /* Wait Until Updated       */

  LPC_SYSCON->SYSAHBCLKDIV  = SYSAHBCLKDIV_Val;

#if __SYSTEM_CLOCK <= 30000000
  LPC_FLASHCTRL->FLASHCFG=0;		/* Set flash waitstates to zero if core clock is <=20 Mhz. Default is 1 waitstate */
#endif

#endif
}
