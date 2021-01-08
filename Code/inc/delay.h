#pragma once

#include "stm32g0xx.h"

#define F_CPU 64000000UL
#define TimerTick		F_CPU/1000

/****************************************************************************/

void Delay_Init(void);
void Delay_ms(uint32_t dlyTicks);
void Delay_us(uint32_t us);



