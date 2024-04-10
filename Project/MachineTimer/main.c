#include "Foundation.h"
#include "gd32vf103_eclic.h"
#include "MachineTimer.h"
#include "USART.h"
#include "LED.h"
#include "ECLICInfo.h"

// 系统初始化函数使用标准工具链时没有在main函数前调用,声明后手动调用
extern void _init();

uint32_t timerCount = 0;
uint32_t tickCount = 0;
bool tickCountChanged = false;

void ECLICConfig(void)
{
    eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL4_PRIO0); // 四位优先级组全配置为lvl 0-15
    eclic_irq_enable(CLIC_INT_TMR, 15, 0);                 // 0-15
    eclic_global_interrupt_enable();                       // 使能全局中断
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* systesm init */
    _init();

    USARTInit();
    LEDInit();
    MTTimerSet(MTTimerPeriodMillonSecond);
    ECLICConfig();

    printf("eclic_get_clicinfo:0x%X\n", eclic_get_clicinfo());
    PrintfECLICInfo();

    while (1)
    {
        if (tickCountChanged)
        {
            LEDToggle(LED_RED);
            tickCountChanged = false;
            printf("tickCount %d\r\n", tickCount);
            printf("Machine Timer Vlaue = %u %u\r\n", mtime_hi(), mtime_lo());
        }
    }
}

/* retarget the C library printf function to the USART */
// int _put_char(int ch)
// {
//     usart_data_transmit(USART0, (uint8_t)ch);
//     while (usart_flag_get(USART0, USART_FLAG_TBE) == RESET)
//         ;

//     return ch;
// }

void eclic_mtip_handler(void)
{
    MTTimerCountClear();
    timerCount++;
    if (timerCount == 1000)
    {
        timerCount = 0;
        tickCount++;
        tickCountChanged = true;
    }
}