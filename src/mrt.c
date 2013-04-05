/**************************************************************************/
/*!
    @file     mrt.c
    @author   K. Townsend
    @brief    Multi-rate timer (mrt) helper functions
*/
/**************************************************************************/
#include "LPC8xx.h"
#include "mrt.h"

volatile uint32_t mrt_counter = 0;

void MRT_IRQHandler(void)
{
  if ( LPC_MRT->Channel[0].STAT & MRT_STAT_IRQ_FLAG )
  {
    LPC_MRT->Channel[0].STAT = MRT_STAT_IRQ_FLAG;      /* clear interrupt flag */
    mrt_counter++;
  }
  return;
}

void mrtInit(uint32_t delay)
{
  /* Enable clock to MRT and reset the MRT peripheral */
  LPC_SYSCON->SYSAHBCLKCTRL |= (0x1<<10);
  LPC_SYSCON->PRESETCTRL &= ~(0x1<<7);
  LPC_SYSCON->PRESETCTRL |= (0x1<<7);

  mrt_counter = 0;
  LPC_MRT->Channel[0].INTVAL = delay;
  LPC_MRT->Channel[0].INTVAL |= 0x1UL<<31;

  LPC_MRT->Channel[0].CTRL = MRT_REPEATED_MODE|MRT_INT_ENA;

  /* Enable the MRT Interrupt */
#if NMI_ENABLED
  NVIC_DisableIRQ( MRT_IRQn );
  NMI_Init( MRT_IRQn );
#else
  NVIC_EnableIRQ(MRT_IRQn);
#endif
  return;
}

void mrtDelay(uint32_t ticks)
{
  mrt_counter = 0;
  while(mrt_counter < ticks);
}
