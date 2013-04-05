/**************************************************************************/
/*!
    @file     gpio.c
    @author   K. Townsend
*/
/**************************************************************************/
#include <string.h>

#include "gpio.h"

void gpioInit(void)
{
  /* Enable AHB clock to the GPIO domain. */
  LPC_SYSCON->SYSAHBCLKCTRL |=  (1 << 6);
  LPC_SYSCON->PRESETCTRL    &= ~(1 << 10);
  LPC_SYSCON->PRESETCTRL    |=  (1 << 10);
}

uint32_t gpioGetPinValue(uint32_t port, uint32_t pin)
{
  uint32_t result = 0;

  if(pin < 0x20)
  {
    if (LPC_GPIO_PORT->PIN0 & (0x1 << pin))
    {
      result = 1;
    }
  }
  else if(pin == 0xFF)
  {
    result = LPC_GPIO_PORT->PIN0;
  }
  return result;
}

void gpioSetValue(uint32_t port, uint32_t pin, uint32_t value)
{
  if (value)
  {
    LPC_GPIO_PORT->SET0 = 1 << pin;
  }
  else
  {
    LPC_GPIO_PORT->CLR0 = 1 << pin;
  }
  return;
}

void gpioSetDir(uint32_t port, uint32_t pin, uint32_t dir)
{
  if(dir)
  {
    LPC_GPIO_PORT->DIR0 |= (1 << pin);
  }
  else
  {
    LPC_GPIO_PORT->DIR0 &= ~(1 << pin);
  }
  return;
}
