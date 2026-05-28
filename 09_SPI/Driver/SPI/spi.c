#include "spi.h"
#include "Systick.h"

#define USE_SOFT_SPI 1

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
        MOSI_Set(t_byte & 0x80);  // 准备要发送的1位数据
        r_byte <<= 1;
        SCK_Set(1);               // 将数据发送到MOSI
        SPI_DELAY;
        r_byte |= MISO_Read();    // 从MISO采集数据
        SCK_Set(0);               // 完成1位交换
        SPI_DELAY;
        t_byte <<= 1;
    }

    return r_byte;
}
#endif
