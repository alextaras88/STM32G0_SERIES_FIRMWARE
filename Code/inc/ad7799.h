#pragma once

#include "stm32g0xx.h"
#include "delay.h"
#include "spi.h"

/*****************************************************************************/
#define PGA 128.0f
#define VREF 2.999f
#define VFSR VREF/PGA
#define FSR (((int32_t)1<<23)-1)
/*****************************************************************************/

#define		CS_LOW		GPIOA->ODR &= ~GPIO_ODR_OD15;
#define 	CS_HIGH		GPIOA->ODR |=  GPIO_ODR_OD15;

#define CHANNEL_1 	(0x00)
#define CHANNEL_2 	(0x01)
#define CHANNEL_3 	(0x02)

#define BO_OFF		(0x00)
#define BO_ON		(0x40)

#define BIPOLAR_IN	(0x00)
#define UNIPOLAR_IN	(0x10) 

#define GAIN_1		(0x00)
#define GAIN_2		(0x01)
#define GAIN_4		(0x02)
#define GAIN_8		(0x03)
#define GAIN_16		(0x04)
#define GAIN_32		(0x05)
#define GAIN_64		(0x06)
#define GAIN_128	(0x07)


#define DATARATE_470	(0x01)
#define DATARATE_242	(0x02)
#define DATARATE_123	(0x03)
#define DATARATE_62		(0x04)
#define DATARATE_50		(0x05)
#define DATARATE_39		(0x06)
#define DATARATE_33_2	(0x07)
#define DATARATE_19_6_60_HZ	(0x08)
#define DATARATE_16_7_50_HZ	(0x09)
#define DATARATE_16_7	(0x0A)
#define DATARATE_12_5	(0x0B)
#define DATARATE_10		(0x0C)
#define DATARATE_8_33	(0x0D)
#define DATARATE_6_25	(0x0E)
#define DATARATE_4_17	(0x0F)
/*************************************/
typedef enum
{
	ADC_CHANNEL_1  = CHANNEL_1,
	ADC_CHANNEL_2  = CHANNEL_2,
	ADC_CHANNEL_3  = CHANNEL_3
	
} ad7799_channel;
/*************************************/
typedef enum
{
	ADC_BO_OFF  = BO_OFF,
	ADC_BO_ON  = BO_ON
	
} ad7799_burnout_current;
/*************************************/
typedef enum
{	
	ADC_BIPOLAR_IN  = BIPOLAR_IN,
	ADC_UNIPOLAR_IN  = UNIPOLAR_IN
	
} ad7799_in_polar;
/*************************************/
typedef enum
{
	ADC_GAIN_1 = GAIN_1, 
	ADC_GAIN_2 = GAIN_2, 
	ADC_GAIN_4 = GAIN_4,
	ADC_GAIN_8 = GAIN_8,
	ADC_GAIN_16 = GAIN_16, 
	ADC_GAIN_32 = GAIN_32, 
	ADC_GAIN_64 = GAIN_64, 
	ADC_GAIN_128 = GAIN_128 
	
} ad7799_gain;
/*************************************/
typedef enum
{	
	ADC_DATARATE_470  = DATARATE_470,
	ADC_DATARATE_242  = DATARATE_242,
	ADC_DATARATE_123  = DATARATE_123,
	ADC_DATARATE_62  = DATARATE_62,
	ADC_DATARATE_50  = DATARATE_50,
	ADC_DATARATE_39  = DATARATE_39,
	ADC_DATARATE_33_2  = DATARATE_33_2,
	ADC_DATARATE_19_6_60_HZ  = DATARATE_19_6_60_HZ,
	ADC_DATARATE_16_7_50HZ  = DATARATE_16_7_50_HZ,
	ADC_DATARATE_16_7  = DATARATE_16_7,
	ADC_DATARATE_12_5  = DATARATE_12_5,
	ADC_DATARATE_10  = DATARATE_10,
	ADC_DATARATE_8_33  = DATARATE_8_33,
	ADC_DATARATE_6_25  = DATARATE_6_25,
	ADC_DATARATE_4_17  = DATARATE_4_17,
	
} ad7799_datarate;
/*************************************/

/*****************************************************************************/
void AD7799_Init(void);
void _AD7799_Init(ad7799_channel ch, ad7799_burnout_current bo,  ad7799_in_polar polar, ad7799_gain gain, ad7799_datarate speed);
void AD7799_Reset(void);
unsigned long getADC7799(void);
void AD7799_Set_CH_1(void);
void AD7799_Set_CH_2(void);
void AD7799_Set_CH_3(void);
float ad7799_Get_V_Unipolar(uint8_t sample);
float ad7799_Get_V_Bipolar(uint8_t sample);

void Internal_Zero_Scale_Calibration(ad7799_datarate speed);
void Internal_Full_Scale_Calibration(ad7799_datarate speed);
uint8_t AD7799_Read_ID(void);
uint32_t AD7799_Read_OFFSET(void);
uint32_t AD7799_Read_FULL_SCALE(void);
/*****************************************************************************/
