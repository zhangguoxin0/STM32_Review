#include "bsp_led.h"

void LED0_Init(void)
{
    // 开启时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    // PA0工作模式
    GPIOA->CRL |= GPIO_CRL_MODE0;
    GPIOA->CRL &= ~GPIO_CRL_CNF0;

    LED0_OFF();
}

void LED0_ON(void)
{
    GPIOA->ODR &= ~GPIO_ODR_ODR0;
}

void LED0_OFF(void)
{
    GPIOA->ODR |= GPIO_ODR_ODR0;
}

void LED0_Toggle(void)
{
    if ((GPIOA->IDR & GPIO_IDR_IDR0) == 0)
    {
        LED0_OFF();
    }
    else
    {
        LED0_ON();
    }
}
