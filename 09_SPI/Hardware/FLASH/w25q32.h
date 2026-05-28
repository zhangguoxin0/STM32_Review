#ifndef __W25Q32_H__
#define __W25Q32_H__

#include "stm32f10x.h"

void W25Q32_Init(void);
void W25Q32_ReadID(uint8_t *mid, uint16_t *did);
void W25Q32_SectorErase(uint8_t block, uint8_t sector);
void W25Q32_PageWrie(uint8_t block, uint8_t sector, uint8_t page, uint8_t *data, uint16_t len);
void W25Q32_Read(uint8_t block, uint8_t sector, uint8_t page, uint8_t innerAddr, uint8_t *data, uint16_t len);

#endif /* __W25Q32_H__ */
