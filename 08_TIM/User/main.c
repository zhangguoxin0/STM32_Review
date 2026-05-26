#include "stm32f10x.h"
#include "Systick.h"
#include "usart.h"
#include "tim.h"
#include "bsp_led.h"

int main(void)
{
    USART1_Init();
    LED0_Init();
    TIM6_Init();

    LED0_ON();

    printf("\n-----------------------------------\n");

    while (1)
    {
    }
}
