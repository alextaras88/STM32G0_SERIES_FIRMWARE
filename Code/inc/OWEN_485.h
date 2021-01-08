#pragma once

#include "stm32g0xx.h"
#include "uart.h"
#include "delay.h"

/***********************************/
#define MB110_GET_VOLTS 		0x7F46
#define MB110_GET_WEIGHT 		0x399C
/***********************************/
#define SI30_SET_U1					0xFA1E
#define SI30_SET_U2					0x9707
#define SI30_SET_MODE COUNT 0x38D9
#define SI30_SET_MODE_OUT		0xD11F
#define SI30_SET_LOCK_BTN		0xE954
#define SI30_SET_U_DISPLAY  0xCAA0
#define SI30_SET_ADDR_OWEN  0x9F62

#define SI30_GET_CNT  			0xA158
#define SI30_GET_PHY_VAL 		0xB8BC

#define SI30_GET_STST  			0x6577
#define SI30_RESET_IN_STATE 0xC010
#define SI30_LOCK_IN_STATE  0x93EC
#define SI30_OUT1_STATE  		0x6FFD
#define SI30_OUT2_STATE  		0x52A1
#define SI30_RESET_CNT  		0xDF1F
/******************************************************************************/
void OWEN_485_Init(void);
int16_t CRC16(uint8_t * dataIN, uint16_t len);
uint16_t Convert_Data_To_OwenData(uint8_t dataIN);
uint8_t Convert_OwenData_To_Data(uint8_t byte1, uint8_t byte2);
/*************************/
void TRM101_GetTemperature(uint8_t addr);
void MB110_224_Get_Value(uint8_t addr, uint16_t hash);
/******************************************************************************/
void SI30_Write_1byte(uint16_t addr, uint16_t hash, uint8_t data);
void SI30_Write_2byte(uint16_t addr, uint16_t hash, uint16_t data);
void SI30_Write_4byte(uint16_t addr, uint16_t hash, uint32_t data);

void SI30_GetCNT(uint8_t addr);
/******************************************************************************/



