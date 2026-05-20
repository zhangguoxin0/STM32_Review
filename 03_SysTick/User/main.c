#include "stm32f10x.h"
#include "bsp_led.h"
#include "Delay.h"

int main(void)
{
    LED0_Init();

    // 设置系统定时器重装载值，通过计算可以得出定时时间
    // 默认时钟源选择位为AHB，可设置为AHB/8
    // 默认开启中断
    // 默认打开SysTick定时器使能位
    // SysTick_Config();
    // 可在void SysTick_Handler(void)中编写中断服务程序

    while (1)
    {
        LED0_ON();
        Delay_ms(500);
        LED0_OFF();
        Delay_ms(500);
    }
}
