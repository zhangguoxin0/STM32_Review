#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f10x.h"

void SPI_Init(void);
uint8_t SPI_SwapByte(uint8_t t_byte);

#endif /* __SPI_H__ */
