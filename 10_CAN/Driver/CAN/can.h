#ifndef __CAN_H__
#define __CAN_H__

#include "stm32f10x.h"

// 定义结构体，保存接收到的报文信息
typedef struct 
{
    uint16_t stdID;
    uint8_t data[8];
    uint8_t len;
}RxMag;


void CAN_Init(void);
void CAN_SendMsg(uint16_t stdID, uint8_t *data, uint8_t len);
void CAN_ReceiveMsg(RxMag rxMsg[], uint8_t *msgcount);

#endif /* __CAN_H__ */
