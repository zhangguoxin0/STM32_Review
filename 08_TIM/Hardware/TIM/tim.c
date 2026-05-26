#include "tim.h"
#include "bsp_led.h"

// #define USE_TIM_BASIC 1 // 使用基本定时器
#define USE_TIM_GENERAL 1 // 使用通用定时器

#ifdef USE_TIM_BASIC
void TIM6_Init(void)
{
    // 1.开启时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    // 2.设置预分频值 7199，表示7200分频，得到10000Hz（100us）
    TIM6->PSC = 7199;
    // 3.设置自动重装载值 9999，表示 10000 次计数，得到 1s
    TIM6->ARR = 9999;
    // 4.打开更新中断使能
    TIM6->DIER |= TIM_DIER_UIE;
    // 5.NVIC配置
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM6_IRQn, 2);
    NVIC_EnableIRQ(TIM6_IRQn);
    // 6.使能计数器
    TIM6->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief 中断服务程序
 *
 */
void TIM6_IRQHandler(void)
{
    // 清除中断标志位
    TIM6->SR &= ~TIM_SR_UIF;

    // 需要执行的逻辑
    LED0_Toggle();
}
#endif

#ifdef USE_TIM_GENERAL

void TIM5_Init(void)
{
    // 1. 开启时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    RCC->APB1ENR |= RCC_APB2ENR_IOPAEN;
    // 2.配置GPIO工作模式：PA1 -> 复用推挽输出
    GPIOA->CRL |= GPIO_CRL_MODE1;
    GPIOA->CRL |= GPIO_CRL_CNF1_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF1_0;
    
    /* 定时器相关配置 */
    // 1.设置预分频值 7199，表示7200分频，得到10000Hz（100us）
    TIM5->PSC = 7199;
    // 2.设置自动重装载值 99，表示 10000 次计数，得到10ms
    TIM5->ARR = 99;
    // 3.设置计数方向 -> 向上计数
    TIM5->CR1 &= ~TIM_CR1_DIR;
    // 4.设置通道2的CCR值
    TIM5->CCR2 = 50;
    // 5.设置通道2为输出模式
    TIM5->CCMR1 &= ~TIM_CCMR1_CC2S;
    // 6.配置通道2为PWM1模式
    TIM5->CCMR1 |= TIM_CCMR1_OC2M_2;
    TIM5->CCMR1 |= TIM_CCMR1_OC2M_1;
    TIM5->CCMR1 &= ~TIM_CCMR1_OC2M_0;
    // 7.使能输出通道
    TIM5->CCER |= TIM_CCER_CC2E;
}

/**
 * @brief 开启定时器计数
 * 
 */
void TIM5_Start(void)
{
    TIM5->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief 关闭定时器计数
 * 
 */
void TIM5_Stop(void)
{
    TIM5->CR1 &= ~TIM_CR1_CEN;
}

/**
 * @brief 通过设置CCR的值来设置占空比
 * 
 */
void TIM5_SetDuty(uint8_t dutyCycle)
{
    TIM5->CCR2 = dutyCycle;
}

#endif
