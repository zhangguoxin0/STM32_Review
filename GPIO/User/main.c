#include "stm32f10x.h"

#if 0
#define RCC_APB2_IOPA *(unsigned int *)(0x40021000 + 0x18)
#define GPIOA_CRL *(unsigned int *)(0x40010800 + 0x00)
#define GPIOA_ODR *(unsigned int *)(0x40010800 + 0x0C)

int main(void)
{
    // 开启GPIOA的时钟
    RCC_APB2_IOPA |= (1 << 2);
    // 配置PA0工作模式为推挽输出
    GPIOA_CRL &= ~(3 << (2 * 0)); // 清空CRL寄存器MODE0
    GPIOA_CRL |= (3 << (2 * 0));  // 将CRL寄存器MODE0设置为11 => 输出模式,最大速度50MHz
    GPIOA_CRL &= ~(3 << (2 * 1)); // 清空CRL寄存器CNF0
    GPIOA_CRL &= ~(3 << (2 * 1)); // 将CRL寄存器CNF0设置为00 => 推挽输出
    // PA0输出低电平
    GPIOA_ODR &= ~(1 << 0); // 将ODR寄存器的ODR1置0
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

/* 构建库函数雏形，加深对库函数的理解 */
#if 0
#include <stdint.h>
#include "Delay.h"

typedef struct
{
    uint32_t CRL;
    uint32_t CRH;
    uint32_t IDR;
    uint32_t ODR;
    uint16_t BSRRL;
    uint16_t BSRRH;
    uint32_t BRR;
    uint32_t LCKR;
} MY_GPIO_TypeDef;

#define MY_RCC_BASE (unsigned int)0x40021000
#define RCC_APB2ENR *(unsigned int *)(MY_RCC_BASE + 0x18)

#define MY_GPIOA_BASE (unsigned int)0x40010800
#define MY_GPIOA ((MY_GPIO_TypeDef *)MY_GPIOA_BASE)

int main(void)
{
    // 开启GPIOA时钟
    RCC_APB2ENR |= (1 << 2);

    // 配置PA8工作模式为开漏输出，速度为高速模式
    MY_GPIOA->CRH &= ~(3 << 0);
    MY_GPIOA->CRH |= (3 << 0);
    MY_GPIOA->CRH &= ~(3 << 1);
    MY_GPIOA->CRH |= (1 << 1);
    // 将PA8引脚电平拉低
    MY_GPIOA->ODR &= ~(1 << 8);

    // 配置PA1工作模式为推挽输出
    MY_GPIOA->CRL &= ~(3 << (2 * 2)); 
    MY_GPIOA->CRL |= (3 << (2 * 2));  
    MY_GPIOA->CRL &= ~(3 << (2 * 3)); 
    MY_GPIOA->CRL &= ~(3 << (2 * 3)); 
    // PA1输出低电平
    MY_GPIOA->BSRRL |= (1 << 17);

    while (1)
    {
    }
}
#endif

/* 封装端口复位/置位函数 */
#if 0
#include <stdint.h>
#include "Delay.h"

typedef struct
{
    uint32_t CRL;
    uint32_t CRH;
    uint32_t IDR;
    uint32_t ODR;
    uint32_t BSRR;
    uint32_t BRR;
    uint32_t LCKR;
} MY_GPIO_TypeDef;

#define MY_RCC_BASE (unsigned int)0x40021000
#define RCC_APB2ENR *(unsigned int *)(MY_RCC_BASE + 0x18)

#define MY_GPIOA_BASE (unsigned int)0x40010800
#define MY_GPIOA ((MY_GPIO_TypeDef *)MY_GPIOA_BASE)
#define MY_GPIO_PIN8 (1 << 8)

void MY_GPIO_SetBits(MY_GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->BSRR |= GPIO_Pin;
}

void MY_GPIO_ReSetBit(MY_GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->BSRR |= (GPIO_Pin << 16);
}

int main(void)
{
    // 开启GPIOA时钟
    RCC_APB2ENR |= (1 << 2);

    // 配置PA8工作模式为开漏输出，速度为高速模式
    MY_GPIOA->CRH &= ~(3 << 0);
    MY_GPIOA->CRH |= (3 << 0);
    MY_GPIOA->CRH &= ~(3 << 1);
    MY_GPIOA->CRH |= (1 << 1);

    while (1)
    {
        // 将PA8引脚电平拉低
        MY_GPIO_ReSetBit(MY_GPIOA, MY_GPIO_PIN8);
        Delay_ms(1000);
        // 将PA8引脚电平拉高
        MY_GPIO_SetBits(MY_GPIOA, MY_GPIO_PIN8);
        Delay_ms(1000);
    }
}
#endif

/* 固件库实现GPIO输出 */
#if 0
#include "bsp_led.h"
#include "Delay.h"

int main(void)
{
    LED_Init();

    while(1)
    {
        LED0_Toggle();
        Delay_ms(500);
    }
}
#endif
