#include "stm32f10x.h"

int main(void)
{
    // 开启GPIOA的时钟
    *(unsigned int *)(0x40021000 + 0x18) |= (1 << 2);
    // 配置PA1工作模式为推挽输出
    *(unsigned int *)(0x40010800 + 0x00) &= ~(3 << (2 * 0)); // 清空CRL寄存器MODE0
    *(unsigned int *)(0x40010800 + 0x00) |= (3 << (2 * 0));  // 将CRL寄存器MODE0设置为11 => 输出模式,最大速度50MHz
    *(unsigned int *)(0x40010800 + 0x00) &= ~(3 << (2 * 1)); // 清空CRL寄存器CNF0
    *(unsigned int *)(0x40010800 + 0x00) &= ~(3 << (2 * 1)); // 将CRL寄存器CNF0设置为00 => 推挽输出
    // PA1输出低电平
    *(unsigned int *)(0x40010800 + 0x0C) &= ~(1 << 1);
    while (1)
    {
    }
}
