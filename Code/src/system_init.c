#include "system_init.h"

/********************************************************************/
void Clock_Init(void){
	
	FLASH->ACR |= FLASH_ACR_LATENCY_1 ;					// Flash latency - Two wait states
	FLASH->ACR |= FLASH_ACR_PRFTEN;						// CPU Prefetch enable
	
	RCC->CR |= ((uint32_t)RCC_CR_HSION);			 	// Enable HSI
	while((RCC->CR & RCC_CR_HSIRDY) ==0){};     		// Wait HSI enable flag
				
	RCC->PLLCFGR = 0x30000802;							// HSI16x1x8/8 = 64MHz
	RCC->CR |= RCC_CR_PLLON;                   		    // Enable PLL
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;
	while((RCC->CR & RCC_CR_PLLRDY) == 0){};    		// Wait PLL enable flag
		
	RCC->CFGR &= ~RCC_CFGR_HPRE;						// AHB prescaler - 1
	RCC->CFGR &= ~RCC_CFGR_PPRE;						// AHP prescaler - 1
	
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_1;				// Clock USART2 - 16MHz
		
	RCC->CFGR |= RCC_CFGR_SW_1;                 			// SYSCLK = PLL
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {};// Wait PLL-SYSCLK flag
	
}
/********************************************************************/
void MCO_Init(void){
	
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;				// Clock GPIOA
	
	GPIOA->MODER &= ~GPIO_MODER_MODE8;
	GPIOA->MODER |= GPIO_MODER_MODE8_1;
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED8;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT8;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD8;
	GPIOA->AFR[1] = 0x00000000;
	
	RCC->CFGR |=  RCC_CFGR_MCOPRE_1;				// Divider - 4
	RCC->CFGR |=  RCC_CFGR_MCOSEL_0;				// SYSCLK
	
}
/********************************************************************/

