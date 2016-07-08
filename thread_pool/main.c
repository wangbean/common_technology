
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "thread_pool.h"
/* 测试代码 */

void*
func(void *arg)
{
    size_t i = *(size_t *)arg;
    while(i--) {
	printf("thread run num = %zu\n", *(size_t *)arg);
	sleep(1);
    }
    return NULL;
}

int
main(int argc, char **argv)
{
    printf("main start...........\n");
    tpool_t *tpool = tpool_create(3);
    if(tpool == NULL) {
	printf("tpool_create failed...\n");
	exit(1);
    }
    size_t i = 10;
    size_t a[10];
    for(i = 0; i < 10; i++) {
	a[i] = i;
	tpool_task_add(tpool, func, (void*)&(a[i]));
    }
    tpool_destroy(tpool);
    printf("main end...........\n");
    return 0;
}
