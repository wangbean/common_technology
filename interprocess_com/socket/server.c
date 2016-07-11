#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define BUFFER_SIZE 1024
const char * UNIX_DOMAIN = "/tmp/UNIX.domain";

/* AF表示 ADDRESS FAMILY 地址族 */
/* PF标识 PROTOCL FAMILY 协议族 */
/* 建议:对于socketpair(双工管道)与socket的domain参数,使用PF_LOCAL系列,
   而在初始化套接口地址结构时,则使用AF_LOCAL.例如:
   z = socket(PF_LOCAL, SOCK_STREAM, 0);
   adr_unix.sin_family = AF_LOCAL;
 */

int
main(int argc, char **argv)
{
    int ret, server_sockfd, client_sockfd;
    socklen_t server_len, client_len;
    /* create socket to bind local IP and Port */
    server_sockfd = socket(PF_UNIX, SOCK_STREAM, 0);
    if(server_sockfd < 0) {
	perror("can't create communication socket!");
	return -1;
    }
    /* create local IP and PORT */
    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, UNIX_DOMAIN, sizeof(server_addr.sun_path)-1);
    /*删除原有server_socket对象*/
    unlink(UNIX_DOMAIN);
    /* bind sockfd and sockaddr */
    server_len = sizeof(server_addr);
    ret = bind(server_sockfd, (struct sockaddr*)&server_addr, server_len);
    if(ret < 0) {
	perror("can't bind local sockaddr!");
	close(server_sockfd);
	unlink(UNIX_DOMAIN);
	return ret;
    }
    /* 监听网络,队列数为5 */
    ret = listen(server_sockfd, 5);
    if(ret < 0) {
	perror("can't listen client connect request");
	close(server_sockfd);
	unlink(UNIX_DOMAIN);
	return ret;
    }
    printf("Server is waiting for client connect...\n");
    struct sockaddr_un client_addr;
    client_len = sizeof(client_addr);
    char snd_buf[BUFFER_SIZE] = {0};
    char rcv_buf[BUFFER_SIZE] = {0};
    while(1) {
	client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_len);
	if(client_sockfd < 0) {
	    perror("can't listen client connect request");
	    close(server_sockfd);
	    unlink(UNIX_DOMAIN);
	    return -1;
	}
	printf("received a connection\n");
	/* 收发数据 */
	sprintf(snd_buf, "connect successed!");
	int snd_num = send(client_sockfd, snd_buf, BUFFER_SIZE, MSG_DONTROUTE);
	printf("send message (%d) : (%s) to client.\n", snd_num, snd_buf);
	memset(rcv_buf, 0, BUFFER_SIZE);
	int rcv_num = recv(client_sockfd, rcv_buf, BUFFER_SIZE, MSG_WAITALL);

	printf("Message form client (%d) : %s\n", rcv_num, rcv_buf);
    }
    close(server_sockfd);
    close(client_sockfd);
    unlink(UNIX_DOMAIN);
    return 0;
}
