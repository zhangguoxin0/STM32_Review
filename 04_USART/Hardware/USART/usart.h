#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"

void USART1_Init(void);

void USART1_SendChar(uint8_t ch);
uint8_t USART1_ReceiveChar(void);
void USART1_SendString(uint8_t *str, uint8_t size);
void USART1_ReceiveString(uint8_t buff[], uint8_t *size);

#endif /* __USART_H__ */
