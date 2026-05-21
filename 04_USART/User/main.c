#include "stm32f10x.h"
#include "usart.h"
#include "Delay.h"

int main(void)
{
    USART1_Init();

    // 发送测试
    USART1_SendChar('H');
    USART1_SendChar('e');
    USART1_SendChar('l');
    USART1_SendChar('l');
    USART1_SendChar('o');

    while (1)
    {
        // 接收测试
        uint8_t c = USART1_ReceiveChar();
        USART1_SendChar(c);
        Delay_ms(1000);
    }
}
