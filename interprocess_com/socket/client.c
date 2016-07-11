#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>


#define BUFFER_SIZE 1024
const char * UNIX_DOMAIN = "/tmp/UNIX.domain";


int
main(int argc, char **argv[])
{
    int ret, connect_sockfd;
    socklen_t server_len;
    /* create client socket */
    connect_sockfd = socket(PF_UNIX, SOCK_STREAM, 0);
    if(connect_sockfd < 0) {
	perror("client create socket failed.");
	return -1;
    }
    /* set server sockaddr_un */
    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, UNIX_DOMAIN);

    /* connect to server */
    server_len = sizeof(server_addr);
    ret = connect(connect_sockfd, (struct sockaddr*)&server_addr, server_len);
    if(ret < 0) {
	perror("connect to server failed!");
	close(connect_sockfd);
	unlink(UNIX_DOMAIN);
	return -1;
    }
    /* receive message from server */
    char snd_buf[BUFFER_SIZE] = {0};
    char rcv_buf[BUFFER_SIZE] = {0};
    int rcv_num = recv(connect_sockfd, rcv_buf, BUFFER_SIZE, MSG_WAITALL);
    printf("receive message from server (%d) :%s\n", rcv_num, rcv_buf);

    sprintf(snd_buf, "message form clinet (pid = %d)", getpid());

    int snd_num = send(connect_sockfd, snd_buf, BUFFER_SIZE, MSG_DONTROUTE);

    close(connect_sockfd);
    return 0;
}
