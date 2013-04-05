/**************************************************************************/
/*!
    @file     main.c
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
