#include "adc.h"

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
