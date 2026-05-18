#include "bsp_led.h"
#include "bsp_key.h"
#include "Delay.h"

void KEY1_Init(void)
{
    // 1.开启GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    // 2.定义GPIO初始化结构体
    GPIO_InitTypeDef GPIO_InitStruct;
    // 3.配置GPIO初始化结构体成员
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    // 4.调用GPIO初始化函数，把配置好的结构体成员的参数写入寄存器
    GPIO_Init(GPIOF, &GPIO_InitStruct);
}

/**
 * @brief 按键扫描
 * 
 * @return uint8_t 0：按键未被按下 1：按键被按下
 */
uint8_t KEY1_Scan(void)
{
    // 按键被按下
    if (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8) == 0)
    {
        // 消抖
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8) == 0)
        {
            // 等待按键弹起
        }
        return 1;
    }
    return 0;
}
