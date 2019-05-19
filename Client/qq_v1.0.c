/*************************************************************************
	> File Name: qq.c
	> Author: 
	> Mail: 
	> Created Time: 2019年02月21日 星期四 18时07分42秒
 ************************************************************************/


#include "../common/common.h"
#define DE_PORT 8731

int main() {
    int sockfd;
    char sendbuf[200];
    char ip[50] = "192.168.10.210";
    sockfd = socket_connect( DE_PORT, ip);
    printf("连接主机中...\n");
    char host[1000] = {0};
    gethostname(host, sizeof(host));
    //char name[100] = {0};
    send(sockfd,"张全蛋", 20, 0);
    while(1) {
        memset(sendbuf, 0, sizeof(sendbuf));
        printf("send:");
        scanf("%[^\n]s", sendbuf);
        getchar();
        printf("\n");
        if(strcmp(sendbuf, "quit") == 0) {
            break;
        }
        send(sockfd, sendbuf, strlen(sendbuf), 0);
    }
    close(sockfd);
    return 0;
}
