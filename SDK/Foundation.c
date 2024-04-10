#include "Foundation.h"
#include "system_gd32vf103.h"
#include "n200_func.h"

/**
 * @brief  delay with million seconds
 * @note   
 * @param  millionSeconds: 
 * @retval None
 */
void delay(uint millionSeconds)
{
    uint64_t start_mtime, delta_mtime;

    /* get current timer value */
    uint64_t tmp = get_timer_value();
    do {
        start_mtime = get_timer_value();
    } while(start_mtime == tmp);

    /* continue counting until the delay time is reached */
    do {
        delta_mtime = get_timer_value() - start_mtime;
    } while(delta_mtime < (SystemCoreClock / 4000 * millionSeconds));
}

int RandomWithRange(int fromNumber, int toNumer)
{
    int minNumber = fromNumber < toNumer ? fromNumber : toNumer;
    int maxNumber = fromNumber > toNumer ? fromNumber : toNumer;
    return rand() % (maxNumber - minNumber + 1) + minNumber;;
}