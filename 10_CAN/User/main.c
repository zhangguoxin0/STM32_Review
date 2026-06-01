#include "stm32f10x.h"
#include "Systick.h"
#include "usart.h"
#include "can.h"
#include <string.h>

int main(void)
{
    USART1_Init();
    CAN_Init();

    printf("\n-----------------------------------\n");

    // 发送报文
    uint16_t stdID = 0x066;
    uint8_t *data = "abcdefg";
    CAN_SendMsg(stdID, data, strlen((const char *)data));
    stdID = 0x066;
    data = "12345";
    CAN_SendMsg(stdID, data, strlen((const char *)data));
    stdID = 0x067;
    data = "zzz";
    CAN_SendMsg(stdID, data, strlen((const char *)data));

    // 接收报文
    RxMag rxMsg[3];
    uint8_t msgCount;
    CAN_ReceiveMsg(rxMsg, &msgCount);

    // 输出结果
    printf("报文接收完毕,报文个数位：%d\n", msgCount);
    for (uint8_t i = 0; i < msgCount; i++)
    {
        printf("stdID = %#X,len = %d,data = %.*s\n", rxMsg[i].stdID, rxMsg[i].len, rxMsg[i].len, rxMsg[i].data);
    }

    while (1)
    {
    }
}
