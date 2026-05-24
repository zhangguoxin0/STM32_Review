#include "stm32f10x.h"
#include "Delay.h"
#include "usart.h"
#include "adc.h"

int main(void)
{
    USART1_Init();
    ADC1_Init();

    printf("\n-----------------------------------\n");

    // 开启ADC1转换
    ADC1_Start();

    while (1)
    {
        // ADC1转换结果验证
        printf("V = %.2f\n", ADC1_ReadData());
        Delay_ms(1000);
    }
}
