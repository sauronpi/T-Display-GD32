#include "Foundation.h"
#include "gd32vf103_eclic.h"
#include "USART.h"
#include "LED.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

// 系统初始化函数使用标准工具链时没有在main函数前调用,声明后手动调用
extern void _init();

void TaskCreate(void);
void TaskCreater(void *parameters);
void TaskA(void *parameters);
void TaskB(void *parameters);
void TaskC(void *parameters);
void TaskMonitor(void *parameters);

TaskHandle_t taskCreater = NULL;
TaskHandle_t taskA = NULL;
TaskHandle_t taskB = NULL;
TaskHandle_t taskC = NULL;
TaskHandle_t taskMonitor = NULL;

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
    debugprint("1\r\n");
#endif
    TaskCreate();
    // xTaskCreate(TaskCreater, "TaskCreater", 256, NULL, 2, &taskCreater);
#if DEBUG
    debugprint("2\r\n");
#endif
    vTaskStartScheduler();
#if DEBUG
    while (1)
    {
        debugprint("RTOS Exit\r\n");
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
    debugprint("tc1 size %d\r\n", size);
#endif
    xTaskCreate(TaskA, "TaskA", 256, NULL, 3, &taskA);
    size = xPortGetFreeHeapSize();
#if DEBUG
    debugprint("tc2 size %d\r\n", size);
#endif
    xTaskCreate(TaskB, "TaskB", 256, NULL, 3, &taskB);
    size = xPortGetFreeHeapSize();
#if DEBUG
    debugprint("tc3 size %d\r\n", size);
#endif
    xTaskCreate(TaskC, "TaskC", 256, NULL, 3, &taskC);
    size = xPortGetFreeHeapSize();
#if DEBUG
    debugprint("tc4 size %d\r\n", size);
#endif
    xTaskCreate(TaskMonitor, "TaskMonitor", 256, NULL, 2, &taskMonitor);
    size = xPortGetFreeHeapSize();
#if DEBUG
    debugprint("tc5 size %d\r\n", size);
#endif
#if DEBUG
    debugprint("A=%x\r\n", taskA);
    debugprint("B=%x\r\n", taskB);
    debugprint("C=%x\r\n", taskC);
    debugprint("D=%x\r\n", taskMonitor);
#endif
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
    size_t size = 0;
    UBaseType_t mark = 0;
    while (1)
    {
#if DEBUG
        taskENTER_CRITICAL();
        debugprint("TaskA\r\n");
        // size = xPortGetFreeHeapSize();
        // debugprint("ta size %d\r\n", size);
        // mark = uxTaskGetStackHighWaterMark(taskA);
        // debugprint("mark %d\r\n", mark);
        taskEXIT_CRITICAL();
#endif
        ToggleLED(LEDItemRed);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void TaskB(void *parameters)
{
    while (1)
    {
#if DEBUG
        taskENTER_CRITICAL();
        printf("TaskB\r\n");
        taskEXIT_CRITICAL();
#endif
        ToggleLED(LEDItemGreen);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void TaskC(void *parameters)
{
    while (1)
    {
#if DEBUG
        taskENTER_CRITICAL();
        debugprint("TaskC\r\n");
        taskEXIT_CRITICAL();
#endif
        ToggleLED(LEDItemBlue);
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}

void TaskMonitor(void *parameters)
{
    size_t size = 0;
    configSTACK_DEPTH_TYPE mark = 0;
    while (1)
    {
#if DEBUG
        taskENTER_CRITICAL();
        debugprint("TaskMonitor\r\n");
        size = xPortGetFreeHeapSize();
        debugprint("FreeHeapSize %d\r\n", size);
        mark = uxTaskGetStackHighWaterMark2(taskMonitor);
        debugprint("StackHighWaterMark %d\r\n", mark);
        taskEXIT_CRITICAL();
#endif
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void freertos_risc_v_application_exception_handler(UBaseType_t mcause)
{
    UBaseType_t status = 0;

    status = taskENTER_CRITICAL_FROM_ISR();
    debugprint("exception: 0x%04x\r\n", mcause);
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
    debugprint("interrupt: 0x%04x\r\n", mcause);
    taskEXIT_CRITICAL_FROM_ISR(status);
}

void vApplicationTickHook(void)
{
    // printf("Tick\r\n");
}

void vApplicationIdleHook(void)
{
    // printf("Idle\r\n");
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
#if DEBUG_HOOK
    debugprint("task：%s Overflow\r\n", pcTaskName);
#endif
}

void vApplicationMallocFailedHook(void)
{
#if DEBUG_HOOK
    debugprint("MallocFailed\r\n");
#endif
}

void vApplicationDaemonTaskStartupHook(void)
{
#if DEBUG_HOOK
    debugprint("DaemonTask\r\n");
#endif
}