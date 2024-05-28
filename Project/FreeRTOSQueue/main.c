#include "Foundation.h"
#include "gd32vf103_eclic.h"
#include "USART.h"
#include "LED.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "tsprintf.h"

// 系统初始化函数使用标准工具链时没有在main函数前调用,声明后手动调用
extern void _init();

void TaskCreater(void *parameters);
void TaskA(void *parameters);
void TaskB(void *parameters);
void TaskC(void *parameters);
void TaskD(void *parameters);

typedef struct
{
    int value;
} Message;

TaskHandle_t taskCreater = NULL;
TaskHandle_t taskA = NULL;
TaskHandle_t taskB = NULL;
TaskHandle_t taskC = NULL;
TaskHandle_t taskD = NULL;

QueueHandle_t taskAMessageQueue = NULL;

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
    xTaskCreate(TaskA, "TaskA", 256, NULL, 2, &taskA);
    xTaskCreate(TaskB, "TaskB", 256, NULL, 2, &taskB);
    xTaskCreate(TaskC, "TaskC", 256, NULL, 2, &taskC);
    xTaskCreate(TaskD, "TaskD", 256, NULL, 2, &taskD);
    taskEXIT_CRITICAL();
    vTaskDelete(taskCreater);
}

void TaskA(void *parameters)
{
    Message message;
    taskAMessageQueue = xQueueCreate(10, sizeof(Message));
    while (1)
    {
        if (taskAMessageQueue != NULL)
        {
            if (xQueueReceive(taskAMessageQueue, &message, portMAX_DELAY) == pdTRUE)
            {
#if DEBUG
                printf("TaskA receive message succeed, value = %d\r\n", message.value);
#endif
                ToggleLED(LEDItemRed);
            }
            else
            {
#if DEBUG
                printf("TaskA receive message failed");
#endif
            }
        }
    }
}

void TaskB(void *parameters)
{
    Message message = { 1 };

    while (1)
    {
        if (taskAMessageQueue != NULL)
        {
            if (xQueueSend(taskAMessageQueue, &message, portMAX_DELAY) == pdTRUE)
            {
#if DEBUG
                printf("TaskB send message succeed, value = %d\r\n", message.value);
#endif
                ToggleLED(LEDItemGreen);
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
            else
            {
#if DEBUG
                printf("TaskB send message failed");
#endif
            }
        }
    }
}

void TaskC(void *parameters)
{
    Message message = { 2 };

    while (1)
    {
        if (taskAMessageQueue != NULL)
        {
            if (xQueueSend(taskAMessageQueue, &message, portMAX_DELAY) == pdTRUE)
            {
#if DEBUG
                printf("TaskC send message succeed, value = %d\r\n", message.value);
#endif
                ToggleLED(LEDItemBlue);
                vTaskDelay(pdMS_TO_TICKS(1500));
            }
            else
            {
#if DEBUG
                printf("TaskC send message failed");
#endif
            }
        }
    }
}

void TaskD(void *parameters)
{
    size_t size = 0;
    configSTACK_DEPTH_TYPE mark = 0;
    while (1)
    {
#if DEBUG
        printf("TaskD\r\n");
        size = xPortGetFreeHeapSize();
        printf("FreeHeapSize %d\r\n", size);
        mark = uxTaskGetStackHighWaterMark2(taskD);
        printf("StackHighWaterMark %d\r\n", mark);
#endif
        vTaskDelay(pdMS_TO_TICKS(1000));
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