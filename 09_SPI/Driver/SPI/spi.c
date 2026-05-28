#include "spi.h"
#include "Systick.h"

// #define USE_SOFT_SPI 1  // 使用软件SPI

#ifdef USE_SOFT_SPI
#define SPI_DELAY Delay_us(5)

void SCK_Set(uint8_t value)
{
    if (value == 0)
    {
        GPIOA->ODR &= ~GPIO_ODR_ODR5;
    }
    else
    {
        GPIOA->ODR |= GPIO_ODR_ODR5;
    }
}

void MOSI_Set(uint8_t value)
{
    if (value == 0)
    {
        GPIOA->ODR &= ~GPIO_ODR_ODR7;
    }
    else
    {
        GPIOA->ODR |= GPIO_ODR_ODR7;
    }
}

uint8_t MISO_Read(void)
{
    if ((GPIOA->IDR & GPIO_IDR_IDR6) == 0)
    {
        return 0;
    }
    return 1;
}

void SPI_Init(void)
{
    // 开启时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    /* 配置GPIO */
    // PA5 -> SCK -> 推挽输出
    GPIOA->CRL |= GPIO_CRL_MODE5;
    GPIOA->CRL &= ~GPIO_CRL_CNF5;
    // PA6 -> MISO -> 浮空输入
    GPIOA->CRL &= ~GPIO_CRL_MODE6;
    GPIOA->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOA->CRL |= GPIO_CRL_CNF6_0;
    // PA7 -> MOSI -> 推挽输出
    GPIOA->CRL |= GPIO_CRL_MODE7;
    GPIOA->CRL &= ~GPIO_CRL_CNF7;
    // 使用SPI的mode0，SCK默认为低电平
    SCK_Set(0);
}

/**
 * @brief SPI进行数据交换
 *
 * @param t_byte 需要发送的数据
 * @return uint8_t 接收到的数据
 */
uint8_t SPI_SwapByte(uint8_t t_byte)
{
    uint8_t i;
    uint8_t r_byte = 0;

    for (i = 0; i < 8; i++)
    {
        MOSI_Set(t_byte & 0x80); // 准备要发送的1位数据
        r_byte <<= 1;
        SCK_Set(1); // 将数据发送到MOSI
        SPI_DELAY;
        r_byte |= MISO_Read(); // 从MISO采集数据
        SCK_Set(0);            // 完成1位交换
        SPI_DELAY;
        t_byte <<= 1;
    }

    return r_byte;
}
#else
void SPI_Init(void)
{
    // 开启时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    /* 配置GPIO */
    // PA5 -> SCK -> 复用推挽输出
    GPIOA->CRL |= GPIO_CRL_MODE5;
    GPIOA->CRL |= GPIO_CRL_CNF5_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF5_0;
    // PA6 -> MISO -> 浮空输入
    GPIOA->CRL &= ~GPIO_CRL_MODE6;
    GPIOA->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOA->CRL |= GPIO_CRL_CNF6_0;
    // PA7 -> MOSI -> 复用推挽输出
    GPIOA->CRL |= GPIO_CRL_MODE7;
    GPIOA->CRL |= GPIO_CRL_CNF7_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF7_0;
    /* 配置SPI1 */
    SPI1->CR1 &= ~SPI_CR1_DFF;      // 数据帧格式：8位
    SPI1->CR1 |= SPI_CR1_SSM;       // 启用软件片选
    SPI1->CR1 |= SPI_CR1_SSI;       // 片选信号位拉高
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST; // 帧格式：高位先行
    SPI1->CR1 &= ~SPI_CR1_BR_2;     // 波特率控制：fpclk/4
    SPI1->CR1 &= ~SPI_CR1_BR_1;     // 波特率控制：fpclk/4
    SPI1->CR1 |= SPI_CR1_BR_0;      // 波特率控制：fpclk/4
    SPI1->CR1 |= SPI_CR1_MSTR;      // 配置位SPI主设备
    SPI1->CR1 &= ~SPI_CR1_CPOL;     // SPI模式0：空闲时为0
    SPI1->CR1 &= ~SPI_CR1_CPHA;     // SPI模式0：数据采样从第一个时钟边沿开始
    SPI1->CR1 |= SPI_CR1_SPE;       // 使能SPI
}

/**
 * @brief SPI1进行数据交换
 *
 * @param t_byte 需要发送的数据
 * @return uint8_t 接收到的数据
 */
uint8_t SPI_SwapByte(uint8_t t_byte)
{
    // 等待发送缓冲区为空
    while ((SPI1->SR & SPI_SR_TXE) == 0)
    {
    }
    SPI1->DR = t_byte; // 将要发送的数据写入到发送缓冲区
    // 等待接收缓冲区为非空
    while ((SPI1->SR & SPI_SR_RXNE) == 0)
    {
    }

    return (SPI1->DR & 0xFF);
}

/**
 * @brief 关闭SPI通讯
 *
 */
void SPI_Stop(void)
{
    // 1.等待RXNE = 1并接收最后一个数据：这一步在SPI_SwapByte已经实现
    // 2.等待TXE = 1
    while ((SPI1->SR & SPI_SR_TXE) == 0)
    {
    }
    // 3.等待BSY = 0
    while (SPI1->SR & SPI_SR_BSY)
    {
    }
    // 4.关闭SPI
    SPI1->CR1 &= ~SPI_CR1_SPE;
}

#endif
