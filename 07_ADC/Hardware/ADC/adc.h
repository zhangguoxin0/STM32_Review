#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f10x.h"

void ADC1_Init(void);
void ADC1_Start(void);
double ADC1_ReadData(void);

#endif /* __ADC_H__ */
