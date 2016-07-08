#include <stdio.h>
#include <stdlib.h>
#include "thread_pool.h"
/*
 * 创建线程池
 */
tpool_t*
tpool_create(int max_thr_num)
{
    int i;
    tpool_t *tpool = calloc(1, sizeof(tpool_t));
    if(!tpool) {
	return NULL;
    }
}













