#include "stm32f10x.h"
#include "usart.h"
#include "Delay.h"

int main(void)
{
    uint8_t r_buff[10];
    uint8_t r_size;

    USART1_Init();

    // 发送字符串测试
    USART1_SendString("Hello",5);

    while (1)
    {
        // 接收字符串测试
        USART1_ReceiveString(r_buff, &r_size);
        USART1_SendString(r_buff, r_size);
    }
}
