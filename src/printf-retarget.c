/**************************************************************************/
/*!
    @file     printf-retarget.c
    @author   K. Townsend
*/
/**************************************************************************/
#include <stdarg.h>

void putchar(const char c)
{
  // uartSendByte(c);
  __asm volatile("nop");
}

int puts(const char * str)
{
  while(*str) putchar(*str++);

  return 0;
}
