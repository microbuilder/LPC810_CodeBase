/**************************************************************************/
/*!
    @file     spi.c
    @author   K. Townsend
*/
/**************************************************************************/
#include <string.h>

#include "spi.h"

/* Configure SPI as Master in Mode 0 (CPHA and CPOL = 0) */
void spiInit(LPC_SPI_TypeDef *SPIx, uint32_t div, uint32_t delay)
{
  /* Enable SPI clock */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<11);

  /* Bring SPI out of reset */
  LPC_SYSCON->PRESETCTRL &= ~(0x1<<0);
  LPC_SYSCON->PRESETCTRL |= (0x1<<0);

  /* Set clock speed and mode */
  SPIx->DIV = div;
  SPIx->DLY = delay;
  SPIx->CFG = (SPI_CFG_MASTER & ~SPI_CFG_ENABLE);
  SPIx->CFG |= SPI_CFG_ENABLE;
}

/* Send/receive data via the SPI bus (assumes 8bit data) */
uint8_t spiTransfer(LPC_SPI_TypeDef *SPIx, uint8_t data)
{
  while ( (SPIx->STAT & SPI_STAT_TXRDY) == 0 );
  SPIx->TXDATCTL = SPI_TXDATCTL_FSIZE(8-1) | SPI_TXDATCTL_EOT | data;
  while ( (SPIx->STAT & SPI_STAT_RXRDY) == 0 );
  return SPIx->RXDAT;
}
