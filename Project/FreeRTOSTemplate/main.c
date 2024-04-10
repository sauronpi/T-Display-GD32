#include "Foundation.h"
#include "gd32vf103_eclic.h"
#include "USART.h"
#include "LED.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "tsprintf.h"

// 系统初始化函数使用标准工具链时没有在main函数前调用,声明后手动调用
extern void _init();

void TaskCreate();
void TaskCreater(void *parameters);
void TaskA(void *parameters);
void TaskB(void *parameters);
void TaskC(void *parameters);
void TaskD(void *parameters);

TaskHandle_t taskCreater = NULL;
TaskHandle_t taskA = NULL;
TaskHandle_t taskB = NULL;
TaskHandle_t taskC = NULL;
TaskHandle_t taskD = NULL;

void IRQConfigure(void)
{
    eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL4_PRIO0); // 四位优先级组全配置为lvl
    eclic_global_interrupt_enable(); // 使能全局中断
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

#if DEBUG
    tsprintf("1\r\n");
#endif
    TaskCreate();
    // xTaskCreate(TaskCreater, "TaskCreater", 256, NULL, 2, &taskCreater);
#if DEBUG
    tsprintf("2\r\n");
#endif
    vTaskStartScheduler();
#if DEBUG
    while (1)
    {
        tsprintf("RTOS Exit\r\n");
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
    tsprintf("tc1 size %d\r\n", size);
#endif
    xTaskCreate(TaskA, "TaskA", 256, NULL, 3, &taskA);
    size = xPortGetFreeHeapSize();
#if DEBUG
    tsprintf("tc2 size %d\r\n", size);
#endif
    xTaskCreate(TaskB, "TaskB", 256, NULL, 3, &taskB);
    size = xPortGetFreeHeapSize();
#if DEBUG
    tsprintf("tc3 size %d\r\n", size);
#endif
    xTaskCreate(TaskC, "TaskC", 256, NULL, 3, &taskC);
    size = xPortGetFreeHeapSize();
#if DEBUG
    tsprintf("tc4 size %d\r\n", size);
#endif
    xTaskCreate(TaskD, "TaskD", 256, NULL, 2, &taskD);
    size = xPortGetFreeHeapSize();
#if DEBUG
    tsprintf("tc5 size %d\r\n", size);
#endif
#if DEBUG
    tsprintf("A=%x\r\n", taskA);
    tsprintf("B=%x\r\n", taskB);
    tsprintf("C=%x\r\n", taskC);
    tsprintf("D=%x\r\n", taskD);
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
        taskENTER_CRITICAL();
        tsprintf("TaskA\r\n");
        // size = xPortGetFreeHeapSize();
        // tsprintf("ta size %d\r\n", size);
        // mark = uxTaskGetStackHighWaterMark(taskA);
        // tsprintf("mark %d\r\n", mark);
        taskEXIT_CRITICAL();
#endif
        LEDToggle(LED_RED);
        vTaskDelay(500);
    }
}

void TaskB(void *parameters)
{
    while (1)
    {
#if DEBUG
        taskENTER_CRITICAL();
        tsprintf("TaskB\r\n");
        taskEXIT_CRITICAL();
#endif
        LEDToggle(LED_GREEN);
        vTaskDelay(1000);
    }
}

void TaskC(void *parameters)
{
    while (1)
    {
#if DEBUG
        taskENTER_CRITICAL();
        tsprintf("TaskC\r\n");
        taskEXIT_CRITICAL();
#endif
        LEDToggle(LED_BLUE);
        vTaskDelay(1500);
    }
}

void TaskD(void *parameters)
{
    size_t size = 0;
    configSTACK_DEPTH_TYPE mark = 0;
    while (1)
    {
#if DEBUG
        taskENTER_CRITICAL();
        tsprintf("TaskD\r\n");
        size = xPortGetFreeHeapSize();
        tsprintf("FreeHeapSize %d\r\n", size);
        mark = uxTaskGetStackHighWaterMark2(taskD);
        tsprintf("StackHighWaterMark %d\r\n", mark);
        taskEXIT_CRITICAL();
#endif
        vTaskDelay(1000);
    }
}

void freertos_risc_v_application_exception_handler(UBaseType_t mcause)
{
    UBaseType_t status = 0;

    status = taskENTER_CRITICAL_FROM_ISR();
    tsprintf("exception: 0x%04x\r\n", mcause);
    taskEXIT_CRITICAL_FROM_ISR(status);
    // write(1, "trap\n", 5);
    // printf("In trap handler, the mcause is %d\n", mcause);
    // printf("In trap handler, the mepc is 0x%x\n", read_csr(mepc));
    // printf("In trap handler, the mtval is 0x%x\n", read_csr(mbadaddr));
    // _exit(mcause);
    // return 0;
}

void freertos_risc_v_application_interrupt_handler(UBaseType_t mcause)
{
    UBaseType_t status = 0;

    status = taskENTER_CRITICAL_FROM_ISR();
    tsprintf("interrupt: 0x%04x\r\n", mcause);
    taskEXIT_CRITICAL_FROM_ISR(status);
}

void vApplicationTickHook(void)
{
    // tsprintf("Tick\r\n");
}

void vApplicationIdleHook(void)
{
    // tsprintf("Idle\r\n");
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
#if DEBUG_HOOK
    tsprintf("task：%s Overflow\r\n", pcTaskName);
#endif
}

void vApplicationMallocFailedHook(void)
{
#if DEBUG_HOOK
    tsprintf("MallocFailed\r\n");
#endif
}

void vApplicationDaemonTaskStartupHook(void)
{
#if DEBUG_HOOK
    tsprintf("DaemonTask\r\n");
#endif
}