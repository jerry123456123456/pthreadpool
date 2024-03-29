
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define THREAD_COUNT 5
#define TASK_QUEUE_SIZE 1000

typedef struct Task {
    void (*task_func)(struct Task *);
    void *arg;
} Task;

typedef struct ThreadPool {
    pthread_t threads[THREAD_COUNT];
    Task task_queue[TASK_QUEUE_SIZE];
    int queue_front;
    int queue_rear;
    int task_count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int shutdown;
} ThreadPool;

void task_execute(struct Task *task) {
    void (*task_func)(struct Task *) = task->task_func;
    void *arg = task->arg;
    task_func(task);
}


void *thread_function(void *arg) {
    ThreadPool *pool = (ThreadPool *)arg;
    while (1) {
        pthread_mutex_lock(&pool->mutex);
        while (pool->task_count == 0 && !pool->shutdown) {
            pthread_cond_wait(&pool->cond, &pool->mutex);
        }

        if (pool->shutdown) {
            pthread_mutex_unlock(&pool->mutex);
            pthread_exit(NULL);
        }

        Task task = pool->task_queue[pool->queue_front];
        pool->queue_front = (pool->queue_front + 1) % TASK_QUEUE_SIZE;
        pool->task_count--;

        pthread_mutex_unlock(&pool->mutex);

        task_execute(&task);
    }
    return NULL;
}

void nThreadPoolCreate(ThreadPool *pool) {
    pool->queue_front = 0;
    pool->queue_rear = 0;
    pool->task_count = 0;
    pool->shutdown = 0;

    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->cond, NULL);

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&pool->threads[i], NULL, thread_function, pool);
    }
}

void nThreadPoolPushTask(ThreadPool *pool, Task task) {
    pthread_mutex_lock(&pool->mutex);
    pool->task_queue[pool->queue_rear] = task;
    pool->queue_rear = (pool->queue_rear + 1) % TASK_QUEUE_SIZE;
    pool->task_count++;
    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);
}

void nThreadPoolDestroy(ThreadPool *pool) {
    pthread_mutex_lock(&pool->mutex);
    pool->shutdown = 1;
    pthread_mutex_unlock(&pool->mutex);

    pthread_cond_broadcast(&pool->cond);

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->cond);
}


void task_entry(struct Task *task) {
    int idx = *(int *)task->arg;
    printf("Task executed with index: %d\n", idx);
}

int main() {
    ThreadPool pool;
    nThreadPoolCreate(&pool);

    for (int i = 0; i < 1000; i++) {
        Task task;
        task.task_func = task_entry;
        task.arg = malloc(sizeof(int));
        *(int *)task.arg = i;
        nThreadPoolPushTask(&pool, task);
    }

    sleep(2); // wait for tasks to complete

    nThreadPoolDestroy(&pool);

    return 0;
}
