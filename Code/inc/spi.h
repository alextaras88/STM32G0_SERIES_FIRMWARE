#pragma once

#include "stm32g0xx.h"

#define SPI1_DR_8bit          (*(__IO uint8_t *)((uint32_t)&(SPIx->DR)))
#define SPI1_DR_16bit          (*(__IO uint16_t *)((uint32_t)&(SPIx->DR))) 

#define SPI_IS_BUSY(SPIx)         (((SPIx)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((SPIx)->SR & SPI_SR_BSY))
#define SPI_WAIT(SPIx)            while (SPI_IS_BUSY(SPIx))

uint8_t Spi_transfer_8(SPI_TypeDef* SPIx, uint8_t data);
uint16_t Spi_transfer_16(SPI_TypeDef* SPIx, uint16_t data);
void Spi_multi_transfer_8(SPI_TypeDef* SPIx, uint8_t* dataOUT,uint8_t* dataIN, uint16_t size);

