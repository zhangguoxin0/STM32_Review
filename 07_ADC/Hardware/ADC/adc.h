#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f10x.h"

void ADC1_Init(void);
void ADC1_DMA_Init(void);
void ADC1_Start(void);
void ADC1_Start_DMA(uint32_t destAddr, uint16_t dataLen);
double ADC1_ReadData(void);

#endif /* __ADC_H__ */
