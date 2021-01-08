#pragma once

#include "stm32g0xx.h"

/********************************************************************/
#define _I2C_MASTER_WRITE         	     0x00
#define _I2C_MASTER_READ     			 0x01

#define _I2C_MASTER_SINGLE_SEND          0x00
#define _I2C_MASTER_BURST_SEND_START     0x01
#define _I2C_MASTER_BURST_SEND_CONT      0x02
#define _I2C_MASTER_BURST_SEND_FINISH    0x03

#define _I2C_MASTER_SINGLE_RECEIVE       0x00
#define _I2C_MASTER_BURST_RECEIVE_START  0x01
#define _I2C_MASTER_BURST_RECEIVE_CONT   0x02
#define _I2C_MASTER_BURST_RECEIVE_FINISH 0x03

/****************************************************************/
void I2C1_Init(void);
void i2c_Start_addr(I2C_TypeDef* I2Cx,  uint8_t addr, uint8_t dir);
void i2c_Write(I2C_TypeDef* I2Cx, uint8_t mode, uint8_t data);
void i2c_Read(I2C_TypeDef* I2Cx, uint8_t *data ,uint8_t mode);
void i2c_BUSY(I2C_TypeDef* I2Cx);
/********************************************************************/

