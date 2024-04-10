#include "MachineTimer.h"
#include <stdint.h>
#include "n200_timer.h"

void MTTimerSet(MTTimerPeriod period)
{
    *(uint64_t *)(TIMER_CTRL_ADDR + TIMER_MTIMECMP) = TIMER_FREQ / period;
    *(uint64_t *)(TIMER_CTRL_ADDR + TIMER_MTIME) = 0;
}

void MTTimerCountClear(void)
{
    *(uint64_t *)(TIMER_CTRL_ADDR + TIMER_MTIME) = 0;
}
