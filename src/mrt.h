/**************************************************************************/
/*!
    @file     mrt.h
    @author   K. Townsend
*/
/**************************************************************************/
#ifndef __MRT_H
#define __MRT_H

/* Control register bit definition. */
#define MRT_INT_ENA          (0x1<<0)
#define MRT_REPEATED_MODE    (0x00<<1)
#define MRT_ONE_SHOT_INT     (0x01<<1)
#define MRT_ONE_SHOT_STALL   (0x02<<1)

/* Status register bit definition */
#define MRT_STAT_IRQ_FLAG    (0x1<<0)
#define MRT_STAT_RUN         (0x1<<1)

void MRT_IRQHandler(void);
void mrtInit(uint32_t delay);
void mrtDelay(uint32_t ticks);

#endif
