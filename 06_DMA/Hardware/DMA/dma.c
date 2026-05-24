#include "dma.h"

void DMA1_Init(void)
{
    // 开启时钟
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 配置DMA(这里使用DMA1的通道1)
    DMA1_Channel1->CCR |= DMA_CCR1_MEM2MEM; // 数据传输模式：寄存器-寄存器
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR;    // 数据传输方向：从外设读
    DMA1_Channel1->CCR &= ~DMA_CCR1_PSIZE;  // 外设数据宽度：8位
    DMA1_Channel1->CCR &= ~DMA_CCR1_MSIZE;  // 存储器数据宽度：8位
    DMA1_Channel1->CCR |= DMA_CCR1_PINC;    // 开启外设地址自增
    DMA1_Channel1->CCR |= DMA_CCR1_MINC;    // 开启存储器地址自增
    DMA1_Channel1->CCR &= ~DMA_CCR1_CIRC;   // 不开启循化模式
    DMA1_Channel1->CCR &= ~DMA_CCR1_PL;     // 通道优先级：低
    DMA1_Channel1->CCR |= DMA_CCR1_TCIE;    // 开启传输完成中断

    // NVIC配置
    NVIC_SetPriorityGrouping(3);             // 设置中断优先级分组
    NVIC_SetPriority(DMA1_Channel1_IRQn, 2); // 设置中断优先级
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);      // 使能中断
}

/**
 * @brief 使用DMA1传输数据
 *
 * @param srcAddr 源地址
 * @param destAddr 目的地址
 * @param dataLen 数据长度
 */
void DMA1_Transmit(uint32_t srcAddr, uint32_t destAddr, uint16_t dataLen)
{
    // 完成DMA其他配置
    DMA1_Channel1->CPAR = srcAddr;  // 设置源地址
    DMA1_Channel1->CMAR = destAddr; // 设置目的地址
    DMA1_Channel1->CNDTR = dataLen; // 设置数据传输数量

    // 开启DMA传输
    DMA1_Channel1->CCR |= DMA_CCR1_EN;
}

/**
 * @brief DMA1通道1中断服务程序
 *
 */
void DMA1_Channel1_IRQHandler(void)
{
    // 判断中断标志位
    if (DMA1->ISR & DMA_ISR_TCIF1)
    {
        // 传输完成标志位被置起 -> 通道1传输完成
        DMA1_Channel1->CCR &= ~DMA_CCR1_EN; // 关闭DMA通道

        IRQflag_DMA1Channel1_TransmitFinished = 1; // 传输完成标志位置1，表示传输完成

        // 清除通道1中断标志位
        DMA1->IFCR |= DMA_IFCR_CTCIF1;
    }
}
