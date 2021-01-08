#include "main.h"

char str[50];
char str_d[20];
float Voltage;
float res;
float temp;
uint32_t adc;
uint32_t offset = 0;
uint32_t f_scale;
uint8_t id;
uint8_t status = 0;

int main(void)
{

	Clock_Init();
	Delay_Init();
	UART4_Init();
	USART_Send_String_rs485(USART4, "AD7799 Test programm\r\n");
	I2C1_Init();
	ssd1306_Init();
	ssd1306_SetCursor(10, 0);
	ssd1306_WriteString("AD7799 RTD", Font_11x18, White);
	ssd1306_UpdateScreen();
	USART_Send_String_rs485(USART4, "SSD1306 Init!\r\n");
	_AD7799_Init(ADC_CHANNEL_1, ADC_BO_OFF, BIPOLAR_IN, ADC_GAIN_2, ADC_DATARATE_50);
	USART_Send_String_rs485(USART4, "AD7799 Init!\r\n");

	//id =  AD7799_Read_ID();
	//if ((id&0x0f)== 0x09) status = 0x99;
	//offset = AD7799_Read_OFFSET();
	//f_scale = AD7799_Read_FULL_SCALE();

	while (1)
	{

		Voltage = ad7799_Get_V_Bipolar(4);
		adc = getADC7799();
		res = (Voltage / 1000) / 0.0006;
		temp = (res - 100) * 3.85;
		sprintf(str, "%.2fmV   %.2fR   %.2fC   %u\r\n", Voltage, res, temp, adc);
		sprintf(str_d, "%.2fC", temp);
		USART_Send_String_rs485(USART4, str);

		ssd1306_SetCursor(30, 30);
		ssd1306_WriteString(str_d, Font_11x18, White);
		ssd1306_UpdateScreen();
		//Delay_ms(100);
	}
}
/******************************************************/
