/*************************************************************************
	> File Name: qq.c
	> Author: 
	> Mail: 
	> Created Time: 2019年02月21日 星期四 18时07分42秒
 ************************************************************************/
#include "../common/common.h"

//port = atoi(argv[1]);
int main(int argc, char *argv[]) {
    if(argc != 4) {
        printf("(argc)%d 格式:./qq2.c port ip name\n", argc);
        exit(1);
    }
    int sockfd;
    char sendbuf[200];
    memset(sendbuf, 0, sizeof(sendbuf));
    char *ip_addr = argv[2];
    int port = atoi(argv[1]);
    char *name = argv[3];
    printf("(port)%d (ip)%s\n", port, ip_addr);
    printf("连接主机中...\n");
    sockfd = socket_connect(port, ip_addr);
    if(sockfd < 0) {
        printf("socket failed\n");
        exit(1);
    } else {
        printf("连接成功\n");
    }
    //char host[1000] = {0};
    //gethostname(host, sizeof(host));
    send(sockfd, name, 100, 0);
    while(1) {
        printf("send:");
        memset(sendbuf, 0, sizeof(sendbuf));
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
