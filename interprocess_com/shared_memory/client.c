#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "common.h"


int main(int argc, char **argv)
{
    int shmid = 0, ret = 0;
    /* 获取共享内存 */
    shmid = shmget(MY_SHM_ID, 0, 0);
    if(shmid < 0) {
	perror("shmget failed.");
	return shmid;
    }
    //共享内存区段的挂载,脱离和使用
    //理解共享内存区段就是一块大内存 
    while(1) {
	/* 将共享内存连接到当前进程的地址空间 */
	void * shm = shmat(shmid, 0, 0);
	/* 像共享内存中写入数据 */
	strcpy((char *)shm, "this is a test string!");
	printf("sleep...\n");
	sleep(5);
	ret = shmdt(shm);
	printf("sleep...\n");
	sleep(5);
    }
    return 0;
}
