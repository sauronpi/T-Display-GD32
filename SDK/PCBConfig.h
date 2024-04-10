#ifndef PCBCONFIG_H
#define PCBCONFIG_H
#ifdef __cplusplus
extern "C" {
#endif 

#include "gd32vf103_gpio.h"

#define SPI0_GPIO_PORT                  GPIOA
#define SPI0_GPIO_MODE                  GPIO_MODE_AF_PP
#define SPI0_NSS_PIN                    GPIO_PIN_4
#define SPI0_SCK_PIN                    GPIO_PIN_5
#define SPI0_MISO_PIN                   GPIO_PIN_6
#define SPI0_MOSI_PIN                   GPIO_PIN_7

#ifdef __cplusplus
}
#endif
#endif /* PCBCONFIG_H */