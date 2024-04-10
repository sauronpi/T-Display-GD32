#include "Foundation.h"
#include "USART.h"
#include "LED.h"
// 系统初始化函数使用标准工具链时没有在main函数前调用,声明后手动调用
extern void _init();

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

    while (1)
    {
        printf("hello, world\r\n");
        LEDToggle(LED_RED);
        LEDToggle(LED_GREEN);
        LEDToggle(LED_BLUE);
        delay(500);
    }
}

/* retarget the C library printf function to the USART */
// int _put_char(int ch)
// {
//     usart_data_transmit(USART0, (uint8_t)ch);
//     while (usart_flag_get(USART0, USART_FLAG_TBE) == RESET);

//     return ch;
// }