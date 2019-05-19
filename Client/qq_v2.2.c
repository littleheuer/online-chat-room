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

        int pid = fork();
        if(!pid) {
            close(sockfd);
            struct Message recvMessage;
            struct sockaddr_in c_addr;
            int s_fd = sockect_create(LI_PORT);
            int c_fd, iDataNum;
            if(server_fd < 0) {
                printf("server listen failed\n");
                return -1;
            }
            while(1) {
                c_fd = accept(s_fd, (struct sockaddr*) &c_addr, (socklen_t*) &addr_len);
                if(cfd < 0) {
                    perror("accept\n");
                    continue;
                }
                while(1) {
                    iDataNum = recv(c_fd, &recvMessage, sizeof(recvMessage), 0);
                    if(iDataNum < 0) {
                        printf("recv NULL\n");
                        continue;
                    }
                    if(iDataNum == 0|| iDataNum == -1) {
                        close(cfd);
                        exit(0);
                    }
                    printf("%s\n", recvMessage.message);
                    //system("tail -f ./")
                    switch(recvMessage.flag) {
                        case 1 : {
                            fprintf(fp, "(私聊)%s\n", recvMessage.message);
                            break;
                        }
                        case 0 : {
2qaqa22qa<F2>
                            fprintf(fp, "(公聊)%s\n", recvMessage.message);
                            break;
                        }
                        case 2 : {
                            fprintf(fp, "(message)%s\n", recvMessage.message);
                            break;
                        }
                        default : {
                            printf("flag error\n");
                        }
                    }
                }
            }
        }
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
