#include "tim.h"
#include "bsp_led.h"

// #define USE_TIM_BASIC 1 // 使用基本定时器产生定时中断
#define USE_TIM_GENERAL_OC 1 // 使用通用定时器输入捕获
#define USE_TIM_GENERAL_IC 1 // // 使用通用定时器输出比较

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

#if USE_TIM_GENERAL_OC
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

#ifdef USE_TIM_GENERAL_IC
void TIM4_Init(void)
{
    // 1. 开启时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // 2.配置GPIO工作模式：PAB -> 浮空输入
    GPIOA->CRL &= ~GPIO_CRL_MODE6;
    GPIOA->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOA->CRL |= GPIO_CRL_CNF6_0;

    /* 定时器相关配置 */
    // 1.设置预分频值 71，表示72分频，得到1MHz（1us）
    TIM4->PSC = 71;
    // 2.设置自动重装载值 65535，防止溢出
    TIM4->ARR = 65535;
    // 3.设置计数方向 -> 向上计数
    TIM4->CR1 &= ~TIM_CR1_DIR;
    // 4.输入通道部分
    TIM4->CR2 &= ~TIM_CR2_TI1S;       // TI1输入选择：不与TI2,TI3异或
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;   // 输入滤波器：不进行滤波
    TIM4->CCER &= ~TIM_CCER_CC1P;     // 配置极性：上升沿触发
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1; // 选择通道1的输入映射位TI1
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;  // 选择通道1的输入映射位TI1
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1PSC; // 预分频器：不分频
    // 5.通道1输入捕获使能
    TIM4->CCER |= TIM_CCER_CC1E;
    // 6.开启输入捕获中断使能
    TIM4->DIER |= TIM_DIER_CC1IE;

    /* NVIC配置 */
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM4_IRQn, 2);
    NVIC_EnableIRQ(TIM4_IRQn);
}

void TIM4_Start(void)
{
    TIM4->CR1 |= TIM_CR1_CEN;
}

void TIM4_Stop(void)
{
    TIM4->CR1 &= ~TIM_CR1_CEN;
}

/**
 * @brief 获取输入信号周期
 *
 * @return double
 */
double TIM4_GetPWMCycle(void)
{
    return TIM4->CCR1 / 1000.0;
}

/**
 * @brief 获取输入信号占空比
 *
 * @return double
 */
double TIM4_GetPWMFreq(void)
{
    return 1000000.0 / TIM4->CCR1;
}

/**
 * @brief 定时器4中断服务程序
 *
 */
void TIM4_IRQHandler(void)
{
    // 判断是定时器4的通道1产生的中断
    if (TIM4->SR & TIM_SR_CC1IF)
    {
        // 清除中断标志位
        TIM4->SR &= ~TIM_SR_CC1IF;

        TIM4->CNT = 0;
    }
}

#endif
