/**************************************************************************/
/*!
    @file     gpio.h
    @author   K. Townsend
*/
/**************************************************************************/
#ifndef _GPIO_H_
#define _GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LPC8xx.h"
#include "errors.h"
#include "asserts.h"

void     gpioInit(void);
uint32_t gpioGetPinValue (uint32_t port, uint32_t pin);
void     gpioSetValue (uint32_t port, uint32_t pin, uint32_t value);
void     gpioSetDir (uint32_t port, uint32_t pin, uint32_t dir);

#ifdef __cplusplus
}
#endif

#endif
