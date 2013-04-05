/**************************************************************************/
/*!
    @file     spi.h
    @author   K. Townsend
*/
/**************************************************************************/
#ifndef _SPI_H_
#define _SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LPC8xx.h"
#include "errors.h"
#include "asserts.h"

#define SPI_CFG_ENABLE          (1 << 0)
#define SPI_CFG_MASTER          (1 << 2)

#define SPI_DLY_PREDELAY(d)     ((d) << 0)
#define SPI_DLY_POSTDELAY(d)    ((d) << 4)
#define SPI_DLY_FRAMEDELAY(d)   ((d) << 8)
#define SPI_DLY_INTERDELAY(d)   ((d) << 12)

#define SPI_TXDATCTL_EOT        (1 << 20)
#define SPI_TXDATCTL_FSIZE(s)   ((s) << 24)

#define SPI_STAT_RXRDY          (1 << 0)
#define SPI_STAT_TXRDY          (1 << 1)

void    spiInit     ( LPC_SPI_TypeDef *SPIx, uint32_t div, uint32_t delay );
uint8_t spiTransfer ( LPC_SPI_TypeDef *SPIx, uint8_t data );

#ifdef __cplusplus
}
#endif

#endif
