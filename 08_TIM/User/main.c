#include "stm32f10x.h"
#include "Systick.h"
#include "usart.h"
#include "tim.h"
#include "bsp_led.h"

int main(void)
{
    USART1_Init();
    TIM5_Init();
    TIM4_Init();

    printf("\n-----------------------------------\n");

    TIM5_Start();
    TIM4_Start();

    while (1)
    {
        printf("T = %.2f ms,f = %.2f Hz\n", TIM4_GetPWMCycle(), TIM4_GetPWMFreq());
        Delay_ms(1000);
    }
}
