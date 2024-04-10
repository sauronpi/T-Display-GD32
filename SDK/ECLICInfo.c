#include "ECLICInfo.h"
#include <stdio.h>
#include "n200_func.h"

ECLICInfo MakeECLICInfo(uint32_t value)
{
    ECLICInfo info;
    info.numInterruot = value & 0x00001FFF;
    info.version = (value >> 13) & 0x0000000F;
    info.clicintctlbits = (value >> 21) & 0x0000000F;
    info.reserved = 0;
    return info;
}

void PrintfECLICInfo()
{
    ECLICInfo info = MakeECLICInfo(eclic_get_clicinfo());
    printf("ECLICInfo: numInterruot=%d, version=%d, clicintctlbits=%d\n",
           info.numInterruot,
           info.version,
           info.clicintctlbits);
}
