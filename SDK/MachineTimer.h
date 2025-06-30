#ifndef MACHINETIMER_H
#define MACHINETIMER_H
#ifdef __cplusplus
 extern "C" {
#endif 

typedef enum
{
    MTTimerPeriodSecond = 1,
    MTTimerPeriodMillonSecond = 1000,
    MTTimerPeriodMicroSecond = 1000000,
} MTTimerPeriod;

void MTTimerSet(MTTimerPeriod period);
void MTTimerCountClear(void);

#ifdef __cplusplus
}
#endif
#endif /* MACHINETIMER_H */