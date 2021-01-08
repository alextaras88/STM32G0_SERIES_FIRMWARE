#include "spi.h"

uint8_t Spi_transfer_8(SPI_TypeDef *SPIx, uint8_t data)
{

	SPI_WAIT(SPIx);
	SPI1_DR_8bit = data;
	SPI_WAIT(SPIx);

	return SPI1_DR_8bit;
}
/****************************************************************************************/
uint16_t Spi_transfer_16(SPI_TypeDef *SPIx, uint16_t data)
{

	SPI_WAIT(SPIx);
	SPI1_DR_16bit = (uint16_t)data;
	SPI_WAIT(SPIx);

	return SPI1_DR_16bit;
}
/****************************************************************************************/
void Spi_multi_transfer_8(SPI_TypeDef *SPIx, uint8_t *dataOUT, uint8_t *dataIN, uint16_t size)
{

	uint16_t i;

	SPI_WAIT(SPIx);

	for (i = 0; i < size; i++)
	{

		SPI1_DR_8bit = dataOUT[i];
		SPI_WAIT(SPIx);
		dataIN[i] = SPI1_DR_8bit;
	}
}
/****************************************************************************************/
