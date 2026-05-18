#include "bsp_led.h"

void LED0_Init(void)
{
    // 1.开启GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // 2.定义GPIO初始化结构体
    GPIO_InitTypeDef GPIO_InitStruct;
    // 3.配置GPIO初始化结构体成员
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    // 4.调用GPIO初始化函数，把配置好的结构体成员的参数写入寄存器
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    LED0_OFF();
}

void LED0_ON(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}

void LED0_OFF(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

void LED0_Toggle(void)
{
    if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0) == 0)
    {
        LED0_OFF();
    }
    else
    {
        LED0_ON();
    }
}
