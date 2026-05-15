#include "stm32f10x.h"

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
