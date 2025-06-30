#include "Foundation.h"
#include "gd32vf103_eclic.h"
#include "USART.h"
#include "LED.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

typedef struct
{
    int value;
} Message;

TaskHandle_t taskCreater = NULL;
TaskHandle_t taskA = NULL;
TaskHandle_t taskB = NULL;
TaskHandle_t taskC = NULL;
TaskHandle_t taskMonitor = NULL;

SemaphoreHandle_t semaphoreCounting = NULL;

// 系统初始化函数使用标准工具链时没有在main函数前调用,声明后手动调用
extern void _init();

void TaskCreate();
void TaskCreater(void *parameters);
void TaskA(void *parameters);
void TaskB(void *parameters);
void TaskC(void *parameters);
void TaskMonitor(void *parameters);

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

    semaphoreCounting = xSemaphoreCreateCounting(5, 2);
    if (semaphoreCounting != NULL)
    {
        printf("Semaphore create Succeed\r\n");
    }
    else
    {
        printf("Semaphore create Failed\r\n");
    }
    // TaskCreate();
    TaskCreate(TaskCreater, "TaskCreater", 256, NULL, 2, &taskCreater);
    vTaskStartScheduler();
    printf("FreeRTOS Exit\r\n");
    for (;;)
        ;
    return 0;
}

void TaskCreate(void)
{
    size_t size = 0;
    size = xPortGetFreeHeapSize();
    printf("tc1 size %d\r\n", size);
    xTaskCreate(TaskA, "TaskA", 256, NULL, 3, &taskA);
    size = xPortGetFreeHeapSize();
    printf("tc2 size %d\r\n", size);
    xTaskCreate(TaskB, "TaskB", 256, NULL, 3, &taskB);
    size = xPortGetFreeHeapSize();
    printf("tc3 size %d\r\n", size);
    xTaskCreate(TaskC, "TaskC", 256, NULL, 3, &taskC);
    size = xPortGetFreeHeapSize();
    printf("tc4 size %d\r\n", size);
    xTaskCreate(TaskMonitor, "TaskMonitor", 256, NULL, 2, &taskMonitor);
    size = xPortGetFreeHeapSize();
    printf("tc5 size %d\r\n", size);
    printf("A=%x\r\n", taskA);
    printf("B=%x\r\n", taskB);
    printf("C=%x\r\n", taskC);
    printf("D=%x\r\n", taskMonitor);
}

void TaskCreater(void *parameters)
{
    taskENTER_CRITICAL();
    xTaskCreate(TaskA, "TaskA", 256, NULL, 2, &taskA);
    xTaskCreate(TaskB, "TaskB", 256, NULL, 2, &taskB);
    xTaskCreate(TaskC, "TaskC", 256, NULL, 2, &taskC);
    xTaskCreate(TaskMonitor, "TaskMonitor", 256, NULL, 2, &taskMonitor);
    taskEXIT_CRITICAL();
    vTaskDelete(taskCreater);
}

void TaskA(void *parameters)
{
    while (1)
    {
        if (semaphoreCounting != NULL)
        {
            UBaseType_t count = uxSemaphoreGetCount(semaphoreCounting);
            printf("TaskA get semaphore current count = %d\r\n", count);
            if (xQueueSemaphoreTake(semaphoreCounting, portMAX_DELAY) == pdTRUE)
            {
                count = uxSemaphoreGetCount(semaphoreCounting);
                printf("TaskA semaphore take succeed, current count = %d\r\n", count);
                ToggleLED(LEDItemRed);
            }
            else
            {
                printf("TaskA semaphore take failed\r\n");
            }
        }
    }
}

void TaskB(void *parameters)
{
    while (1)
    {
        if (semaphoreCounting != NULL)
        {
            UBaseType_t count = uxSemaphoreGetCount(semaphoreCounting);
            printf("TaskB get semaphore current count = %d\r\n", count);
            if (xQueueSemaphoreTake(semaphoreCounting, portMAX_DELAY) == pdTRUE)
            {
                count = uxSemaphoreGetCount(semaphoreCounting);
                printf("TaskB semaphore take succeed, current count = %d\r\n", count);
                ToggleLED(LEDItemGreen);
            }
            else
            {
                printf("TaskB semaphore take failed\r\n");
            }
        }
    }
}

void TaskC(void *parameters)
{
    while (1)
    {
        if (semaphoreCounting != NULL)
        {
            UBaseType_t count = uxSemaphoreGetCount(semaphoreCounting);
            printf("TaskC get semaphore current count = %d\r\n", count);
            if (xSemaphoreGive(semaphoreCounting) == pdPASS)
            {
                UBaseType_t count = uxSemaphoreGetCount(semaphoreCounting);
                printf("TaskC semaphore give succeed, current count = %d\r\n", count);
                ToggleLED(LEDItemBlue);
            }
            else
            {
                printf("TaskC semaphore give failed");
            }
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

void TaskMonitor(void *parameters)
{
    size_t size = 0;
    configSTACK_DEPTH_TYPE mark = 0;
    while (1)
    {
        printf("TaskMonitor\r\n");
        size = xPortGetFreeHeapSize();
        printf("FreeHeapSize %d\r\n", size);
        mark = uxTaskGetStackHighWaterMark2(taskA);
        printf("Task A StackHighWaterMark %d\r\n", mark);
        mark = uxTaskGetStackHighWaterMark2(taskB);
        printf("Task B StackHighWaterMark %d\r\n", mark);
        mark = uxTaskGetStackHighWaterMark2(taskC);
        printf("Task C StackHighWaterMark %d\r\n", mark);
        mark = uxTaskGetStackHighWaterMark2(taskMonitor);
        printf("Task D StackHighWaterMark %d\r\n", mark);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void freertos_risc_v_application_exception_handler(UBaseType_t mcause)
{
    printf("exception: the mcause is 0x%x\r\n", mcause);
    // printf("exception, the mepc is 0x%x\n", read_csr(mepc));
    // printf("exception, the mtval is 0x%x\n", read_csr(mbadaddr));
    for (;;)
        ;
}

void freertos_risc_v_application_interrupt_handler(UBaseType_t mcause)
{
#if DEBUG_INTERRUPT
    printf("interrupt: the mcause is %x\r\n", mcause);
#endif
}

void vApplicationTickHook(void)
{
#if configUSE_TICK_HOOK == ON
    printf("Tick\r\n");
#endif
}

void vApplicationIdleHook(void)
{
#if configUSE_IDLE_HOOK == ON
    printf("Idle\r\n");
#endif
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
#if configCHECK_FOR_STACK_OVERFLOW == ON
    printf("task：%s Overflow\r\n", pcTaskName);
#endif
}

void vApplicationMallocFailedHook(void)
{
#if configUSE_MALLOC_FAILED_HOOK == ON
    printf("MallocFailed\r\n");
#endif
}

void vApplicationDaemonTaskStartupHook(void)
{
#if configUSE_DAEMON_TASK_STARTUP_HOOK == ON
    printf("DaemonTask\r\n");
#endif
}