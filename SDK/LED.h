#ifndef LED_H
#define LED_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "Foundation.h"

typedef enum {
    LED_RED,
    LED_GREEN,
    LED_BLUE,
} LED;

void LEDInitWithEnable(bool enable);
void LEDInit(void);
void LEDControl(LED led, bool enable);
void LEDToggle(LED led);

#ifdef __cplusplus
}
#endif
#endif /* LED_H */