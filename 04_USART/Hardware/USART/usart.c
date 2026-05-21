#include "usart.h"

void USART1_Init(void)
{
    // 1.开启时钟

    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // USART1使用PA8和PA9，这里需要打开GPIOA的时钟
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // 打开USART1时钟

    // 2.设置GPIO工作模式
    // PA9 -> USART1_TX -> 配置为复用推挽输出
    GPIOA->CRH |= GPIO_CRH_MODE9;   // PA9输出模式，最大速度50MHz
    GPIOA->CRH |= GPIO_CRH_CNF9_1;  // 10：复用功能推挽输出模式
    GPIOA->CRH &= ~GPIO_CRH_CNF9_0; // 10：复用功能推挽输出模式
    // PA10 -> USART1_RX -> 配置为浮空输入
    GPIOA->CRH &= ~GPIO_CRH_MODE10;   // PA10输入模式
    GPIOA->CRH &= ~GPIO_CRH_MODE10_1; // 01：浮空输入模式
    GPIOA->CRH |= GPIO_CRH_MODE10_0;  // 01：浮空输入模式

    // 3.串口配置
    // 3.1.配置波特率
    USART1->BRR = 0x271; // 配置波特率为115200，关于波特率的配置可参考手册 “分数波特率的产生”
    // 3.2.配置收发模块使能
    USART1->CR1 |= USART_CR1_UE; // 置位UE位来激活USART
    USART1->CR1 |= USART_CR1_TE; // 置位TE为来激活USART发送
    USART1->CR1 |= USART_CR1_RE; // 置位RE为来激活USART接收
    // 3.3.其他配置，一般使用默认即可，这里将配置为默认状态
    USART1->CR1 &= ~USART_CR1_M;    // 8位有效数据
    USART1->CR1 &= ~USART_CR1_PCE;  // 无校验位
    USART1->CR2 &= ~USART_CR2_STOP; // 00：1个停止位
}

/**
 * @brief USART发送一个字节
 *
 * @param ch 需要发送的数据
 */
void USART1_SendChar(uint8_t ch)
{
    // 判断SR里TXE是否为1，表示发送缓冲区为空
    while ((USART1->SR & USART_SR_TXE) == 0)
    {
    }
    // TXE位为1 -> 跳出循环 -> 表示可以向发送缓冲区写入数据

    // 向DR写入要发送的数据
    USART1->DR = ch;

    // 等待发送完成 -> TC置1，防止连续发送导致数据覆盖
    while ((USART1->SR & USART_SR_TC) == 0)
    {
    }
    // 一个字节发送完成，可以进行下一个字节的发送
}

/**
 * @brief USART接收一个字节
 *
 * @return uint8_t
 */
uint8_t USART1_ReceiveChar(void)
{
    // 判断SR里RXNE是否为1，表示接收缓冲区非空
    while (USART1->SR & USART_SR_RXNE == 0)
    {
    }
    // RXEN位为0 -> 跳出循环 -> 表示接收缓冲区有数据，可以进行读取

    // 读取已经接收到的数据
    return USART1->DR;
}
