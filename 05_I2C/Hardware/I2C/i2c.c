#include "i2c.h"
#include "Delay.h"

#ifdef USE_SOFT_IIC
/************************************** 底层操作函数 **************************************/
/**
 * @brief 设置SCL引脚电平
 *
 * @param value 0：SCL拉低 1：SCL拉高
 */
void SCL_SetValue(uint8_t value)
{
    if (value != 0)
    {
        GPIOB->ODR |= GPIO_ODR_ODR10;
    }
    else
    {
        GPIOB->ODR &= ~GPIO_ODR_ODR10;
    }
}

/**
 * @brief 设置SDA引脚电平
 *
 * @param value 0：SDA拉低 1：SDA拉高
 */
void SDA_SetValue(uint8_t value)
{
    if (value != 0)
    {
        GPIOB->ODR |= GPIO_ODR_ODR11;
    }
    else
    {
        GPIOB->ODR &= ~GPIO_ODR_ODR11;
    }
}

/**
 * @brief 读取SDA引脚电平
 *
 * @return uint8_t 0：低电平 1：高电平
 */
uint8_t SDA_ReadValue(void)
{
    if ((GPIOB->IDR & GPIO_IDR_IDR11) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/************************************** 外部使用的函数 **************************************/

void I2C_Init(void)
{
    // 1.开启时钟(这里使用软件I2C，所以不需要开启I2C时钟)
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // 开启GPIOB时钟(根据I2C软件模拟IO口需求设置)
    // 2.设置GPIO工作模式 -> 需要配置为开漏输出
    // SCL -> PB10
    GPIOB->CRH |= GPIO_CRH_MODE10;
    GPIOB->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOB->CRH |= GPIO_CRH_CNF10_0;
    // SDA -> PA11
    GPIOB->CRH |= GPIO_CRH_MODE11;
    GPIOB->CRH &= ~GPIO_CRH_CNF11_1;
    GPIOB->CRH |= GPIO_CRH_CNF11_0;

    SCL_SetValue(0); // 每一步操作后拉低SCL
    SDA_SetValue(1); // 每一步操作后释放SDA
    Delay_us(10);
}

/**
 * @brief I2C起始信号
 *
 */
void I2C_Start(void)
{
    SDA_SetValue(1); // SCL低电平期间将SDA拉高 -> 为起始信号做准备
    Delay_us(10);

    SCL_SetValue(1); // 先将SCL拉高
    Delay_us(10);    // SCL高电平保持一段时间
    SDA_SetValue(0); // 在SCL高电平期间将SDA拉低 -> 起始
    Delay_us(10);    // 保持一段时间

    SCL_SetValue(0); // SCL拉低
    SDA_SetValue(1); // 释放SDA
    Delay_us(10);
}

/**
 * @brief I2C停止信号
 *
 */
void I2C_Stop(void)
{
    SDA_SetValue(0); // SCL低电平期间将SDA拉低 -> 为终止信号做准备
    Delay_us(10);

    SCL_SetValue(1); // 先将SCL拉高
    Delay_us(10);    // 保持一段时间
    SDA_SetValue(1); // 在SCL高电平期间将SDA拉高 -> 终止
    Delay_us(10);    // 保持一段时间

    SCL_SetValue(0); // SCL拉低
    SDA_SetValue(1); // 释放SDA
    Delay_us(10);
}

/**
 * @brief 向从机发送应答信号
 *
 */
void I2C_ACK(void)
{
    SDA_SetValue(0); // SCL低电平期间准备应答信号
    Delay_us(10);    // 保持一段时间
    SCL_SetValue(1); // SCL置高，SDA的数据被发送
    Delay_us(10);    // 保持一段时间

    SCL_SetValue(0); // SCL拉低
    SDA_SetValue(1); // 释放SDA
    Delay_us(10);
}

/**
 * @brief 向从机发送非应答信号
 *
 */
void I2C_NACK(void)
{
    SDA_SetValue(1); // SCL低电平期间准备非应答信号
    Delay_us(10);    // 保持一段时间
    SCL_SetValue(1); // SCL置高，SDA的数据被发送
    Delay_us(10);    // 保持一段时间

    SCL_SetValue(0); // SCL拉低
    SDA_SetValue(1); // 释放SDA
    Delay_us(10);
}

/**
 * @brief 主机等待从机应答信号
 *
 * @return uint8_t 0：从机有应答 1：从机没有应答
 */
uint8_t I2C_WaitACK(void)
{
    uint8_t ack;

    SCL_SetValue(1); // SDA拉高 -> 开始采集SDA数据
    Delay_us(10);

    ack = SDA_ReadValue(); // 读取SDA的值
    Delay_us(10);

    SCL_SetValue(0); // SCL拉低
    SDA_SetValue(1); // 释放SDA
    Delay_us(10);

    return ack;
}

/**
 * @brief I2C发送一个字节数据
 *
 * @param byte 需要发送的数据
 */
void I2C_SendByte(uint8_t byte)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        // 1.SCL低电平期间准备SDA数据
        SDA_SetValue(byte & 0x80);
        Delay_us(10);
        // 2.SCL拉高发送SDA数据
        SCL_SetValue(1);
        Delay_us(10);
        // 3.SCL拉低
        SCL_SetValue(0);

        Delay_us(10);

        byte <<= 1;
    }
    SDA_SetValue(1); // 释放SDA
}

