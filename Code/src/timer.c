#include "timer.h"

/******************************************************/
void TIM16_Init(void){
	
	RCC->APBENR2 |= RCC_APBENR2_TIM16EN;

	TIM16->PSC = 6400-1;																	// div, frequency  10khz
	TIM16->ARR = 1000; 																		// count to 10khz/2000 - 5Hz
	TIM16->DIER |= TIM_DIER_UIE; 													// enable interrupt for timer
	TIM16->CR1 |= TIM_CR1_CEN; 														// start count
	NVIC_EnableIRQ(TIM16_IRQn);
}
/******************************************************/
//void TIM16_IRQHandler(void){

//	 TIM16->SR &= ~TIM_SR_UIF;
//   // User code
//}
/******************************************************/