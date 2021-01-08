#pragma once

#include "stm32g0xx.h"

#define RS485_1_LOW			GPIOC->ODR &= ~GPIO_ODR_OD14;
#define RS485_1_HIGH		GPIOC->ODR |= GPIO_ODR_OD14;

#define RS485_2_LOW			GPIOC->ODR &= ~GPIO_ODR_OD15;
#define RS485_2_HIGH		GPIOC->ODR |= GPIO_ODR_OD15;

void UART2_Init(void);
void UART3_Init(void);
void UART4_Init(void);

void UART4_DMA_Init(void);
void UART2_DMA_Init(void);
void UART4_DMA_Send(uint16_t len);

void USART_Send (USART_TypeDef* USARTx,char chr);
void USART_Send_String (USART_TypeDef* USARTx, char* str);
void USART_Send_Buffer(USART_TypeDef* USARTx, uint8_t *pBuff, uint16_t Len);
void USART_Send_String_rs485 (USART_TypeDef* USARTx, char* str);
	