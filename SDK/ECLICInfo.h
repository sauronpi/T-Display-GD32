#ifndef ECLIC_INFO_H
#define ECLIC_INFO_H
#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdint.h>

typedef struct ECLICInfo
{
    uint32_t numInterruot : 13;  /* 0-12 */
    uint32_t version : 8;        /* 13-20 */
    uint32_t clicintctlbits : 4; /* 21-24 */
    uint32_t reserved : 7        /* 25-31 */;
} ECLICInfo;

ECLICInfo MakeECLICInfo(uint32_t value);
void PrintfECLICInfo();

#ifdef __cplusplus
}
#endif
#endif /* ECLIC_INFO_H */