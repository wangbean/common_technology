/*共享内存允许两个或多个进程进程共享同一块内存(这块内存会映射到各个进程自己独立的地址空间) 
  从而使得这些进程可以相互通信。 
  在GNU/Linux中所有的进程都有唯一的虚拟地址空间，而共享内存应用编程接口API允许一个进程使 
  用公共内存区段。但是对内存的共享访问其复杂度也相应增加。共享内存的优点是简易性。 
  使用消息队列时，一个进程要向队列中写入消息，这要引起从用户地址空间向内核地址空间的一次复制， 
  同样一个进程进行消息读取时也要进行一次复制。共享内存的优点是完全省去了这些操作。 
  共享内存会映射到进程的虚拟地址空间，进程对其可以直接访问，避免了数据的复制过程。 
  因此，共享内存是GNU/Linux现在可用的最快速的IPC机制。 
  进程退出时会自动和已经挂接的共享内存区段分离，但是仍建议当进程不再使用共享区段时 
  调用shmdt来卸载区段。 
  注意，当一个进程分支出父进程和子进程时，父进程先前创建的所有共享内存区段都会被子进程继承。 
  如果区段已经做了删除标记(在前面以IPC——RMID指令调用shmctl)，而当前挂接数已经变为0， 
  这个区段就会被移除。 
*/
/*
  shmget(key_t __key, size_t __size, int __shmflg);
  创建一个新的共享内存区段
  取得一个共享内存区段的描述符 
  shmctl(int __shmid, int __cmd, struct shmid_ds *__buf);
  取得一个共享内存区段的信息 
  为一个共享内存区段设置特定的信息
  移除一个共享内存区段 
  shmat(int __shmid, const void *__shmaddr, int __shmflg);
  挂接一个共享内存区段 
  shmdt(const void *__shmaddr);
  于一个共享内存区段的分离 
*/
//getpagesize(  )
//创建一个共享内存区段，并显示其相关信息，然后删除该内存共享区
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "common.h"

int main(int argc, char **argv)
{
    /* 获取系统中的页面大小 */
    printf("page size = %d\n", getpagesize());
    /* 创建一个共享内存区段 */
    int shmid = 0, ret = 0;
    shmid = shmget(MY_SHM_ID, getpagesize() * SHARED_MEMORY_SIZE, 0666 | IPC_CREAT);
    /* 指定的大小必须是当前系统架构中页面大小的整数倍 */
    if(shmid < 0) {
	perror("shmid failed.");
	return shmid;
    }
    /* 获得一个内存区段的信息 */
    struct shmid_ds shmds;
    ret = shmctl(shmid, IPC_STAT, &shmds);
    if(ret == 0) {
	printf("Size of memory segment is %zu\n", shmds.shm_segsz);
	printf("Number of attaches %d\n", (int)shmds.shm_nattch);
    } else {
	printf("shmctl call failed\n");
    }
    /* 接收共享内存的消息 */
    
    char buf[BUFFER_SIZE] = {0};

    while(1) {
	void * shm = shmat(shmid, 0, 0);
	strcpy(buf, shm);
	printf("%s\n", buf);
	memset(shm, 0, strlen(buf)+1);
	ret = shmdt(shm);
	sleep(1);
    }
    /* 删除该共享内存 */
    ret = shmctl(shmid, IPC_RMID, 0);
    if(ret == 0) {
	printf("Shared memory removed\n");
    } else {
	printf("Shared memory remove failed\n");
    }
}
