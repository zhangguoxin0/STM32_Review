#ifndef __DMA_H__
#define __DMA_H__

#include "stm32f10x.h"

extern uint8_t IRQflag_DMA1Channel1_TransmitFinished;

void DMA1_Init(void);
void DMA1_Transmit(uint32_t srcAddr,uint32_t destAddr,uint16_t dataLen);

#endif /* __DMA_H__ */
