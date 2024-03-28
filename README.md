# 简单线程池实现

这是一个简单的线程池实现，用于管理和执行任务。该线程池包括任务结构体 `nTask`、工作线程结构体 `nWorker` 和线程池结构体 `ThreadPool`。通过创建线程池和将任务推送到线程池中，可以实现多线程任务处理。

## 如何使用

### 编译

bash
Copy code

gcc -o main main.c -lpthread
### 运行

bash
Copy code

./main
## API

### `int nThreadPoolCreate(ThreadPool *pool, int numWorkers)`

创建一个线程池并启动指定数量的工作线程。

- `ThreadPool *pool`: 线程池结构体指针。

- `int numWorkers`: 要创建的工作线程数量。

### `int nThreadPoolDestroy(ThreadPool *pool)`

销毁线程池及其所有工作线程。

- `ThreadPool *pool`: 线程池结构体指针。

### `int nThreadPoolPushTask(ThreadPool *pool, struct nTask *task)`

将任务推送到线程池中等待执行。

- `ThreadPool *pool`: 线程池结构体指针。

- `struct nTask *task`: 要执行的任务结构体指针。