#ifndef __TIM_H__
#define __TIM_H__

#include "stm32f10x.h"

void TIM6_Init(void);

void TIM5_Init(void);
void TIM5_Start(void);
void TIM5_Stop(void);
void TIM5_SetDuty(uint8_t dutyCycle);

#endif /* __TIM_H__ */
