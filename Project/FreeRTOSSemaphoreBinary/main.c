#include "Foundation.h"
#include "gd32vf103_eclic.h"
#include "USART.h"
#include "LED.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "tsprintf.h"

typedef struct
{
    int value;
} Message;

TaskHandle_t taskCreater = NULL;
TaskHandle_t taskA = NULL;
TaskHandle_t taskB = NULL;
TaskHandle_t taskC = NULL;

SemaphoreHandle_t semaphoreBinary = NULL;

// 系统初始化函数使用标准工具链时没有在main函数前调用,声明后手动调用
extern void _init();

void TaskCreater(void *parameters);
void TaskA(void *parameters);
void TaskB(void *parameters);
void TaskC(void *parameters);

void IRQConfigure(void)
{
    eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL4_PRIO0); // 4 bits for level, 0 bits for priority
    eclic_global_interrupt_enable();                       // enable global interrupt
}

int main(void)
{
    size_t size = 0;
    /* systesm init */
    _init();
    IRQConfigure();
    USARTInit();
    LEDInit();
    semaphoreBinary = xSemaphoreCreateBinary();
    if (semaphoreBinary != NULL)
    {
#if DEBUG
        printf("Semaphore create Succeed\r\n");
#endif
    }
    else
    {
#if DEBUG
        printf("Semaphore create Failed\r\n");
#endif
    }
    xTaskCreate(TaskCreater, "TaskCreater", 256, NULL, 2, &taskCreater);
    vTaskStartScheduler();

    while (1)
    {
        printf("RTOS Exit\r\n");
    }
    return 0;
}

void TaskCreater(void *parameters)
{
    taskENTER_CRITICAL();
    // xTaskCreate(TaskA, "TaskA", 256, NULL, 3, &taskA);
    // xTaskCreate(TaskB, "TaskB", 256, NULL, 3, &taskB);
    xTaskCreate(TaskC, "TaskC", 256, NULL, 3, &taskC);
    taskEXIT_CRITICAL();
    vTaskDelete(taskCreater);
}

void TaskA(void *parameters)
{
    while (1)
    {
        if (semaphoreBinary != NULL)
        {
            if (xQueueSemaphoreTake(semaphoreBinary, portMAX_DELAY) == pdTRUE)
            {
#if DEBUG
                printf("TaskA semaphore take succeed\r\n");
#endif
                ToggleLED(LEDItemRed);
            }
            else
            {
#if DEBUG
                printf("TaskA semaphore take failed\r\n");
#endif
            }
        }
    }
}

void TaskB(void *parameters)
{
    uint random = 0;
    while (1)
    {
        if (semaphoreBinary != NULL)
        {
            if (xQueueSemaphoreTake(semaphoreBinary, portMAX_DELAY) == pdTRUE)
            {
#if DEBUG
                printf("TaskB semaphore take succeed\r\n");
#endif
                ToggleLED(LEDItemGreen);
            }
            else
            {
#if DEBUG
                printf("TaskB semaphore take failed\r\n");
#endif
            }
        }
    }
}

void TaskC(void *parameters)
{
    uint random = 0;
    //     while (1)
    //     {
    //         if (xSemaphoreGive(semaphoreBinary) == pdPASS)
    //         {
    // #if DEBUG
    //             printf("TaskC semaphore give succeed\r\n");
    // #endif
    //         }
    //         else
    //         {
    // #if DEBUG
    //             printf("TaskC semaphore give failed\r\n");
    // #endif
    //         }

    //     }
    while (1)
    {
        random = RandomWithRange(1, 5);
        vTaskDelay(pdMS_TO_TICKS(random * 1000));
#if DEBUG
        printf("task c done, delay = %ds\r\n", random);
#endif
    }
}

void freertos_risc_v_application_exception_handler(UBaseType_t mcause)
{
#if DEBUG_EXCEPTION
    printf("exception: 0x%04x\r\n", mcause);
    printf("In trap handler, the mcause is %d\n", mcause);
    printf("In trap handler, the mepc is 0x%x\n", read_csr(mepc));
    printf("In trap handler, the mtval is 0x%x\n", read_csr(mbadaddr));
    _exit(mcause);
#endif
}

void freertos_risc_v_application_interrupt_handler(UBaseType_t mcause)
{
#if DEBUG_INTERRUPT
    printf("interrupt: 0x%04x\r\n", mcause);
#endif
}

void vApplicationTickHook(void)
{
#if DEBUG_HOOK
// printf("Tick\r\n");
#endif
}

void vApplicationIdleHook(void)
{
#if DEBUG_HOOK
// printf("Idle\r\n");
#endif
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
#if DEBUG_HOOK
    printf("task：%s Overflow\r\n", pcTaskName);
#endif
}

void vApplicationMallocFailedHook(void)
{
#if DEBUG_HOOK
    printf("MallocFailed\r\n");
#endif
}

void vApplicationDaemonTaskStartupHook(void)
{
#if DEBUG_HOOK
    printf("DaemonTask\r\n");
#endif
}