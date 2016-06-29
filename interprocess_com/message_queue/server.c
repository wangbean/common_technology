#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include "common.h"


typedef struct msgbuffer {
    long mtype;
    char mtext[BUFFER_SIZE];
} msgbuf_st;

int main(int argc, char **argv)
{
    /* 保证取互不干扰的IPC键值 */
    key_t key = ftok((char *)getenv("HOME"), 1);
    if(key < 0 ) {
	perror("ftok failed.");
	return key;
    }
    /* 创建消息队列 */
    int msqid = msgget(key, 0644 | IPC_CREAT);
    if(msqid < 0) {
	perror("msgget Initialization failed.");
	return msqid;
    }
    msgbuf_st bufrecv;
    /* msgtype可以实现一种简单的接收优先级。如果msgtype为0，就获取队列中的第一个消息。
       如果它的值大于零，将获取具有相同消息类型的第一个信息。
       如果它小于零，就获取类型等于或小于msgtype的绝对值的第一个消息。*/
    while(1) {
	msgrcv(msqid, &bufrecv, sizeof(bufrecv), 0, 0);
	printf("Reply : %s \n", bufrecv.mtext);
    }
    int ret = msgctl(msqid, IPC_RMID, NULL);
    if(ret < 0) {
	perror("msgctl failed");
	return ret;
    }
    return 0;
}
