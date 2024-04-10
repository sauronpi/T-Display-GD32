# Semaphore Mutex

![alt freertos_semaphore_mutexes_animation](./freertos_semaphore_mutexes_animation.gif)
* 互斥信号量其实就是一个有优先级继承的二值信号量。
* 有优先级继承，降低优先级翻转带来影响。
* 只有任务级函数，没有中断级函数。因为有任务优先级继承，没有中断优先级继承。
* 使用二值信号量过程：创建互斥量 -> 获取互斥信号量 -> 释放互斥信号量