# 简单线程池实现

这是一个简单的 C 语言实现的线程池，用于管理和执行任务队列中的任务。该线程池包括以下功能：

- 创建线程池
- 向线程池中推送任务
- 销毁线程池

## 使用方法

1. 将 `threadpool.c` 文件添加到您的项目中。
2. 包含 `threadpool.h` 头文件以便使用线程池功能。
3. 使用 `nThreadPoolCreate` 函数创建线程池。
4. 使用 `nThreadPoolPushTask` 函数向线程池中推送任务。
5. 使用 `nThreadPoolDestroy` 函数销毁线程池。

## 示例代码

c

#include "threadpool.h"

int main() {

    // 创建线程池

    ThreadPool pool;

    nThreadPoolCreate(&pool);

    // 创建任务

    Task task;

    int arg = 42; // 任务参数

    task.task_func = task_entry;

    task.arg = &arg;

    // 推送任务到线程池

    nThreadPoolPushTask(&pool, task);

    // 销毁线程池

    nThreadPoolDestroy(&pool);

    return 0;

}

Copy code


## 注意事项

- 确保在使用线程池功能之前正确初始化和销毁线程池。
- 任务执行函数应该符合 `void task_func(struct Task *)` 的函数指针定义。

作者：源远流长