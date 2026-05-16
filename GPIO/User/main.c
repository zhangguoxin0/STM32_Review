#include "stm32f10x.h"

#if 0
#define RCC_APB2_IOPA *(unsigned int *)(0x40021000 + 0x18)
#define GPIOA_CRL *(unsigned int *)(0x40010800 + 0x00)
#define GPIOA_ODR *(unsigned int *)(0x40010800 + 0x0C)

int main(void)
{
    // 开启GPIOA的时钟
    RCC_APB2_IOPA |= (1 << 2);
    // 配置PA1工作模式为推挽输出
    GPIOA_CRL &= ~(3 << (2 * 0)); // 清空CRL寄存器MODE0
    GPIOA_CRL |= (3 << (2 * 0));  // 将CRL寄存器MODE0设置为11 => 输出模式,最大速度50MHz
    GPIOA_CRL &= ~(3 << (2 * 1)); // 清空CRL寄存器CNF0
    GPIOA_CRL &= ~(3 << (2 * 1)); // 将CRL寄存器CNF0设置为00 => 推挽输出
    // PA1输出低电平
    GPIOA_ODR &= ~(1 << 1); // 将ODR寄存器的ODR1置0
    while (1)
    {
    }
}
#endif

/* 不参考例程，自己对着手册敲一遍 */
#if 0
#define MY_RCC_BASE (unsigned int)0x40021000
#define MY_GPIOA_BASE (unsigned int)0x40010800

#define RCC_APB2ENR *(unsigned int *)(MY_RCC_BASE + 0x18)

#define GPIOA_CRL *(unsigned int *)(MY_GPIOA_BASE + 0x00)
#define GPIOA_CRH *(unsigned int *)(MY_GPIOA_BASE + 0x04)
#define GPIOA_IDR *(unsigned int *)(MY_GPIOA_BASE + 0x08)
#define GPIOA_ODR *(unsigned int *)(MY_GPIOA_BASE + 0x0C)
#define GPIOA_BSRR *(unsigned int *)(MY_GPIOA_BASE + 0x10)
#define GPIOA_BRR *(unsigned int *)(MY_GPIOA_BASE + 0x14)
#define GPIOA_LCKR *(unsigned int *)(MY_GPIOA_BASE + 0x18)

int main(void)
{
    // 开启GPIOA时钟
    RCC_APB2ENR |= (1 << 2);
    // 配置PA8工作模式为开漏输出，速度为高速模式
    GPIOA_CRH &= ~(3 << 0);
    GPIOA_CRH |= (3 << 0);
    GPIOA_CRH &= ~(3 << 1);
    GPIOA_CRH |= (1 << 1);
    // 将PA8引脚电平拉低
    GPIOA_BSRR &= ~(1 << 24);

    while (1)
    {
    }
}
#endif


