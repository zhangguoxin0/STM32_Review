/**
 * @file w25q32.c
 * @author 3024967223@qq.com
 * @brief W25Q32驱动程序，兼容W25Q64
 * @version 0.1
 * @date 2026-05-28
 *
 * @copyright Copyright (c) 2026
 *
 */
#include "w25q32.h"
#include "spi.h"

/**
 * @brief 拉高/拉低 W25Q32 片选信号
 *
 * @param value 0：选中W25Q32 1：断开W25Q32
 */
void W25Q32_CS_Set(uint8_t value)
{
    if (value == 0)
    {
        GPIOC->ODR &= ~GPIO_ODR_ODR9;
    }
    else
    {
        GPIOC->ODR |= GPIO_ODR_ODR9;
    }
}

/**
 * @brief 等待W25Q32空闲
 *
 */
void W25Q32_Wait(void)
{
    // 选中W25Q32
    W25Q32_CS_Set(0);

    // 发送读取状态寄存器指令
    SPI_SwapByte(0x05);
    // 等待收到的数据末位变成0
    while (SPI_SwapByte(0xFF) & 0x01)
    {
    }

    // 断开W25Q32
    W25Q32_CS_Set(1);
}

/**
 * @brief W25Q32写使能
 *
 */
void W25Q32_WriteEnable(void)
{
    W25Q32_CS_Set(0);   // 选中W25Q32
    SPI_SwapByte(0x06); // 发送写使能信号
    W25Q32_CS_Set(1);   // 断开W25Q32
}

/**
 * @brief W25Q32关闭写使能
 *
 */
void W25Q32_WriteDisenable(void)
{
    W25Q32_CS_Set(0);   // 选中W25Q32
    SPI_SwapByte(0x04); // 发送写失能信号
    W25Q32_CS_Set(1);   // 断开W25Q32
}

void W25Q32_Init(void)
{
    // 开启时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    // 配置GPIO
    // PC9 -> W25Q32片选 -> 推挽输出
    GPIOC->CRH |= GPIO_CRH_MODE9;
    GPIOC->CRH &= ~GPIO_CRH_CNF9;
    // 默认不选中W25Q32
    W25Q32_CS_Set(1);
    // 初始化SPI
    SPI_Init();
}

/**
 * @brief 读取ID信息
 *
 * @param mid 厂商ID
 * @param did 设备ID
 */
void W25Q32_ReadID(uint8_t *mid, uint16_t *did)
{
    // 选中W25Q32
    W25Q32_CS_Set(0);

    SPI_SwapByte(0x9F);                // 发送获取ID指令
    *mid = SPI_SwapByte(0xFF);         // 获取厂商ID
    *did = 0;                          // 清除原有数据，防止干扰
    *did |= (SPI_SwapByte(0xFF) << 8); // 获取设备ID高8位
    *did |= SPI_SwapByte(0xFF);        // 获取设备ID低8位

    // 断开W25Q32
    W25Q32_CS_Set(1);
}

/**
 * @brief 扇区擦除
 *
 * @param block 块号
 * @param sector 扇区号
 */
void W25Q32_SectorErase(uint8_t block, uint8_t sector)
{
    uint32_t addr = (block << 16) + (sector << 12); // 拼接地址

    W25Q32_Wait();        // 等待W25Q32状态不为忙
    W25Q32_WriteEnable(); // 写使能

    W25Q32_CS_Set(0);                  // 选中W25Q32
    SPI_SwapByte(0x20);                // 扇区擦除命令
    SPI_SwapByte((addr >> 16) & 0xFF); // 发送地址
    SPI_SwapByte((addr >> 8) & 0xFF);  // 发送地址
    SPI_SwapByte(addr & 0xFF);         // 发送地址
    W25Q32_CS_Set(1);                  // 断开W25Q32

    W25Q32_WriteDisenable(); // 关闭写使能
}

/**
 * @brief 页写
 *
 * @param block 块号
 * @param sector 扇区号
 * @param page 页号
 * @param data 数据
 * @param len 数据长度
 */
void W25Q32_PageWrie(uint8_t block, uint8_t sector, uint8_t page, uint8_t *data, uint16_t len)
{
    uint16_t i;
    uint32_t addr = (block << 16) + (sector << 12) + (page << 8); // 拼接地址

    W25Q32_Wait();        // 等待W25Q32状态不为忙
    W25Q32_WriteEnable(); // 写使能

    W25Q32_CS_Set(0);                  // 选中W25Q32
    SPI_SwapByte(0x02);                // 发送写指令
    SPI_SwapByte((addr >> 16) & 0xFF); // 发送地址
    SPI_SwapByte((addr >> 8) & 0xFF);  // 发送地址
    SPI_SwapByte(addr & 0xFF);         // 发送地址
    // 发送数据
    for (i = 0; i < len; i++)
    {
        SPI_SwapByte(data[i]);
    }
    W25Q32_CS_Set(1); // 断开W25Q32

    W25Q32_WriteDisenable(); // 关闭写使能
}

/**
 * @brief 读取数据
 *
 * @param block 块号
 * @param sector 扇区号
 * @param page 页号
 * @param innerAddr 地址
 * @param data 数据缓冲区
 * @param len 数据长度
 */
void W25Q32_Read(uint8_t block, uint8_t sector, uint8_t page, uint8_t innerAddr, uint8_t *data, uint16_t len)
{
    uint16_t i;
    uint32_t addr = (block << 16) + (sector << 12) + (page << 8) + innerAddr; // 拼接地址

    W25Q32_Wait(); // 等待W25Q32状态不为忙

    W25Q32_CS_Set(0);                  // 选中W25Q32
    SPI_SwapByte(0x03);                // 发送写指令
    SPI_SwapByte((addr >> 16) & 0xFF); // 发送地址
    SPI_SwapByte((addr >> 8) & 0xFF);  // 发送地址
    SPI_SwapByte(addr & 0xFF);         // 发送地址
    // 发送数据
    for (i = 0; i < len; i++)
    {
        data[i] = SPI_SwapByte(0xFF);
    }
    W25Q32_CS_Set(1); // 断开W25Q32
}
