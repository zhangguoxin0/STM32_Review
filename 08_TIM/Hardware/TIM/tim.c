#include "tim.h"
#include "bsp_led.h"

#define USE_TIM_BASIC 1 // 使用基本定时器
// #define USE_TIM_GENERAL 1 // 使用通用定时器

#ifdef USE_TIM_BASIC
void TIM6_Init(void)
{
    // 1. 开启时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    // 2. 设置预分频值 7199，表示7200分频，得到10000Hz（100us）
    TIM6->PSC = 7199;
    // 3. 设置自动重装载值 9999，表示 10000 次计数，得到 1s
    TIM6->ARR = 9999;
    // 4. 打开更新中断使能
    TIM6->DIER |= TIM_DIER_UIE;
    // 5. NVIC配置
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM6_IRQn, 2);
    NVIC_EnableIRQ(TIM6_IRQn);
    // 6. 使能计数器
    TIM6->CR1 = TIM_CR1_CEN;
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

#endif
