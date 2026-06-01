#include "can.h"

static void CAN_FilterConfig(void);

void CAN_Init(void)
{
    // 开启时钟
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    // CAN引脚重映射
    AFIO->MAPR |= AFIO_MAPR_CAN_REMAP_1;
    AFIO->MAPR &= ~AFIO_MAPR_CAN_REMAP_0;
    // GPIO工作模式配置 -> PB8:浮空输入 PB9:推挽输出
    GPIOB->CRH &= ~GPIO_CRH_MODE8;
    GPIOB->CRH &= ~GPIO_CRH_CNF8_1;
    GPIOB->CRH |= GPIO_CRH_CNF8_0;
    GPIOB->CRH |= GPIO_CRH_MODE9;
    GPIOB->CRH |= GPIO_CRH_CNF9_1;
    GPIOB->CRH &= ~GPIO_CRH_CNF9_0;

    /* CAN初始化基本配置 */
    // 1.进入初始化模式
    CAN1->MCR |= CAN_MCR_INRQ;
    while ((CAN1->MSR & CAN_MSR_INAK) == 0)
    {
    }
    // 2.退出睡眠模式
    CAN1->MCR &= ~CAN_MCR_SLEEP;
    while ((CAN1->MSR & CAN_MSR_SLAK))
    {
    }
    // 3.开启自动离线管理
    CAN1->MCR |= CAN_MCR_ABOM;
    // 4.开启自动唤醒管理
    CAN1->MCR |= CAN_MCR_AWUM;
    // 5.配置工作模式位环回静默模式
    CAN1->BTR |= CAN_BTR_SILM; // 进入静默模式
    CAN1->BTR |= CAN_BTR_LBKM; // 进入环回模式
    // 6.配置位时序
    CAN1->BTR &= ~CAN_BTR_BRP;
    CAN1->BTR |= (35 << 0); // 配置波特率分频器，35 + 1分频，Tq = 1us
    CAN1->BTR &= ~CAN_BTR_TS1;
    CAN1->BTR |= (2 << 16); // 配置BS1时间长度
    CAN1->BTR &= ~CAN_BTR_TS2;
    CAN1->BTR |= (5 << 20); // 配置BS2时间长度
    CAN1->BTR &= ~CAN_BTR_SJW;
    CAN1->BTR |= (1 << 24); // 配置跳跃宽度
    // 7.退出初始化模式
    CAN1->MCR &= ~CAN_MCR_INRQ;
    while ((CAN1->MSR & CAN_MSR_INAK))
    {
    }
    // 8.过滤器配置
    CAN_FilterConfig();
}

/**
 * @brief CAN接收过滤器配置
 *
 */
static void CAN_FilterConfig(void)
{
    // 1.进入初始化模式
    CAN1->FMR |= CAN_FMR_FINIT;
    // 2.配置过滤器工作模式：屏蔽位模式
    CAN1->FM1R &= ~CAN_FM1R_FBM0;
    // 3.设置位宽：32位
    CAN1->FS1R |= CAN_FS1R_FSC0;
    // 4.关联接收队列：FIFO0
    CAN1->FFA1R &= ~CAN_FFA1R_FFA0;
    // 5.设置过滤器组0的ID寄存器：FR1
    CAN1->sFilterRegister[0].FR1 = 0x66 << 21;
    // 6.设置过滤器组0的屏蔽位寄存器：FR2
    CAN1->sFilterRegister[0].FR2 = (uint32_t)0x7FF << 21;
    // 7.激活过滤器组0
    CAN1->FA1R |= CAN_FA1R_FACT0;
    // 8.退出初始化模式
    CAN1->FMR &= ~CAN_FMR_FINIT;
}

/**
 * @brief 发送报文(使用发送邮箱0)
 *
 * @param stdID 报文ID
 * @param data 数据
 * @param len 数据长度(单位：字节)
 */
void CAN_SendMsg(uint16_t stdID, uint8_t *data, uint8_t len)
{
    // 1.等待发送邮箱0为空
    while ((CAN1->TSR & CAN_TSR_TME0) == 0)
    {
    }
    // 2.包装要发送的数据
    // 2.1.设置标准ID
    CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_STID;
    CAN1->sTxMailBox[0].TIR |= (stdID << 21);
    // 2.2.设置为标准帧
    CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_IDE;
    // 2.3.设置为数据帧
    CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_RTR;
    // 2.4.设置数据长度
    CAN1->sTxMailBox[0].TDTR &= ~CAN_TDT0R_DLC;
    CAN1->sTxMailBox[0].TDTR |= (len << 0);
    // 2.5.设置数据
    CAN1->sTxMailBox[0].TDHR = 0;
    CAN1->sTxMailBox[0].TDLR = 0;
    for (uint8_t i = 0; i < len; i++)
    {
        // 前4个字节放到TDLR中
        if (i < 4)
        {
            CAN1->sTxMailBox[0].TDLR |= (data[i] << (i * 8));
        }
        // 后4个字节放到TDLR中
        else
        {
            CAN1->sTxMailBox[0].TDHR |= (data[i] << ((i - 4) * 8));
        }
    }
    // 3.请求发送数据帧
    CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;
    // 4.等待发送完成
    while ((CAN1->TSR & CAN_TSR_TXOK0) == 0)
    {
    }
}

/**
 * @brief 接收报文(从FIFO0中读取)
 *
 * @param rxMsg 报文结构体数组
 * @param msgcount 数组长度
 */
void CAN_ReceiveMsg(RxMag rxMsg[], uint8_t *msgcount)
{
    // 1.获取FIFO0的报文个数
    *msgcount = ((CAN1->RF0R & CAN_RF0R_FMP0) >> 0);
    // 2.循环读取每一个报文
    for (uint8_t i = 0; i < *msgcount; i++)
    {
        // 定义指针，指向当前保存报文的数据对象
        RxMag *msg = &rxMsg[i];
        // 读取ID
        msg->stdID = (CAN1->sFIFOMailBox[0].RIR >> 21) & 0x7FF;
        // 读取数据长度
        msg->len = (CAN1->sFIFOMailBox[0].RDTR >> 0) & 0x0F;
        // 读取数据
        uint32_t low = CAN1->sFIFOMailBox[0].RDLR;
        uint32_t high = CAN1->sFIFOMailBox[0].RDHR;
        for (uint8_t j = 0; j < msg->len; j++)
        {
            // 如果是前4个字节，就从RDLR中提取
            if (j < 4)
            {
                msg->data[j] = ((low >> (8 * j)) & 0xFF);
            }
            // 如果是后4个字节，就从RDHR中提取
            else
            {
                msg->data[j] = (high >> (8 * (j - 4)) & 0xFF);
            }
        }
        // 释放FIFO0，当前FIFO0中的报文出队，读取下一个报文
        CAN1->RF0R |= CAN_RF0R_RFOM0;
    }
}
