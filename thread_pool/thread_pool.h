#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <pthread.h>

typedef void* (*callback_t)(void *);

/* 需要执行的任务链表 */
typedef struct tpool_task {
    callback_t routine;		/* 线程函数 */
    void  *arg;			/* 线程传入参数 */
    struct tpool_task *next;	/* 指向下一个任务 */
} tpool_task_t;

/* 线程池 */
typedef struct tpool {
    int max_thr_num;            /* 最大线程数 */
    tpool_task_t *queue_head;   /* 线程链表头 */
    pthread_mutex_t queue_lock;
    pthread_cond_t queue_ready;
} tpool_t;

/* 
 * @brief 创建线程池
 * @param max_thr_num 最大线程数
 * @return 线程池指针 NULL: 失败
 */
tpool_t*
tpool_create(int max_thr_num);


/* 
 * @brief 销毁线程池
 * @param tpool 要销毁的线程池指针
 */
void
tpool_destroy(tpool_t *tpool);

/* 
 * @brief 向线程中添加任务
 * @param tpool 线程池指针
 * @param routine 线程函数指针
 * @param arg 函数入参
 * @return 0: 成功 其它: 失败
 */
int
tpool_task_add(tpool_t *tpool, void* (*routine)(void*), void *arg);


#endif
