/**************************************************************************/
/*!
    @file     gpio.c
    @author   K. Townsend
    
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
