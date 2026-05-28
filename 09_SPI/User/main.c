#include "stm32f10x.h"
#include "Systick.h"
#include "usart.h"
#include "w25q32.h"
#include <string.h>

uint8_t mid = 0;
uint16_t did = 0;

int main(void)
{
    uint8_t *data = "∫∫◊÷";
    uint8_t buff[10] = {0};

    USART1_Init();
    W25Q32_Init();

    printf("\n-----------------------------------\n");

    // ªÒ»°ID≤‚ ‘
    W25Q32_ReadID(&mid, &did);
    printf("mid = %#x did = %#x\n", mid, did);

    // –¥»Î∂¡»°≤‚ ‘
    W25Q32_SectorErase(0, 0); // ≤¡≥˝…»«¯
    W25Q32_PageWrie(0, 0, 0, data, strlen((const char *)data));
    W25Q32_Read(0, 0, 0, 0, buff, strlen((const char *)data));

    printf("buff = %s\n", buff);

    while (1)
    {
    }
}
