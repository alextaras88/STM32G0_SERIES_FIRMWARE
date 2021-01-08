#include "OWEN_485.h"

/******************************************************************************/
/* ОВЕН протокол																															*/
/* Библеотека работает с USART, прием и передача происходят через DMA        */
/* Обработка входящего пакета происходит в прерывании DMA, или выставляется		*/
/* флаг заполнения буфера RX																									*/
/* ОВЕН протокол							*/																														
/******************************************************************************/

extern uint8_t rx_buffer[32];
extern uint8_t tx_buffer[32];

extern uint8_t rx_buffer_2[32];
extern uint8_t tx_buffer_2[32];

/******************************************************************************/
/* Основные функции протокола                                                 */
/******************************************************************************/
void OWEN_485_Init(void){
	
	UART4_Init();
	UART4_DMA_Init();

}
/******************************************************************************/
// Расчет контрольной суммы
int16_t CRC16(uint8_t * dataIN, uint16_t len){
	
	int i, j, crc;
  crc = 0;
	
    for (i = 0; i < len; ++i)
	{
    	unsigned char b = dataIN[i];
    	for (j = 0; j < 8; ++j, b <<= 1)
    	{
    		if (((b ^ (crc >> 8)) & 0x80) > 0)
    		{
    			crc <<= 1;
    			crc ^= 0x8F57;
    		}
			else
				crc <<= 1;
		 }
	}
	return crc;

}
/******************************************************************************/
// Преобразование байта в формат овен протокола
uint16_t Convert_Data_To_OwenData(uint8_t dataIN){
	
	uint16_t res;
	res = (uint8_t) ((dataIN & 0x0F) + 0x47 );
	res +=((uint8_t)(((dataIN >> 4) & 0x0F) + 0x47) << 8);
	
	return res;

}
/******************************************************************************/
// Преобразование овен протокола в байт
uint8_t Convert_OwenData_To_Data(uint8_t byte1, uint8_t byte2){
	
	uint8_t res = (uint8_t) (byte1 - 0x47);
	
	res  = (uint8_t) (res << 4);
	res += (uint8_t) (byte2 - 0x47);
	
	return res;

}
/******************************************************************************/
/*  функции для ТРМ101                                                 				*/
/******************************************************************************/
// Запрос значения температуры HASH:0xB8D7(по адресу прибора)
void TRM101_GetTemperature(uint8_t addr){
	
	uint8_t dataOut[4];
	uint16_t crc;
	uint16_t owen_data;
	
	dataOut[0] = (uint8_t)addr;			// адрес 0..255
	dataOut[1] = 0x10;  						// число байт данных
	dataOut[2] = 0xB8;							// HASH msb
	dataOut[3] = 0xDF;							// HASH lsb 
	
	crc = CRC16(dataOut, 4);				// контрольная сумма dataOut
	
	tx_buffer[0] = 0x23;						// Индификатор начала пакета
	
	owen_data = Convert_Data_To_OwenData(dataOut[0]);
	tx_buffer[2] = (uint8_t)(owen_data & 0xFF);
	tx_buffer[1] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[1]);
	tx_buffer[4] = (uint8_t)(owen_data & 0xFF);
	tx_buffer[3] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[2]);
	tx_buffer[6] = (uint8_t)(owen_data & 0xFF);
	tx_buffer[5] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[3]);
	tx_buffer[8] = (uint8_t)(owen_data & 0xFF);
	tx_buffer[7] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData((uint8_t) (crc >> 8));
	tx_buffer[10] = (uint8_t)(owen_data & 0xFF);
	tx_buffer[9] =  (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData((uint8_t) (crc & 0xFF));
	tx_buffer[12] = (uint8_t)(owen_data & 0xFF);
	tx_buffer[11] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	tx_buffer[13] = 0x0D;				// Индификатор конца пакета
	
	RS485_1_HIGH;
	UART4_DMA_Send(14);
	
	while(DMA1->ISR & DMA_ISR_TCIF1){};
  while(!(USART4->ISR & USART_ISR_TC));
   
	RS485_1_LOW;
	
}
//*****************************************************************************/
/*  функции для MB110-224.1TД                                               	*/
/******************************************************************************/
// Запрос значения  оперативных параметров(по адресу прибора)
/******************************************************************************/
void MB110_224_Get_Value(uint8_t addr, uint16_t hash){
	
	uint8_t dataOut[4];
	uint16_t crc;
	uint16_t owen_data;
	
	dataOut[0] = (uint8_t)addr;							// адрес 0..255
	dataOut[1] = 0x10;  										// число байт данных
	dataOut[2] = (uint8_t)(hash>>8)&0xFF;		// HASH msb
	dataOut[3] = (uint8_t)hash & 0xFF;			// HASH lsb 
	
	crc = CRC16(dataOut, 4);								// контрольная сумма dataOut
	
	tx_buffer[0] = 0x23;										// Индификатор начала пакета
	
	owen_data = Convert_Data_To_OwenData(dataOut[0]);
	tx_buffer[2] = (uint8_t)(owen_data & 0xFF);
	tx_buffer[1] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[1]);
	tx_buffer[4] = (uint8_t)(owen_data & 0xFF);
	tx_buffer[3] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[2]);
	tx_buffer[6] = (uint8_t)(owen_data & 0xFF);
	tx_buffer[5] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[3]);
	tx_buffer[8] = (uint8_t)(owen_data & 0xFF);
	tx_buffer[7] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData((uint8_t) (crc >> 8));
	tx_buffer[10] = (uint8_t)(owen_data & 0xFF);
	tx_buffer[9] =  (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData((uint8_t) (crc & 0xFF));
	tx_buffer[12] = (uint8_t)(owen_data & 0xFF);
	tx_buffer[11] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	tx_buffer[13] = 0x0D;				// Индификатор конца пакета
	
	RS485_1_HIGH;
	UART4_DMA_Send(14);
	
	while(DMA1->ISR & DMA_ISR_TCIF1){};
  while(!(USART4->ISR & USART_ISR_TC));
   
	RS485_1_LOW;
	
}
/******************************************************************************/
// SI30 Functions
/******************************************************************************/
void SI30_Write_1byte(uint16_t addr, uint16_t hash, uint8_t data){
	
	uint8_t dataOut[6];
	uint16_t crc;
	uint16_t owen_data;
	
	dataOut[0] = (uint8_t)addr;							// адрес 0..255
	dataOut[1] = 0x01;  										// число байт данных
	dataOut[2] = (uint8_t)(hash>>8)&0xFF;		// HASH msb
	dataOut[3] = (uint8_t)hash & 0xFF;			// HASH lsb 
	
	dataOut[4] = data;		//data
	 
	
	crc = CRC16(dataOut, 5);								// контрольная сумма dataOut
	
	tx_buffer_2[0] = 0x23;										// Индификатор начала пакета
	
	owen_data = Convert_Data_To_OwenData(dataOut[0]);
	tx_buffer_2[2] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[1] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[1]);
	tx_buffer_2[4] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[3] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[2]);
	tx_buffer_2[6] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[5] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[3]);
	tx_buffer_2[8] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[7] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	//
	owen_data = Convert_Data_To_OwenData(dataOut[4]);
	tx_buffer_2[10] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[9] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	//
	owen_data = Convert_Data_To_OwenData((uint8_t) (crc >> 8));
	tx_buffer_2[12] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[11] =  (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData((uint8_t) (crc & 0xFF));
	tx_buffer_2[14] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[13] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	tx_buffer_2[15] = 0x0D;				// Индификатор конца пакета
	
	RS485_2_HIGH;
	
	USART_Send_Buffer(USART2, tx_buffer_2, 16);
	while (!(USART2->ISR & USART_ISR_TC));
	
	RS485_2_LOW;
	
}
/******************************************************************************/
void SI30_Write_2byte(uint16_t addr, uint16_t hash, uint16_t data){
	
	uint8_t dataOut[6];
	uint16_t crc;
	uint16_t owen_data;
	
	dataOut[0] = (uint8_t)addr;							// адрес 0..255
	dataOut[1] = 0x02;  										// число байт данных
	dataOut[2] = (uint8_t)(hash>>8)&0xFF;		// HASH msb
	dataOut[3] = (uint8_t)hash & 0xFF;			// HASH lsb 
	
	dataOut[4] = (uint8_t)(data>>8);		//data
	dataOut[5] = (uint8_t)data;		// data msb 
	
	crc = CRC16(dataOut, 6);								// контрольная сумма dataOut
	
	tx_buffer_2[0] = 0x23;										// Индификатор начала пакета
	
	owen_data = Convert_Data_To_OwenData(dataOut[0]);
	tx_buffer_2[2] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[1] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[1]);
	tx_buffer_2[4] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[3] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[2]);
	tx_buffer_2[6] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[5] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[3]);
	tx_buffer_2[8] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[7] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	//
	owen_data = Convert_Data_To_OwenData(dataOut[4]);
	tx_buffer_2[10] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[9] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[5]);
	tx_buffer_2[12] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[11] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	//
	owen_data = Convert_Data_To_OwenData((uint8_t) (crc >> 8));
	tx_buffer_2[14] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[13] =  (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData((uint8_t) (crc & 0xFF));
	tx_buffer_2[16] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[15] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	tx_buffer_2[17] = 0x0D;				// Индификатор конца пакета
	
	RS485_2_HIGH;
	
	USART_Send_Buffer(USART2, tx_buffer_2, 18);
	while (!(USART2->ISR & USART_ISR_TC));
	
	RS485_2_LOW;
	
}
/******************************************************************************/
void SI30_Write_4byte(uint16_t addr, uint16_t hash, uint32_t data){
	
	uint8_t dataOut[8];
	uint16_t crc;
	uint16_t owen_data;
	
	dataOut[0] = (uint8_t)addr;							// адрес 0..255
	dataOut[1] = 0x04;  										// число байт данных
	dataOut[2] = (uint8_t)(hash>>8)&0xFF;		// HASH msb
	dataOut[3] = (uint8_t)hash & 0xFF;			// HASH lsb 
	
	dataOut[4] = (uint8_t)(data>>24)&0xFFFFFF;
	dataOut[5] = (uint8_t)(data>>16)&0xFFFF;
	dataOut[6] = (uint8_t)(data>>8)&0xFF;
	dataOut[7] = (uint8_t)data;
	
	crc = CRC16(dataOut, 8);								// контрольная сумма dataOut
	
	tx_buffer_2[0] = 0x23;										// Индификатор начала пакета
	
	owen_data = Convert_Data_To_OwenData(dataOut[0]);
	tx_buffer_2[2] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[1] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[1]);
	tx_buffer_2[4] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[3] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[2]);
	tx_buffer_2[6] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[5] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[3]);
	tx_buffer_2[8] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[7] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	//
	owen_data = Convert_Data_To_OwenData(dataOut[4]);
	tx_buffer_2[10] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[9] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[5]);
	tx_buffer_2[12] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[11] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[6]);
	tx_buffer_2[14] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[13] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[7]);
	tx_buffer_2[16] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[15] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	//
	owen_data = Convert_Data_To_OwenData((uint8_t) (crc >> 8));
	tx_buffer_2[18] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[17] =  (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData((uint8_t) (crc & 0xFF));
	tx_buffer_2[20] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[19] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	tx_buffer_2[21] = 0x0D;				// Индификатор конца пакета
	
	RS485_2_HIGH;
	
	USART_Send_Buffer(USART2, tx_buffer_2, 22);
	while (!(USART2->ISR & USART_ISR_TC));
	
	RS485_2_LOW;
	
}
/******************************************************************************/
void SI30_GetCNT(uint8_t addr){
	
	uint8_t dataOut[4];
	uint16_t crc;
	uint16_t owen_data;
	
	dataOut[0] = (uint8_t)addr;			// адрес 0..255
	dataOut[1] = 0x10;  						// число байт данных
	dataOut[2] = 0xA1;							// HASH msb
	dataOut[3] = 0x58;							// HASH lsb 
	
	crc = CRC16(dataOut, 4);				// контрольная сумма dataOut
	
	tx_buffer_2[0] = 0x23;						// Индификатор начала пакета
	
	owen_data = Convert_Data_To_OwenData(dataOut[0]);
	tx_buffer_2[2] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[1] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[1]);
	tx_buffer_2[4] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[3] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[2]);
	tx_buffer_2[6] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[5] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData(dataOut[3]);
	tx_buffer_2[8] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[7] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData((uint8_t) (crc >> 8));
	tx_buffer_2[10] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[9] =  (uint8_t)((owen_data >> 8) & 0xFF);
	
	owen_data = Convert_Data_To_OwenData((uint8_t) (crc & 0xFF));
	tx_buffer_2[12] = (uint8_t)(owen_data & 0xFF);
	tx_buffer_2[11] = (uint8_t)((owen_data >> 8) & 0xFF);
	
	tx_buffer_2[13] = 0x0D;				// Индификатор конца пакета
	
	RS485_2_HIGH;
	
	USART_Send_Buffer(USART2, tx_buffer_2, 14);
	while (!(USART2->ISR & USART_ISR_TC));
	
	RS485_2_LOW;
	
}
/******************************************************************************/