/**
 * @brief I2C接收一个字节的数据
 *
 * @return uint8_t 接收到的数据
 */
uint8_t I2C_ReadByte(void)
{
    uint8_t i;
    uint8_t byte = 0;

    for (i = 0; i < 8; i++)
    {
        byte <<= 1;
        // SCL拉高 -> 开始采集SDA数据
        SCL_SetValue(1);
        Delay_us(10);
        // 读取SDA的值
        byte |= SDA_ReadValue();
        Delay_us(10);
        // SCL拉低
        SCL_SetValue(0);
        Delay_us(10);
    }

    SDA_SetValue(1); // 释放SDA

    return byte;
}

#else

void I2C_Init(void)
{
    // 1.开启时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    // 2.GPIO工作模式配置：复用开漏输出
    // SCL -> PB10
    GPIOB->CRH |= GPIO_CRH_MODE10;
    GPIOB->CRH |= GPIO_CRH_CNF10;
    // SDA -> PA11
    GPIOB->CRH |= GPIO_CRH_MODE11;
    GPIOB->CRH |= GPIO_CRH_CNF11;

    // 3.配置I2C工作模式
    I2C2->CR1 &= ~I2C_CR1_SMBUS; // I2C模式
    I2C2->CCR &= ~I2C_CCR_FS;    // 标准模式
    I2C2->CR2 |= 36;             // 输入时钟频率36Mhz
    I2C2->CCR |= 180;            // 配置CCR:传输速率位100kHz，SCL高电平时间位5us，APB时钟频率位36MHz -> CCR = 5us/(1/36MHz)
    I2C2->CCR |= 37;             // 配置TRISE:SCL上升沿最大时钟周期数 + 1

    // 4.使能I2C外设
    I2C2->CR1 |= I2C_CR1_PE;
}

/**
 * @brief I2C起始信号
 *
 */
void I2C_Start(void)
{
    uint16_t timeout = 0xFFFF; // 超时时间

    // 产生起始条件
    I2C2->CR1 |= I2C_CR1_START;

    // 等待起始条件发送完成
    while (((I2C2->SR1 & I2C_SR1_SB) == 0) && timeout)
    {
        timeout--;
    }
}

/**
 * @brief I2C停止信号
 *
 */
void I2C_Stop(void)
{
    // 产生停止条件
    I2C2->CR1 |= I2C_CR1_STOP;
}

/**
 * @brief 向从机发送应答信号
 *
 */
void I2C_ACK(void)
{
    // 应答使能，在接收到一个字节后返回一个应答
    // 这里ACK位置1，表示会发送应答信号
    I2C2->CR1 |= I2C_CR1_ACK;
}

/**
 * @brief 向从机发送非应答信号
 *
 */
void I2C_NACK(void)
{
    // 应答使能，在接收到一个字节后返回一个应答
    // 这里ACK位置0，表示不会发送应答信号
    I2C2->CR1 &= ~I2C_CR1_ACK;
}

/**
 * @brief I2C发送地址
 *
 * @param byte 需要发送的地址
 */
void I2C_SendAddr(uint8_t addr)
{
    uint16_t timeout = 0xFFFF;

    // 将需要发送的地址填入数据寄存器
    I2C2->DR = addr;

    // 等待从机应答
    while (((I2C2->SR1 & I2C_SR1_ADDR) == 0) && timeout)
    {
        // timeout--;
    }

    // 清除ADDR位
    I2C2->SR2;
}

/**
 * @brief I2C发送一个字节数据
 *
 * @param byte 需要发送的数据
 */
void I2C_SendByte(uint8_t byte)
{
    uint16_t timeout = 0xFFFF;

    // 等待DR为空
    while (((I2C2->SR1 & I2C_SR1_TXE) == 0) && timeout)
    {
        timeout--;
    }

    // 将需要发送的数据填入数据寄存器
    I2C2->DR = byte;
    timeout = 0xFFFF;

    // 等待字节发送完成
    while (((I2C2->SR1 & I2C_SR1_BTF) == 0) && timeout)
    {
        timeout--;
    }
}

/**
 * @brief I2C接收一个字节的数据
 *
 * @return uint8_t 接收到的数据
 */
uint8_t I2C_ReadByte(void)
{
    uint16_t timeout = 0XFFFF;

    // 等待数据寄存器满
    while (((I2C2->SR1 & I2C_SR1_RXNE) == 0) && timeout)
    {
        timeout--;
    }

    if (timeout > 0)
    {
        return I2C2->DR;
    }
    else
    {
        return 0;
    }
}

#endif
