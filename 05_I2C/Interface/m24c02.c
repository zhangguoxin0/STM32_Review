#include "m24c02.h"
#include "i2c.h"
#include "Delay.h"

void M24C02_Init(void)
{
    I2C_Init();
}

/**
 * @brief 向M24C02写入一个字节的数据
 *
 * @param innerAddr 写入地址
 * @param byte 需要写入的数据
 */
void M24C02_WriteByte(uint8_t innerAddr, uint8_t byte)
{
    // 1.发送起始信号
    I2C_Start();
    // 2.发送写地址
    I2C_SendByte(W_ADDR);
    // 3.等待从机应答
    uint8_t ack = I2C_WaitACK();
    // 4.若从机有应答
    if (ack == 0)
    {
        // 5.发送需要写入数据的地址
        I2C_SendByte(innerAddr);
        // 6.等待应答信号
        I2C_WaitACK();
        // 7.发送需要写入的数据
        I2C_SendByte(byte);
        // 8.等待应答信号
        I2C_WaitACK();
        // 9.发送终止信号
        I2C_Stop();
    }
    else
    {
        // 每次等待应答如果没有接收到应答信号可以进行错误处理
    }

    // 延迟等待写周期结束
    Delay_ms(5);
}

/**
 * @brief 从M24C02读取一个字节的数据
 *
 * @param innerAddr 读取地址
 * @return uint8_t 读取到的数据
 */
uint8_t M24C02_ReadByte(uint8_t innerAddr)
{
    /* 假写 */
    // 1.发送起始信号
    I2C_Start();
    // 2.发送写地址
    I2C_SendByte(W_ADDR);
    // 3.等待从机应答
    I2C_WaitACK();
    // 4.发送需要写入数据的地址
    I2C_SendByte(innerAddr);
    // 5.等待应答信号
    I2C_WaitACK();

    /* 真读 */
    // 1.发送起始信号
    I2C_Start();
    // 2.发送读地址
    I2C_SendByte(R_ADDR);
    // 3.等待从机应答
    I2C_WaitACK();
    // 4.读取一个字节的数据
    uint8_t byte = I2C_ReadByte();
    // 5.发送非应答信号
    I2C_NACK();
    // 6.发出一个停止信号
    I2C_Stop();

    return byte;
}

/**
 * @brief 向指定地址写入指定个数的数据，需要考虑数据覆盖的问题(页写)，既写满一页(16byte)会回到页开始位置
 *
 * @param innerAddr 写入起始地址
 * @param bytes 需要写入的数据
 * @param size 需要写入的数据个数
 */
void M24C02_WriteBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t size)
{
    uint8_t i;

    // 1.发送起始信号
    I2C_Start();
    // 2.发送写地址
    I2C_SendByte(W_ADDR);
    // 3.等待从机应答
    uint8_t ack = I2C_WaitACK();
    // 4.若从机有应答
    if (ack == 0)
    {
        // 5.发送需要写入数据的地址
        I2C_SendByte(innerAddr);
        // 6.等待应答信号
        I2C_WaitACK();

        // 7.循化发送数据
        for (i = 0; i < size; i++)
        {
            // 发送需要写入数据的地址
            I2C_SendByte(bytes[i]);
            // 等待应答信号
            I2C_WaitACK();
        }

        // 8.发送终止信号
        I2C_Stop();
    }

    // 延迟等待写周期结束
    Delay_ms(5);
}

/**
 * @brief 从指定地址开始读取指定数量的数据，不需要考虑页的问题
 *
 * @param innerAddr 读取起始地址
 * @param buffer 读取数据缓冲区
 * @param size 读取到的数据个数
 */
void M24C02_ReadBytes(uint8_t innerAddr, uint8_t *buffer, uint8_t size)
{
    uint8_t i;

    /* 假写 */
    // 1.发送起始信号
    I2C_Start();
    // 2.发送写地址
    I2C_SendByte(W_ADDR);
    // 3.等待从机应答
    I2C_WaitACK();
    // 4.发送需要写入数据的地址
    I2C_SendByte(innerAddr);
    // 5.等待应答信号
    I2C_WaitACK();

    /* 真读 */
    // 1.发送起始信号
    I2C_Start();
    // 2.发送读地址
    I2C_SendByte(R_ADDR);
    // 3.等待从机应答
    I2C_WaitACK();

    // 4.循环接收数据
    for (i = 0; i < size; i++)
    {
        // 读取一个字节数据
        buffer[i] = I2C_ReadByte();

        // 向从机发送应答
        if (i < size)
        {
            I2C_ACK();
        }
        else
        {
            I2C_NACK();
        }
    }

    // 发出一个停止信号
    I2C_Stop();
}
