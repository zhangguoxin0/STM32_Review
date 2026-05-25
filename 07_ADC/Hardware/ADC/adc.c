#include "adc.h"

/* ADC单通道 */
#if 0
void ADC1_Init(void)
{
    // 开启时钟
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    // ADC时钟频率(需要小于14MHz) -> 72MHz / 预分频系数(这里使用6分频)
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;
    // PC0工作模式 -> 模拟输入
    GPIOC->CRL &= ~GPIO_CRL_MODE0;
    GPIOC->CRL &= ~GPIO_CRL_CNF0;

    /* ADC配置 */
    ADC1->CR1 &= ~ADC_CR1_SCAN;        // 工作模式：禁用扫描模式
    ADC1->CR2 |= ADC_CR2_CONT;         // 开启连续转换模式：一轮采集结束后开启下一轮采集
    ADC1->CR2 &= ~ADC_CR2_ALIGN;       // 数据右对齐
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP10_2; // 采样周期 -> 001 -> 7.5周期
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP10_1; // 采样周期 -> 001 -> 7.5周期
    ADC1->SMPR1 |= ADC_SMPR1_SMP10_0;  // 采样周期 -> 001 -> 7.5周期
    ADC1->SQR1 &= ~ADC_SQR1_L;         // 规则组中的通道个数 -> 0000 -> 1个
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;       // 清空SQR3寄存器前5位
    ADC1->SQR3 |= 10 << (5 * 0);       // 将通道10保存在规则通道组第1位
    ADC1->CR2 |= ADC_CR2_EXTTRIG;      // 设置规则通道的外部触发转换模式：不使用外部触发模式
    ADC1->CR2 |= ADC_CR2_EXTSEL;       // 选择启动规则通道组转换的事 -> 111 ->软件触发
}

/**
 * @brief 开启ADC1转化
 *
 */
void ADC1_Start(void)
{
    uint16_t timeout = 0xFFFF;

    // 上电唤醒
    ADC1->CR2 |= ADC_CR2_ADON;
    // ADC校准
    ADC1->CR2 |= ADC_CR2_CAL;
    // 校准完成后CAL会被硬件置0，所以这里等待CAL被置0
    while ((ADC1->CR2 & ADC_CR2_CAL) && timeout)
    {
    }
    // 启动转换规则通道
    ADC1->CR2 |= ADC_CR2_SWSTART;
    // 等待规则组开始转换
    // EOC：转换结束位 -> 0：转换未完成； 1：转换完成。
    timeout = 0xFFFF;
    while (((ADC1->SR & ADC_SR_EOC) == 0) && timeout)
    {
    }
}

/**
 * @brief 读取ADC1转换值
 * 
 * @return double 
 */
