#ifndef TSPRINTF_H
#define TSPRINTF_H

#include "FreeRTOS.h"
#include "task.h"

void tsprintf(const char *format, ...);
void tsprintfisr(const char *format, ...);

#endif /* TSPRINTF_H */