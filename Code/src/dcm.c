#include "dcm.h"

/***************************************************************/
void TIM3_Init(void){
	
	// GPIO INIT
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;											// Clock GPIOA
	RCC->APBENR1 |= RCC_APBENR1_TIM3EN;								      // Clock TIM3
	
	GPIOA->AFR[0] |= (1<<24);																// PA6 TIM3_CH1	AF1 
	GPIOA->AFR[0] |= (1<<28);																// PA7 TIM3_CH2	AF1
  //GPIOB->AFR[0] |= (1<<4);															// PB1 TIM3_CH4	AF1	
	
	// PA6 PWM OUT CH1 **********************************************
	GPIOA->MODER &= ~GPIO_MODER_MODE6;
	GPIOA->MODER |=   GPIO_MODER_MODE6_1;							   // AF OutPut Push_Pull
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT6;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED6;				       // Full Speed
	
	// PA7 PWM OUT CH2 **********************************************
	GPIOA->MODER &= ~GPIO_MODER_MODE7;
	GPIOA->MODER |=   GPIO_MODER_MODE7_1;							   // AF OutPut Push_Pull
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT7;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED7;				       // Full Speed
	
	TIM3->PSC = 2 - 1;
	TIM3->ARR = 1000;
	
	TIM3->CCR1 = 0;
	TIM3->CCR2 = 0;
	//TIM3->CCR4 = 0;
	
	// CH1 **********************************************
	TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	TIM3->CCER |= TIM_CCER_CC1E;
	TIM3->CCER &= ~TIM_CCER_CC1P;
	
	// CH2 **********************************************
	TIM3->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
	TIM3->CCER |= TIM_CCER_CC2E;
	TIM3->CCER &= ~TIM_CCER_CC2P;
	
	// CH4 **********************************************
//	TIM3->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2;
//	TIM3->CCER |= TIM_CCER_CC4E;
//	TIM3->CCER &= ~TIM_CCER_CC4P;
	
	TIM3->CR1 &= ~TIM_CR1_DIR;
	TIM3->CR1 |= TIM_CR1_CEN;
	
}
/***************************************************************/

/***************************************************************/
void DCM_Init(void){
	
	TIM3_Init();
	
}
/***************************************************************/

