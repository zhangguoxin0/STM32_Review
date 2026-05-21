#include "stm32f10x.h"
#include "usart.h"
#include "Delay.h"

uint8_t r_buff[100] = {0}; // 接收缓冲区
uint8_t r_size = 0;        // 接收数据大小

volatile uint8_t r_flash = 0; // 接收完成标志位

int main(void)
{
    USART1_Init();

    // 发送字符串测试
    USART1_SendString("Hello", 5);

    while (1)
    {
        if (r_flash == 1)
        {
            r_flash = 0;                       // 清除标志位
            USART1_SendString(r_buff, r_size); // 检验接收是否成功
            r_size = 0;
        }
    }
}
