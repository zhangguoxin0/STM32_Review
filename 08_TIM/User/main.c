#include "stm32f10x.h"
#include "Systick.h"
#include "usart.h"
#include "tim.h"
#include "bsp_led.h"

int main(void)
{
    uint8_t dutyCycle = 0; // 占空比
    uint8_t dir = 0;       // 计数方向 -> 0：增大 1：减小

    USART1_Init();
    TIM5_Init();

    printf("\n-----------------------------------\n");

    TIM5_Start();

    while (1)
    {
        // PWM实现呼吸灯效果
        if (dir == 0)
        {
            dutyCycle++;
            if (dutyCycle >= 99)
            {
                dir = 1;
            }
        }
        else
        {
            dutyCycle--;
            if (dutyCycle <= 0)
            {
                dir = 0;
            }
        }

        TIM5_SetDuty(dutyCycle);
        Delay_ms(20);
    }
}
