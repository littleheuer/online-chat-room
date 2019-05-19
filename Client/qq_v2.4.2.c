/*************************************************************************
	> File Name: qq.c
	> Author: 
	> Mail: 
	> Created Time: 2019年02月21日 星期四 18时07分42秒
 ************************************************************************/
#include "../common/common.h"

#ifdef _DEBUG
#define DBG(fmt, args...) printf(fmt, ##args)
#else
#define DBG(fmt, args...)
#endif

int sockfd;
    char sendbuf[200] = {0};
    char pathname[] = "./qq2.conf";
    char ip_addr[30] = {0}, temp[5] = {0}, name[30] = {0}, log_file[40] = {0};

void logout(int sig) {
    freopen(log_file, "a+", stdout);
    printf("\033[43m您已退出登录,我们下次见!\033[0m\n");
    fclose(stdout);
    _exit(0);
}

//port = atoi(argv[1]);
int main() {
    
    int server_port;
    get_conf_value(pathname, "Server_Ip", ip_addr);
    
    get_conf_value(pathname, "Server_Port", temp);
    server_port = atoi(temp); 
    int client_port;
    get_conf_value(pathname, "Client_Port", temp);
    client_port = atoi(temp);
    get_conf_value(pathname, "My_Name", name);
    get_conf_value(pathname, "Log_File", log_file);
    printf("(pathname)=%s,(ip)=%s,(name)=%s,(log_file)=%s(server)=%d(client)=%d\n", pathname, ip_addr, name, log_file, server_port, client_port);
    sockfd = socket_connect(server_port, ip_addr);
    printf("连接主机中...\n");
    if(sockfd < 0) {
        printf("socket failed\n");
        exit(1);
    } else {
        printf("连接成功\n");
    }

    int pid = fork();
    if(pid == 0) {
        
        send(sockfd, name, 100, 0);
        //发送信息
        while(1) {
            printf("send:");
            scanf("%[^\n]s", sendbuf);
            getchar();
            send(sockfd, sendbuf, strlen(sendbuf) + 1, 0);
            memset(sendbuf, 0, sizeof(sendbuf));
        }
    } else {
        signal(SIGINT, logout);
        //监听端口8732,套接字c_fd
        int c_fd = socket_create(client_port);
        int s_fd, iDataNum;
        if(c_fd < 0) {
            printf("server listen failed\n");
            return -1;
        } else {
            printf("监听成功\n");
        }
        //char *temp = (char *)malloc(sizeof(rmsg));
        struct sockaddr_in c_addr;
        int len = sizeof(c_addr);
        //循环接受连接connect
        while(1) {
            //memset(temp, 0, sizeof(temp));
            Message rmsg;
            iDataNum = recv(sockfd, &rmsg, sizeof(rmsg), 0);                
            if(iDataNum == 0 || iDataNum == -1) {
                close(s_fd);
                break;
            }
            //memcpy(&rmsg, temp, sizeof(temp));
            printf("(from)%s:(flag=%d)(msg)%s\n", rmsg.from, rmsg.flag, rmsg.message);
            if(write_file(&rmsg, log_file)) {
                printf("write failed\n");
                exit(-1);
            }
        }
        close(sockfd);
    }
    return 0;
}
