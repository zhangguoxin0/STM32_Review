#include "NVIC.h"

/**
 * @brief 配置NVIC
 *
 */
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;      // 中断源
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;        // 子优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;           // 使能中断
    NVIC_Init(&NVIC_InitStruct);
}

/**
 * @brief 配置EXTI
 *
 */
void EXTI_Configuration(void)
{
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOF, GPIO_PinSource10);
    // 初始化EXIT所使用的GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOF, &GPIO_InitStruct);
    // 初始化EXTI
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN, ENABLE);
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PF;
    EXTI_InitTypeDef EXIT_InitStruct;
    EXIT_InitStruct.EXTI_Line = EXTI_Line10;            // 启用EXTI10
    EXIT_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;    // 设置模式为外部中断
    EXIT_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; // 上升沿触发
    EXIT_InitStruct.EXTI_LineCmd = ENABLE;              // 使能中断
    EXTI_Init(&EXIT_InitStruct);
    // 编写中断服务程序,详见stm32f10x_it.c
}

void IRQ_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 中断优先级分组
    EXTI_Configuration();
    NVIC_Configuration();
}
