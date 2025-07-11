# Semaphore Binary 二值信号量
使用信号量同步任务与中断。 中断只“给出” 信号量，而任务只“接收”信号量。

![alt freertos_semaphore_binary_animation](./freertos_semaphore_binary_animation.gif)
* 通常用于互斥访问或任务同步
* 相当于队列长队等于1的队列
* 没有优先级继承，有可能会导致优先级翻转的问题，更适合用于任务同步
* 支持Task或ISR
* 使用二值信号量过程：创建二值信号量 -> 释放二值信号量 -> 获取二值信号量

## 任务优先级翻转问题
1. 创建 3 个任务 Task1，Task2 和 Task3，优先级分别为 3，2，1。 也就是 Task1 的优先级最高。
2. 任务 Task1 和 Task3 互斥访问串口打印 printf，采用二值信号实现互斥访问。
3. 起初 Task3 通过二值信号量正在调用 printf，被任务 Task1 抢占，开始执行任务 Task1，也就是上图的起始位置。 
运行过程描述如下：
4. 任务 Task1 运行的过程需要调用函数 printf，发现任务 Task3 正在调用，任务 Task1 会被挂起，等
待 Task3 释放函数 printf。
5. 在调度器的作用下，任务 Task3 得到运行，Task3 运行的过程中，由于任务 Task2 就绪，抢占了 Task3
的运行。 优先级翻转问题就出在这里了，从任务执行的现象上看，任务 Task1 需要等待 Task2 执行
完毕才有机会得到执行，这个与抢占式调度正好反了，正常情况下应该是高优先级任务抢占低优先级
任务的执行，这里成了高优先级任务 Task1 等待低优先级任务 Task2 完成。 所以这种情况被称之为
优先级翻转问题。
5. 任务 Task2 执行完毕后，任务 Task3 恢复执行，Task3 释放互斥资源后，任务 Task1 得到互斥资源，
从而可以继续执行。
