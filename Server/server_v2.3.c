/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2019年02月21日 星期四 19时01分19秒
 ************************************************************************/
#include"../common/common.h"
#define SERVER_PORT 8731

int main() {
    Linkedlist list = NULL;
    list = init(list);
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
            Node *user = NULL;
            user = create_node(user);
            
            //关闭子进程的监听
            close(sfd);
            memset(buffer, 0, sizeof(buffer));
            char name[200];
            iDataNum = recv(cfd, name, sizeof(name), 0);
            name[iDataNum] = '\0';
            
            char ip[30];
            strcpy(ip, inet_ntoa(c_addr.sin_addr));
            user = init_node(user, cfd, name, ip);
            list = insert(list, user);
            printf("用户\033[36;5m[%s]\033[0;0m,ip为%s从%d端口进入直播间\n", name,inet_ntoa(c_addr.sin_addr), htons(c_addr.sin_port));
            
            Message rmsg;
            char welcome[100];
            //回复用户登录情况
            sprintf(welcome, "@%s 您已进入直播间，快来和大家搞基吧\n", name);
            init_msg(&rmsg, name, 1, welcome);
            send_msg(list, &rmsg);
            sprintf(welcome, "%s进入直播间，大家欢迎\n", name);
            init_msg(&rmsg, name, 2, welcome);
            send_msg(list, &rmsg);
            while(1) {
                memset(buffer, 0, sizeof(buffer));
                buffer[0] = '\0';
                iDataNum = recv(cfd, buffer, sizeof(buffer), 0);
                if(iDataNum < 0) {
                    printf("recv NULL\n");
                    continue;
                }
                buffer[iDataNum] = '\0';
                
                if(iDataNum == 0 || iDataNum == -1) {
                    printf("\033[31;1m[%s]退出直播间\033[0;0m\n", name);
                    list = dele_name(list, user);
                    sprintf(welcome, "%s已退出直播间\n", name);
                    init_msg(&rmsg, name, 2, welcome);
                    send_msg(list, &rmsg);
                    close(cfd);
                    exit(0);
                }
                if(buffer[0] == '@') {
                    printf("%s\n", rmsg.message);
                    init_msg(&rmsg, name, 1, buffer);
                    send_msg(list, &rmsg);
                } else if(strcmp(buffer, "#") == 0) {
                    printf("%s查询了在线人数\n", name);
                    char sum[50];
                    sprintf(sum, "%d人在线\n", list->count);
                    init_msg(&rmsg, name, 1, sum);
                    send_msg(list, &rmsg);
                }else {
                    init_msg(&rmsg, name, 0, buffer);
                    send_msg(list, &rmsg);
                }
            }
            //close(cfd);
            }
        else {
            continue;
        }
    }
    close(sfd);
    clear(list);
    return 0;
}
