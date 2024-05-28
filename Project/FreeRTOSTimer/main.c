#include "Foundation.h"
#include "gd32vf103_eclic.h"
#include "USART.h"
#include "LED.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "tsprintf.h"

// 系统初始化函数使用标准工具链时没有在main函数前调用,声明后手动调用
extern void _init();

void TaskCreate();
void TaskCreater(void *parameters);
void TaskA(void *parameters);
void TaskB(void *parameters);
void TaskC(void *parameters);
void TaskD(void *parameters);
void vTimerCallback(TimerHandle_t xTimer);

TaskHandle_t taskCreater = NULL;
TaskHandle_t taskA = NULL;
TaskHandle_t taskB = NULL;
TaskHandle_t taskC = NULL;
TaskHandle_t taskD = NULL;

typedef enum
{
    TimerIDLEDRed,
    TimerIDLEDGreen,
    TimerIDLEDBlue,
    TimerIDNumbers,
} TimerID;

char *timerNames[] = {"Red", "Green", "Blue"};

TimerHandle_t xTimers[TimerIDNumbers];

void IRQConfigure(void)
{
    eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL4_PRIO0); // 四位优先级组全配置为lvl
    eclic_global_interrupt_enable();                       // 使能全局中断
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    size_t size = 0;
    /* systesm init */
    _init();

    IRQConfigure();
    USARTInit();
    LEDInit();

    /* Create then start some timers.  Starting the timers before
    the RTOS scheduler has been started means the timers will start
    running immediately that the RTOS scheduler starts. */
    for (int x = 0; x < TimerIDNumbers; x++)
    {
        xTimers[x] = xTimerCreate(/* Just a text name, not used by the RTOS
                                  kernel. */
                                  timerNames[x],
                                  /* The timer period in ticks, must be
                                  greater than 0. */
                                  pdMS_TO_TICKS(500 * (x + 1)),
                                  /* The timers will auto-reload themselves
                                  when they expire. */
                                  pdTRUE,
                                  /* The ID is used to store a count of the
                                  number of times the timer has expired, which
                                  is initialised to 0. */
                                  (void *)x,
                                  /* Each timer calls the same callback when
                                  it expires. */
                                  vTimerCallback);

        if (xTimers[x] == NULL)
        {
            /* The timer was not created. */
        }
        else
        {
            /* Start the timer.  No block time is specified, and
            even if one was it would be ignored because the RTOS
            scheduler has not yet been started. */
            if (xTimerStart(xTimers[x], 0) != pdPASS)
            {
                /* The timer could not be set into the Active
                state. */
            }
        }
    }

    // TaskCreate();
    // xTaskCreate(TaskCreater, "TaskCreater", 256, NULL, 2, &taskCreater);
#if DEBUG
    printf("2\r\n");
#endif
    vTaskStartScheduler();
#if DEBUG
    while (1)
    {
        printf("RTOS Exit\r\n");
    }
#endif
}

/* retarget the C library printf function to the USART */
// int _put_char(int ch)
// {
//     usart_data_transmit(USART0, (uint8_t)ch);
//     while (usart_flag_get(USART0, USART_FLAG_TBE) == RESET);
//     return ch;
// }

void TaskCreate(void)
{
    size_t size = 0;
    size = xPortGetFreeHeapSize();
#if DEBUG
    printf("tc1 size %d\r\n", size);
#endif
    xTaskCreate(TaskA, "TaskA", 256, NULL, 3, &taskA);
    size = xPortGetFreeHeapSize();
#if DEBUG
    printf("tc2 size %d\r\n", size);
#endif
    xTaskCreate(TaskB, "TaskB", 256, NULL, 3, &taskB);
    size = xPortGetFreeHeapSize();
#if DEBUG
    printf("tc3 size %d\r\n", size);
#endif
    xTaskCreate(TaskC, "TaskC", 256, NULL, 3, &taskC);
    size = xPortGetFreeHeapSize();
#if DEBUG
    printf("tc4 size %d\r\n", size);
#endif
    xTaskCreate(TaskD, "TaskD", 256, NULL, 2, &taskD);
    size = xPortGetFreeHeapSize();
#if DEBUG
    printf("tc5 size %d\r\n", size);
#endif
#if DEBUG
    printf("A=%x\r\n", taskA);
    printf("B=%x\r\n", taskB);
    printf("C=%x\r\n", taskC);
    printf("D=%x\r\n", taskD);
#endif
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
    size_t size = 0;
    UBaseType_t mark = 0;
    while (1)
    {
#if DEBUG
        printf("TaskA\r\n");
        // size = xPortGetFreeHeapSize();
        // printf("ta size %d\r\n", size);
        // mark = uxTaskGetStackHighWaterMark(taskA);
        // printf("mark %d\r\n", mark);
#endif
        ToggleLED(LEDItemRed);
        vTaskDelay(500);
    }
}

void TaskB(void *parameters)
{
    while (1)
    {
#if DEBUG
        printf("TaskB\r\n");
#endif
        ToggleLED(LEDItemGreen);
        vTaskDelay(1000);
    }
}

void TaskC(void *parameters)
{
    while (1)
    {
#if DEBUG
        printf("TaskC\r\n");
#endif
        ToggleLED(LEDItemBlue);
        vTaskDelay(2000);
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
        mark = uxTaskGetStackHighWaterMark2(taskA);
        printf("Task A StackHighWaterMark %d\r\n", mark);
        mark = uxTaskGetStackHighWaterMark2(taskB);
        printf("Task B StackHighWaterMark %d\r\n", mark);
        mark = uxTaskGetStackHighWaterMark2(taskC);
        printf("Task C StackHighWaterMark %d\r\n", mark);
        mark = uxTaskGetStackHighWaterMark2(taskD);
        printf("Task D StackHighWaterMark %d\r\n", mark);
#endif
        vTaskDelay(1000);
    }
}

/* Define a callback function that will be used by multiple timer
instances.  The callback function does nothing but count the number
of times the associated timer expires, and stop the timer once the
timer has expired 10 times.  The count is saved as the ID of the
timer. */
void vTimerCallback(TimerHandle_t xTimer)
{
    TimerID timerID;

    /* Optionally do something if the pxTimer parameter is NULL. */
    configASSERT(xTimer);
    timerID = (uint32_t)pvTimerGetTimerID(xTimer);
    printf("timer call back: id is %d, name is %s\r\n", timerID, pcTimerGetName(xTimer));
    switch (timerID)
    {
    case TimerIDLEDRed:
        ToggleLED(LEDItemRed);
        break;
    case TimerIDLEDGreen:
        ToggleLED(LEDItemGreen);
        break;
    case TimerIDLEDBlue:
        ToggleLED(LEDItemBlue);
        break;
    default:
        break;
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