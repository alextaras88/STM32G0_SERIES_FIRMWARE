#include "uart.h"

uint8_t rx_buffer[32];
uint8_t tx_buffer[32];

uint8_t rx_buffer_2[32];
uint8_t tx_buffer_2[32];

/*********************************************************/
// RS485_2
void UART2_Init(void){
	
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;											// Clock GPIOA
	RCC->IOPENR |= RCC_IOPENR_GPIOCEN;											// Clock GPIOC
	RCC->APBENR1 |= RCC_APBENR1_USART2EN;										// Clock USART2
	// PA2 TX
	GPIOA->MODER &= ~GPIO_MODER_MODE2;		
	GPIOA->MODER |= GPIO_MODER_MODE2_1;										  // AF output PP 
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED2;	     	 	    	  // High speed
	// PA3 RX
	GPIOA->MODER &= ~GPIO_MODER_MODE3;	
	GPIOA->MODER  |= GPIO_MODER_MODE3_1;										// Floating input
	// PC15 RS485 RX_TX
	GPIOC->MODER &= ~GPIO_MODER_MODE15;	
	GPIOC->MODER   |= GPIO_MODER_MODE15_0;   			 			 	  // OUT PA_4
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT15;  							 		  // Push pull
	GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED15;	     	 			  // High speed
	
	RS485_2_LOW;
	
	GPIOA->AFR[0] |= (1<<8)|(1<<12);												// PA2, PA3 - GPIO AF1
	
  USART2->CR1 |= USART_CR1_OVER8;													// 8 bit data
	USART2->BRR = 0x0112;																		// 115200
	//USART2->BRR = 0x0D05;																		// 9600
	
	USART2->CR1 |= USART_CR1_TE;                 
	USART2->CR1 |= USART_CR1_RE;                 
	//USART2->CR1 |= USART_CR1_RXNEIE_RXFNEIE; 
  USART2->CR1 |= USART_CR1_UE; 	
	
	//NVIC_EnableIRQ(USART2_IRQn);
	
}
/*********************************************************/
void UART3_Init(void){
	
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;											// Clock GPIOB
	RCC->APBENR1 |= RCC_APBENR1_USART3EN;										// Clock USART3
	// PB8 TX
	GPIOB->MODER &= ~GPIO_MODER_MODE8;		
	GPIOB->MODER |= GPIO_MODER_MODE8_1;										  // AF output PP 
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED8;	     	 	    	  // High speed
	// PB9 RX
	GPIOB->MODER &= ~GPIO_MODER_MODE9;	
	GPIOB->MODER  |= GPIO_MODER_MODE9_1;										// Floating input
	
	GPIOB->AFR[1] |= (4<<0)|(4<<4);													// PB8, PB4 - GPIO AF4
	
  //USART3->CR1 |= USART_CR1_OVER8;												// 8 bit data
	USART3->BRR = 0x022B;																		// 115200
	//USART3->BRR = 0x1A0A;																		// 9600
	
	USART3->CR1 |= USART_CR1_TE;                 
	USART3->CR1 |= USART_CR1_RE;                 
	//USART3->CR1 |= USART_CR1_RXNEIE_RXFNEIE; 
  USART3->CR1 |= USART_CR1_UE; 	
	
	//NVIC_EnableIRQ(USART3_IRQn);
	
}
/*********************************************************/
// RS485_1
void UART4_Init(void){
	
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;											// Clock GPIOA
	RCC->IOPENR |= RCC_IOPENR_GPIOCEN;											// Clock GPIOC
	RCC->APBENR1 |= RCC_APBENR1_USART4EN;										// Clock USART4
	// PA0 TX
	GPIOA->MODER &= ~GPIO_MODER_MODE0;		
	GPIOA->MODER |= GPIO_MODER_MODE0_1;										  // AF output PP 
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED0;	     	 	    	  // High speed
	// PA1 RX
	GPIOA->MODER &= ~GPIO_MODER_MODE1;	
	GPIOA->MODER  |= GPIO_MODER_MODE1_1;										// Floating input
	// PC15 RS485 RX_TX
	GPIOC->MODER &= ~GPIO_MODER_MODE14;	
	GPIOC->MODER   |= GPIO_MODER_MODE14_0;   			 			 	  // OUT PA_4
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT14;  							 		  // Push pull
	GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED14;	     	 			  // High speed
	
	RS485_1_LOW;
	
	GPIOA->AFR[0] |= (4<<0)|(4<<4);													// PA0, PA1 - GPIO AF4
	
  //USART4->CR1 |= USART_CR1_OVER8;												// 8 bit data
	USART4->BRR = 0x022B;																		// 115200
	//USART4->BRR = 0x1A0A;																		// 9600
	
	USART4->CR1 |= USART_CR1_TE;                 
	USART4->CR1 |= USART_CR1_RE;                 
	//USART4->CR1 |= USART_CR1_RXNEIE_RXFNEIE; 
	USART4->CR1 |= USART_CR1_UE; 	
	
	//NVIC_EnableIRQ(USART1_IRQn);
	
}
/*********************************************************/
void USART_Send (USART_TypeDef* USARTx,char chr){  
	
	while (!(USARTx->ISR & USART_ISR_TC));
	USARTx->TDR = chr;
	
}
/*********************************************************/
void USART_Send_String (USART_TypeDef* USARTx, char* str){           
	
	uint8_t i = 0;
	
	while(str[i])
	USART_Send(USARTx, str[i++]);
}
/*********************************************************/
void USART_Send_Buffer(USART_TypeDef* USARTx, uint8_t *pBuff, uint16_t Len){
	
	while (Len--){
		
    USART_Send(USARTx, *(pBuff++));
	
	}
}
/*********************************************************/
void USART_Send_String_rs485 (USART_TypeDef* USARTx, char* str){           
	
	uint8_t i = 0;
	
	RS485_1_HIGH;
	while(str[i])
	USART_Send(USARTx, str[i++]);
	while(!(USARTx->ISR & USART_ISR_TC));
	RS485_1_LOW;
	
}
/*********************************************************/
//RS481_1
void UART4_DMA_Init(void){
	
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	
	//Tx DMA	
	DMAMUX1_Channel0->CCR = 0x00000239;									// DMA request MUX input - 57, Event generation enable
	
	DMA1_Channel1->CPAR = (uint32_t)&USART4->TDR;
	DMA1_Channel1->CMAR = (uint32_t)&tx_buffer[0];
	DMA1_Channel1->CNDTR =  32;													// Size buffer
	
	DMA1_Channel1->CCR  &= ~DMA_CCR_CIRC;								// Disenable cycle mode
	DMA1_Channel1->CCR  &= ~DMA_CCR_PINC;								// Disenable increment pointer periphery
	
	DMA1_Channel1->CCR  &= ~DMA_CCR_PSIZE;							// Size data periphery - 8 bit
	DMA1_Channel1->CCR  &= ~DMA_CCR_MSIZE; 							// Size data memory - 8 bit
	
	DMA1_Channel1->CCR  |=  DMA_CCR_DIR;								// Read: memory -> periphery
	DMA1_Channel1->CCR  |=  DMA_CCR_MINC;								// Disenable increment pointer memory
	
	USART4->CR3        |=  USART_CR3_DMAT;
	
	//Rx DMA
	DMAMUX1_Channel1->CCR = 0x00000038;									// // DMA request MUX input - 56	
	
	DMA1_Channel2->CMAR = (uint32_t)&rx_buffer[0];       //????? ?????? ?????????
	DMA1_Channel2->CPAR = (uint32_t)&USART4->RDR;      	 //????? ???????? ?????? ?????????
	DMA1_Channel2->CNDTR = 20;                         	 //??? ?????????
	DMA1_Channel2->CCR |= DMA_CCR_MINC;
	DMA1_Channel2->CCR |= DMA_CCR_CIRC;
	DMA1_Channel2->CCR |= DMA_CCR_TEIE;
	DMA1_Channel2->CCR |= DMA_CCR_TCIE; 
	DMA1_Channel2->CCR |= DMA_CCR_EN;
	
	NVIC_EnableIRQ (DMA1_Channel1_IRQn);
	NVIC_EnableIRQ (DMA1_Channel2_3_IRQn);
	
	USART4->CR3        |=  USART_CR3_DMAR;
		
}
/*************************************************************************************/
void UART2_DMA_Init(void){
	
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	
	DMAMUX1_Channel2->CCR = 0x00000034;									// // DMA request MUX input - 52	
	
	DMA1_Channel3->CMAR = (uint32_t)&rx_buffer_2[0];       //????? ?????? ?????????
	DMA1_Channel3->CPAR = (uint32_t)&USART2->RDR;      	 //????? ???????? ?????? ?????????
	DMA1_Channel3->CNDTR = 21;                         	 //??? ?????????
	DMA1_Channel3->CCR |= DMA_CCR_MINC;
	DMA1_Channel3->CCR |= DMA_CCR_CIRC;
	DMA1_Channel3->CCR |= DMA_CCR_TEIE;
	DMA1_Channel3->CCR |= DMA_CCR_TCIE; 
	DMA1_Channel3->CCR |= DMA_CCR_EN;
	
	NVIC_EnableIRQ (DMA1_Channel2_3_IRQn);
	
	USART2->CR3        |=  USART_CR3_DMAR;
		
}
/*********************************************************/
void UART4_DMA_Send(uint16_t len){
	
	DMA1_Channel1->CCR  &= ~DMA_CCR_EN;      
	DMA1_Channel1->CNDTR =  len; 
	DMA1->IFCR          |=  DMA_IFCR_CTCIF1;
	DMA1_Channel1->CCR  |=  DMA_CCR_EN;	
  
}
/*********************************************************/