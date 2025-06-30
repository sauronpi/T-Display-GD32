#include "Foundation.h"
#include "gd32vf103_eclic.h"
#include "USART.h"
#include "LED.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "tsprintf.h"

typedef struct
{
    int value;
} Message;

TaskHandle_t taskCreater = NULL;
TaskHandle_t taskA = NULL;
TaskHandle_t taskB = NULL;
TaskHandle_t taskC = NULL;
TaskHandle_t taskMonitor = NULL;

QueueHandle_t taskAMessageQueue = NULL;

// 系统初始化函数使用标准工具链时没有在main函数前调用,声明后手动调用
extern void _init();

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

    xTaskCreate(TaskCreater, "TaskCreater", 256, NULL, 2, &taskCreater);
    vTaskStartScheduler();
    printf("RTOS Exit\r\n");
    for(;;);
    return 0;
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
    Message message;
    taskAMessageQueue = xQueueCreate(10, sizeof(Message));
    while (1)
    {
        if (taskAMessageQueue != NULL)
        {
            if (xQueueReceive(taskAMessageQueue, &message, portMAX_DELAY) == pdPASS)
            {
                printf("TaskA receive message succeed, value = %d\r\n", message.value);
                ToggleLED(LEDItemRed);
            }
            else
            {
                printf("TaskA receive message failed");
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
            if (xQueueSend(taskAMessageQueue, &message, portMAX_DELAY) == pdPASS)
            {
                printf("TaskB send message succeed, value = %d\r\n", message.value);
                ToggleLED(LEDItemGreen);
                vTaskDelay(pdMS_TO_TICKS(500));
            }
            else
            {
                printf("TaskB send message failed");
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
            if (xQueueSend(taskAMessageQueue, &message, portMAX_DELAY) == pdPASS)
            {
                printf("TaskC send message succeed, value = %d\r\n", message.value);
                ToggleLED(LEDItemBlue);
                vTaskDelay(pdMS_TO_TICKS(500));
            }
            else
            {
                printf("TaskC send message failed");
            }
        }
    }
}

void TaskMonitor(void *parameters)
{
    Message message = { 3 };

    while (1)
    {
        if (taskAMessageQueue != NULL)
        {
            if (xQueueSend(taskAMessageQueue, &message, portMAX_DELAY) == pdPASS)
            {
                printf("TaskMonitor send message succeed, value = %d\r\n", message.value);
                ToggleLED(LEDItemBlue);
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
            else
            {
                printf("TaskC send message failed");
            }
        }
    }
}

void freertos_risc_v_application_exception_handler(UBaseType_t mcause)
{
    printf("exception: the mcause is 0x%x\r\n", mcause);
    // printf("exception, the mepc is 0x%x\n", read_csr(mepc));
    // printf("exception, the mtval is 0x%x\n", read_csr(mbadaddr));
    for(;;);
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