double ADC1_ReadData(void)
{
    return ADC1->DR * 3.3 / 4095;
}
#else
void ADC1_Init(void)
{
    // 开启时钟
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    // ADC时钟频率(需要小于14MHz) -> 72MHz / 预分频系数(这里使用6分频)
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;
    // PC0工作模式 -> 模拟输入
    GPIOC->CRL &= ~GPIO_CRL_MODE0;
    GPIOC->CRL &= ~GPIO_CRL_CNF0;
    // PC2工作模式 -> 模拟输入
    GPIOC->CRL &= ~GPIO_CRL_MODE2;
    GPIOC->CRL &= ~GPIO_CRL_CNF2;

    /* ADC配置 */
    // 1.工作模式：扫描模式
    ADC1->CR1 |= ADC_CR1_SCAN;
    // 2.开启连续转换模式：一轮采集结束后开启下一轮采集
    ADC1->CR2 |= ADC_CR2_CONT;
    // 3.数据右对齐
    ADC1->CR2 &= ~ADC_CR2_ALIGN;
    // 4.设置采样周期
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP10_2; // 通道10 -> 001 -> 7.5周期
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP10_1; // 通道10 -> 001 -> 7.5周期
    ADC1->SMPR1 |= ADC_SMPR1_SMP10_0;  // 通道10 -> 001 -> 7.5周期
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP12_2; // 通道12 -> 001 -> 7.5周期
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP12_1; // 通道12 -> 001 -> 7.5周期
    ADC1->SMPR1 |= ADC_SMPR1_SMP12_0;  // 通道12 -> 001 -> 7.5周期
    // 5.设置规则组通道个数
    ADC1->SQR1 &= ~ADC_SQR1_L;  // 规则组中的通道个数 -> 0001 -> 2个
    ADC1->SQR1 |= ADC_SQR1_L_0; // 规则组中的通道个数 -> 0001 -> 2个
    // 6.设置规则组通道顺序
    ADC1->SQR3 &= ~ADC_SQR3_SQ1; // 清空SQR3寄存器前5位
    ADC1->SQR3 |= 10 << (5 * 0); // 将通道10保存在规则通道组第1位
    ADC1->SQR3 &= ~ADC_SQR3_SQ2; // 清空SQR3寄存器前6~10位
    ADC1->SQR3 |= 12 << (5 * 1); // 将通道12保存在规则通道组第2位
    // 7.设置规则通道的外部触发转换模式：不使用外部触发模式
    ADC1->CR2 |= ADC_CR2_EXTTRIG;
    // 8.选择启动规则通道组转换的事 -> 111 ->软件触发
    ADC1->CR2 |= ADC_CR2_EXTSEL;
}

void ADC1_DMA_Init(void)
{
    // 开启时钟
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    /* DMA配置 */
    DMA1_Channel1->CCR &= ~DMA_CCR1_MEM2MEM; // 外设到存储器模式
    DMA1_Channel1->CCR &= ~DMA_CCR1_PL;      // 通道优先级低
    DMA1_Channel1->CCR &= ~DMA_CCR1_MSIZE_1; // 存储器数据宽度 -> 16位
    DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0;  // 存储器数据宽度 -> 16位
    DMA1_Channel1->CCR &= ~DMA_CCR1_PSIZE_1; // 外设数据宽度 -> 16位
    DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0;  // 外设数据宽度 -> 16位
    DMA1_Channel1->CCR |= DMA_CCR1_MINC;     // 存储器地址增量模式：执行存储器地址增量操作
    DMA1_Channel1->CCR &= ~DMA_CCR1_PINC;    // 外设地址增量模式：不执行外设地址增量操作
    DMA1_Channel1->CCR |= DMA_CCR1_CIRC;     // 执行循环操作
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR;     // 数据传输方向：从外设读

    // 开启ADC1的DMA模式
    ADC1->CR2 |= ADC_CR2_DMA;
}

/**
 * @brief 开启ADC1转化，使用DMA搬运数据
 *
 * @param destAddr 目的地址
 * @param dataLen 数据数量
 */
void ADC1_Start_DMA(uint32_t destAddr, uint16_t dataLen)
{
    uint16_t timeout = 0xFFFF;

    // 1.配置DMA外设地址 -> ADC数据寄存器地址
    DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR);
    // 2.配置DMA寄存器地址
    DMA1_Channel1->CMAR = destAddr;
    // 3.配置DMA数据传输数量
    DMA1_Channel1->CNDTR = dataLen;

    // 4.上电唤醒
    ADC1->CR2 |= ADC_CR2_ADON;
    // 5.ADC校准
    ADC1->CR2 |= ADC_CR2_CAL;
    // 校准完成后CAL会被硬件置0，所以这里等待CAL被置0
    while ((ADC1->CR2 & ADC_CR2_CAL) && timeout)
    {
    }
    // 6.启动转换规则通道(这里通过软件启动)
    ADC1->CR2 |= ADC_CR2_SWSTART;
    // 等待规则组开始转换
    // EOC：转换结束位 -> 0：转换未完成； 1：转换完成。
    timeout = 0xFFFF;
    while (((ADC1->SR & ADC_SR_EOC) == 0) && timeout)
    {
    }

    // 7.开启DMA数据传输
    DMA1_Channel1->CCR |= DMA_CCR1_EN;
}

#endif
