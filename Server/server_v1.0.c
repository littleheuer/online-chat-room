/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2019年02月21日 星期四 19时01分19秒
 ************************************************************************/
#include"../common/common.h"
#define SERVER_PORT 8731

int main() {
    int sfd;
    struct sockaddr_in c_addr;
    int addr_len = sizeof(c_addr);
    char buffer[200];
    int iDataNum;
    char *ip = (char *)malloc(sizeof(char) * 1024);
    memset(ip, 0, sizeof(ip));
    ip = get_ip(ip);
    //初始化
    sfd = socket_create(SERVER_PORT);
    if(sfd < 0) {
        return -1;
    }
    int cfd;
    printf("\033[32;1m欢迎来到小小白的直播间!\033[0;0m\n");
    while(1) {
        printf("端口：%d\n", SERVER_PORT);
        cfd = accept(sfd, (struct sockaddr*) &c_addr, (socklen_t*) &addr_len);
        if(cfd < 0) {
            perror("accpet");
            continue;
        }
        
        int pid = fork();
        if(!pid) {
            //关闭子进程的监听
            close(sfd);
            memset(buffer, 0, sizeof(buffer));
            char name[200];
            memset(name, 0, sizeof(name));
            iDataNum = recv(cfd, name, sizeof(name), 0);
            name[iDataNum] = '\0';
            printf("用户\033[36;5m[%s]\033[0;0m,ip为%s从%d端口进入直播间\n", name,inet_ntoa(c_addr.sin_addr), htons(c_addr.sin_port));
            while(1) {
                memset(buffer, 0, sizeof(buffer));
                buffer[0] = '\0';
                iDataNum = recv(cfd, buffer, sizeof(buffer), 0);
                if(iDataNum < 0) {
                    printf("recv NULL\n");
                    continue;
                }
                //printf("srting:%d %s\n", iDataNum, buffer);
                //buffer[iDataNum] = '\0';
                if(iDataNum == 0 || iDataNum == -1) {
                    printf("\033[31;1m[%s]退出直播间\033[0;0m\n", name);
                    close(cfd);
                    exit(0);
                }
                printf("\033[33;1m%s:\033[0;0m:%s\n", name, buffer);
            }
            //close(cfd);
            }
        else {
            continue;
        }
    }
    close(sfd);
    return 0;
}
