#ifndef __I2C_H__
#define __I2C_H__

#include "stm32f10x.h"

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_ACK(void);
void I2C_NACK(void);
uint8_t I2C_WaitACK(void);
void I2C_SendByte(uint8_t byte);
uint8_t I2C_ReadByte(void);

#endif /* __I2C_H__ */
