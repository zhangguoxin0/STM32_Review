#include "stm32f10x.h"
#include "bsp_led.h"
#include "NVIC.h"

int main(void)
{
    LED0_Init();
    IRQ_Init();
    while (1)
    {
    }
}
