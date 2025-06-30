#ifndef LED_H
#define LED_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "Foundation.h"

typedef enum {
    LEDItemRed,
    LEDItemGreen,
    LEDItemBlue,
} LEDItem;

typedef enum {
    LEDStatusOff,
    LEDStatusOn,
} LEDStatus;

void LEDInit(void);
void LEDInitWithStatus(LEDStatus status);
LEDStatus GetLEDStatus(LEDItem item);
void SetLEDStatus(LEDItem item, LEDStatus status);
void ToggleLED(LEDItem item);

#ifdef __cplusplus
}
#endif
#endif /* LED_H */