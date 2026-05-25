#include "stm32f10x.h"
#include "Delay.h"
#include "usart.h"
#include "adc.h"

// 定义数组，保存DMA搬运来的ADC1数据
uint16_t adc1_data[2] = {0};

int main(void)
{
    USART1_Init();
    ADC1_Init();
    ADC1_DMA_Init();

    printf("\n-----------------------------------\n");

    // 开启ADC1转换
    // ADC1_Start();
    ADC1_Start_DMA((uint32_t)adc1_data,2);

    while (1)
    {
        // ADC1转换结果验证
        // printf("V = %.2f\n", ADC1_ReadData());

        // ADC1 + DMA转换结果验证
        printf("ADC1_CH10:%d ADC2_CH12:%d\n", adc1_data[0], adc1_data[1]);

        Delay_ms(1000);
    }
}
