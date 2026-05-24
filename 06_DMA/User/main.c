#include "stm32f10x.h"
#include "Delay.h"
#include "usart.h"
#include "dma.h"

uint8_t IRQflag_DMA1Channel1_TransmitFinished = 0; // DMA1通道1传输完成标志位

// 定义全局常量，放置在ROM中，作为数据源
const uint8_t src[] = {10, 20, 30, 40};

// 定义变量数组，放置在RAM中，用来存储接收到的数据
uint8_t dest[4] = {1, 1, 1, 1};

int main(void)
{
    USART1_Init();
    DMA1_Init();

    printf("\n-----------------------------------\n");

    // 验证DMA传输成功
    printf("传输前: dest[0] = %d,dest[1] = %d,dest[2] = %d,dest[3] = %d\n", dest[0], dest[1], dest[2], dest[3]);

    DMA1_Transmit((uint32_t)src, (uint32_t)dest, 4);

    while (1)
    {
        if (IRQflag_DMA1Channel1_TransmitFinished == 1)
        {
            printf("传输后: dest[0] = %d,dest[1] = %d,dest[2] = %d,dest[3] = %d\n", dest[0], dest[1], dest[2], dest[3]);

            IRQflag_DMA1Channel1_TransmitFinished = 0;
        }
    }
}
