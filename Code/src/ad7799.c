#include "ad7799.h"

/*****************************************************************************/
void AD7799_SPI_Init(void)
{

	// GPIO INIT

	RCC->IOPENR |= RCC_IOPENR_GPIOBEN; // Clock GPIOB
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN; // Clock GPIOA

	GPIOB->AFR[0] |= (0 << 12) | (0 << 16) | (0 << 20); // PB3, PB4, PB5  GPIO AF0

	// PB3 SCK
	GPIOB->MODER &= ~GPIO_MODER_MODE3;
	GPIOB->MODER |= GPIO_MODER_MODE3_1;			// AF
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT3;			// PP
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED3; // Full Speed
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD3;			// No pull-up, pull-down

	// PB4 MISO
	GPIOB->MODER &= ~GPIO_MODER_MODE4;
	GPIOB->MODER |= GPIO_MODER_MODE4_1; // INPUT Floating
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD4;

	// PB5 MOSI
	GPIOB->MODER &= ~GPIO_MODER_MODE5;
	GPIOB->MODER |= GPIO_MODER_MODE5_1;			// AF
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT5;			// PP
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED5; // Full Speed
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD5;			// No pull-up, pull-down

	// PA15 CS
	GPIOA->MODER &= ~GPIO_MODER_MODE15;
	GPIOA->MODER |= GPIO_MODER_MODE15_0;		 // OUTPUT
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT15;			 // PP
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED15; // Full Speed
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD15;			 // No pull-up, pull-down

	// SPI1 INIT
	RCC->APBENR2 |= RCC_APBENR2_SPI1EN; // Clock SPI1

	SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1; // Spi Clock div 64MHz/128
	SPI1->CR1 |= SPI_CR1_CPOL;
	SPI1->CR1 |= SPI_CR1_CPHA;
	SPI1->CR2 &= ~SPI_CR2_DS;
	SPI1->CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2; // 8 Bit Data
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;													 // MSB First
	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;									 // Software slave management & Internal slave select
	SPI1->CR1 |= SPI_CR1_MSTR;															 // SPI Master Mode

	SPI1->CR1 |= SPI_CR1_SPE; // SPI Enabl
}
/*****************************************************************************/
void AD7799_Init(void)
{

	AD7799_SPI_Init();

	Delay_ms(200);

	AD7799_Reset();
	Delay_ms(50);

	CS_LOW;
	// Set CH1
	Delay_ms(1);
	Spi_transfer_8(SPI1, 0x07); //0x0710 CONFIGURATION REGISTER
	Spi_transfer_8(SPI1, 0x10);
	Delay_ms(1);
	Spi_transfer_8(SPI1, 0x50);
	Spi_transfer_8(SPI1, 0x90);
	CS_HIGH;
	// Calibration
	Internal_Zero_Scale_Calibration(0x09);
	Internal_Full_Scale_Calibration(0x09);
	// Set Speed, Filter, Gain
	CS_LOW;
	Delay_ms(1);
	Spi_transfer_8(SPI1, 0x00); // mode reg 0x000A
	Spi_transfer_8(SPI1, 0x0A);
	Delay_ms(1);
	Spi_transfer_8(SPI1, 0x00);
	Spi_transfer_8(SPI1, 0x09); //fADC = 16.6 Hz, 50 Hz only
	//Spi_transfer_8(SPI1, 0x0F); 		//fADC = 4.17 Hz
	//Spi_transfer_8(SPI1, 0x05);		//fADC = 50 Hz
	//Spi_transfer_8(SPI1, 0x0C);		//fADC = 10 Hz
	CS_HIGH;
}
/*****************************************************************************/
void _AD7799_Init(ad7799_channel ch, ad7799_burnout_current bo, ad7799_in_polar polar, ad7799_gain gain, ad7799_datarate speed)
{

	uint8_t config_reg[2];
	uint8_t mode_reg[2];

	config_reg[0] = 0x00 | 0x00 | bo | polar | 0x00 | gain;
	config_reg[1] = 0x00 | 0x00 | 0x00 | 0x10 | 0x00 | ch;

	mode_reg[0] = 0x00;
	mode_reg[1] = speed;

	AD7799_SPI_Init();

	Delay_ms(200);

	AD7799_Reset();
	Delay_ms(50);

	CS_LOW;
	// Set CH1
	Delay_ms(1);
	Spi_transfer_8(SPI1, 0x07); //0x0710 CONFIGURATION REGISTER
	Spi_transfer_8(SPI1, 0x10);
	Delay_ms(1);
	Spi_transfer_8(SPI1, config_reg[0]);
	Spi_transfer_8(SPI1, config_reg[1]);
	CS_HIGH;
	// Calibration
	Internal_Zero_Scale_Calibration(speed);
	Internal_Full_Scale_Calibration(speed);
	// Set Speed, Filter, Gain
	CS_LOW;
	Delay_ms(1);
	Spi_transfer_8(SPI1, 0x00); // mode reg 0x000A
	Spi_transfer_8(SPI1, 0x0A);
	Delay_ms(1);
	Spi_transfer_8(SPI1, mode_reg[0]);
	Spi_transfer_8(SPI1, mode_reg[1]);

	CS_HIGH;
}
/*****************************************************************************/
void AD7799_Reset(void)
{

	Delay_ms(1);
	CS_LOW;
	Spi_transfer_8(SPI1, 0xFF);
	Spi_transfer_8(SPI1, 0xFF);
	Spi_transfer_8(SPI1, 0xFF);
	Spi_transfer_8(SPI1, 0xFF);
	CS_HIGH;
	Delay_ms(500);
}
/************************************************************************************/
unsigned long getADC7799(void)
{

	unsigned char b1 = 0x00, b2 = 0x00, b3 = 0x00;
	unsigned long value = 0x00000000;

	CS_LOW;
	while ((GPIOB->IDR & GPIO_IDR_ID4) != 0)
		;
	Spi_transfer_8(SPI1, 0x58);
	b1 = Spi_transfer_8(SPI1, 0x00);
	b2 = Spi_transfer_8(SPI1, 0x00);
	b3 = Spi_transfer_8(SPI1, 0x00);
	CS_HIGH;
	;

	value += (b1 << 16);
	value += (b2 << 8);
	value += (b3 << 0);

	return value;
}
/************************************************************************************/
void AD7799_Set_CH_1(void)
{

	CS_LOW;
	//Config REG
	Delay_ms(1);
	Spi_transfer_8(SPI1, 0x07); //0x0710 CONFIGURATION REGISTER
	Spi_transfer_8(SPI1, 0x10);
	Delay_ms(1);
	Spi_transfer_8(SPI1, 0x50);
	Spi_transfer_8(SPI1, 0x90);
	CS_HIGH;
}
/************************************************************************************/
void AD7799_Set_CH_2(void)
{

	CS_LOW;
	//Config REG
	Delay_ms(1);
	Spi_transfer_8(SPI1, 0x07); //0x0710 CONFIGURATION REGISTER
	Spi_transfer_8(SPI1, 0x10);
	Delay_ms(1);
	Spi_transfer_8(SPI1, 0x50);
	Spi_transfer_8(SPI1, 0x91);
	CS_HIGH;
}
/************************************************************************************/
void AD7799_Set_CH_3(void)
{

	CS_LOW;
	//Config REG
	Delay_ms(1);
	Spi_transfer_8(SPI1, 0x07); //0x0710 CONFIGURATION REGISTER
	Spi_transfer_8(SPI1, 0x10);
	Delay_ms(1);
	Spi_transfer_8(SPI1, 0x50);
	Spi_transfer_8(SPI1, 0x92);
	CS_HIGH;
}
/************************************************************************************/
float ad7799_Get_V_Unipolar(uint8_t sample)
{

	uint32_t adc = 0;
	float V;

	for (uint8_t i = 0; i < sample; i++)
	{

		adc += getADC7799();
	}

	adc /= sample;

	V = (float)(adc)*0.0000001787543403359f; // UNIPOLAR INPUT

	return V;
}
/************************************************************************************/
float ad7799_Get_V_Bipolar(uint8_t sample)
{

	uint32_t adc = 0;
	float V;

	for (uint8_t i = 0; i < sample; i++)
	{

		adc += getADC7799();
	}

	adc /= sample;

	V = ((float)(adc)*0.00000011920928955078125f - 1) * 3.000f / 2 * 1000; // BIPOLAR INPUT

	return V;
}
/************************************************************************************/
void Internal_Zero_Scale_Calibration(ad7799_datarate speed)
{

	uint8_t mode_2 = speed;

	CS_LOW;
	Delay_ms(1);
	Spi_transfer_8(SPI1, 0x00); // mode reg 0x000A
	Spi_transfer_8(SPI1, 0x0A);
	Delay_ms(1);
	Spi_transfer_8(SPI1, 0x80);
	Spi_transfer_8(SPI1, mode_2);

	while ((GPIOB->IDR & GPIO_IDR_ID4) != 0)
		;
	CS_HIGH;

	Delay_ms(2000);
}
/************************************************************************************/
void Internal_Full_Scale_Calibration(ad7799_datarate speed)
{

	uint8_t mode_2 = speed;

	CS_LOW;
	Delay_ms(1);
	Spi_transfer_8(SPI1, 0x00); // mode reg 0x000A
	Spi_transfer_8(SPI1, 0x0A);
	Delay_ms(1);
	Spi_transfer_8(SPI1, 0xA0);
	Spi_transfer_8(SPI1, mode_2);

	while ((GPIOB->IDR & GPIO_IDR_ID4) != 0)
		;
	CS_HIGH;

	Delay_ms(2000);
}
/************************************************************************************/
uint8_t AD7799_Read_ID(void)
{

	uint8_t id = 0;

	CS_LOW;
	Spi_transfer_8(SPI1, 0x60);
	while ((GPIOB->IDR & GPIO_IDR_ID4) != 0)
		;
	id = Spi_transfer_8(SPI1, 0x00);
	CS_HIGH;

	return id;
}
/************************************************************************************/
uint32_t AD7799_Read_OFFSET(void)
{

	uint8_t b1, b2, b3;
	uint32_t value;

	CS_LOW;

	Spi_transfer_8(SPI1, 0x70);
	while ((GPIOB->IDR & GPIO_IDR_ID4) != 0)
		;
	b1 = Spi_transfer_8(SPI1, 0x00);
	b2 = Spi_transfer_8(SPI1, 0x00);
	b3 = Spi_transfer_8(SPI1, 0x00);

	CS_HIGH;
	;

	value += (b1 << 16);
	value += (b2 << 8);
	value += (b3 << 0);

	return value;
}
/************************************************************************************/
uint32_t AD7799_Read_FULL_SCALE(void)
{

	uint8_t b1, b2, b3;
	uint32_t value;

	CS_LOW;

	Spi_transfer_8(SPI1, 0x78);
	while ((GPIOB->IDR & GPIO_IDR_ID4) != 0)
		;
	b1 = Spi_transfer_8(SPI1, 0x00);
	b2 = Spi_transfer_8(SPI1, 0x00);
	b3 = Spi_transfer_8(SPI1, 0x00);

	CS_HIGH;
	;

	value += (b1 << 16);
	value += (b2 << 8);
	value += (b3 << 0);

	return value;
}
/************************************************************************************/
