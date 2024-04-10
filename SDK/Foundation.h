#ifndef FOUNDATION_H
#define FOUNDATION_H
#ifdef __cplusplus
 extern "C" {
#endif 

#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "gd32vf103_libopt.h"

#include "DEBUGConfig.h"

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;

// #define ON      1
// #define OFF     0

void delay(uint millionSeconds);
int RandomWithRange(int fromNumber, int toNumer);

#ifdef __cplusplus
}
#endif
#endif /* FOUNDATION_H */