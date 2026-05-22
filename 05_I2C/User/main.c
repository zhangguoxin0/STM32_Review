#include "stm32f10x.h"
#include "Delay.h"
#include "usart.h"
#include "m24c02.h"

int main(void)
{
    uint8_t byte1 = 0;
    uint8_t byte2 = 0;
    uint8_t byte3 = 0;
    uint8_t buffer[10];

    USART1_Init();
    M24C02_Init();

    printf("\n-----------------------------------\n");

    // 写入测试
    M24C02_WriteByte(0x00, 'a');
    M24C02_WriteByte(0x01, 'b');
    M24C02_WriteByte(0x02, 'c');
    M24C02_WriteBytes(0x03, "123456", 6);

    // 读取测试
    byte1 = M24C02_ReadByte(0x00);
    byte2 = M24C02_ReadByte(0x01);
    byte3 = M24C02_ReadByte(0x02);
    M24C02_ReadBytes(0x03, buffer, 6);

    printf("byte1 = %c\n", byte1);
    printf("byte2 = %c\n", byte2);
    printf("byte3 = %c\n", byte3);
    printf("buffer = %s", buffer);

    while (1)
    {
    }
}
