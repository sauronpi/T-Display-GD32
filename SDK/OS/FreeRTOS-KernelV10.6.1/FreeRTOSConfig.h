/*
 * FreeRTOS V202212.01
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "n200_timer.h"

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

/* See https://www.freertos.org/Using-FreeRTOS-on-RISC-V.html */
#define configMTIME_BASE_ADDRESS                    ( TIMER_CTRL_ADDR + TIMER_MTIME )
#define configMTIMECMP_BASE_ADDRESS                 ( TIMER_CTRL_ADDR + TIMER_MTIMECMP )

/* 机器计时器时钟频率 */
#define configCPU_CLOCK_HZ                          ( ( UBaseType_t ) 108000000 / 4 )
#define configTICK_RATE_HZ                          ( ( TickType_t ) 1000)
/* 16位平台使用，32位禁用 */
#define configUSE_16_BIT_TICKS                  0

/* 任务相关 */
// #define configUSE_TIME_SLICING                  1/* 时间片轮询任务 默认开启 */
#define configUSE_PREEMPTION                    1/* 抢占式任务 */
// #define configIDLE_SHOULD_YIELD                 1/* 空闲任务是否马上让出运行时间 默认开启 */
#define configMAX_PRIORITIES                    5/* 最大任务优先级，优先级数值越小，那么此任务的优先级越低，空闲任务的优先级是 0
#define configMAX_TASK_NAME_LEN                 16/* 最大任务名长度 默认16 */
#define configUSE_TASK_NOTIFICATIONS            1/* 任务通知 */

#define configUSE_MUTEXES                       1/* 互斥锁 */
#define configUSE_RECURSIVE_MUTEXES             0/* 递归互斥锁 */
#define configUSE_COUNTING_SEMAPHORES           1/* 计数信号量 */
#define configQUEUE_REGISTRY_SIZE               10/* 可以注册的队列最大数量 */
#define configUSE_QUEUE_SETS                    0/* 队列集功能 */
#define configUSE_NEWLIB_REENTRANT              1/* newlib 可重入支持 */
#define configENABLE_BACKWARD_COMPATIBILITY     0/* 新版本对老版本兼容支持 */
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 5/* 设置每个任务的线程本地存储指针数组大小 */

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION         0
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configMINIMAL_STACK_SIZE                ( ( unsigned short ) 128 )/* 单位字 */
#define configTOTAL_HEAP_SIZE                   1024 * 4 * 4/* 单位字节 */
#define configAPPLICATION_ALLOCATED_HEAP        0

/* Hook function related definitions. */
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configCHECK_FOR_STACK_OVERFLOW          1
#define configUSE_MALLOC_FAILED_HOOK            1
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0
#define configUSE_SB_COMPLETED_CALLBACK         0

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_TRACE_FACILITY                1
#define configUSE_STATS_FORMATTING_FUNCTIONS    1

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         1

/* Software timer related definitions. */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               ( configMAX_PRIORITIES - 1 ) //Tmr_svc 独占最高优先级
#define configTIMER_QUEUE_LENGTH                5
#define configTIMER_TASK_STACK_DEPTH            configMINIMAL_STACK_SIZE

/* Interrupt nesting behaviour configuration. */
// #define configKERNEL_INTERRUPT_PRIORITY         [dependent of processor]
// #define configMAX_SYSCALL_INTERRUPT_PRIORITY    [dependent on processor and application]
// #define configMAX_API_CALL_INTERRUPT_PRIORITY   [dependent on processor and application]

/* Define to trap errors during development. */
#define configASSERT( x ) if( ( x ) == 0 ) {taskDISABLE_INTERRUPTS(); for( ;; );}

/* Optional functions - most linkers will remove unused functions anyway. */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xResumeFromISR                  1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     0
#define INCLUDE_uxTaskGetStackHighWaterMark2    1
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_eTaskGetState                   0
#define INCLUDE_xEventGroupSetBitFromISR        1
#define INCLUDE_xTimerPendFunctionCall          0
#define INCLUDE_xTaskAbortDelay                 0
#define INCLUDE_xTaskGetHandle                  0
#define INCLUDE_xTaskResumeFromISR              1

#endif /* FREERTOS_CONFIG_H */
