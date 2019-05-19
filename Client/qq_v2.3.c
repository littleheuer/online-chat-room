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

//port = atoi(argv[1]);
int main() {
    int sockfd;
    char sendbuf[200];
    memset(sendbuf, 0, sizeof(sendbuf));
    char pathname[] = "./qq.conf";
    char ip_addr[30], temp[30], name[30], log_file[40];
    memset(ip_addr, 0, sizeof(ip_addr));
    memset(temp, 0, sizeof(temp));
    memset(name, 0, sizeof(name));
    memset(log_file, 0, sizeof(log_file));
    int server_port = 0;
    get_conf_value(pathname, "Server_Ip", ip_addr);
    
    get_conf_value(pathname, "Server_Port", temp);
    server_port = atoi(temp); 
    get_conf_value(pathname, "My_Name", name);
    get_conf_value(pathname, "Log_File", log_file);
    printf("(pathname)=%s,(ip)=%s,(name)=%s,(log_file)=%s(server)=%d\n", pathname, ip_addr, name, log_file, server_port);
    //printf("(port)%d (ip)%s\n", client_port, ip_addr);
    printf("连接主机中...\n");
    
    sockfd = socket_connect(server_port, ip_addr);
    if(sockfd < 0) {
        printf("socket failed\n");
        exit(1);
    } else {
        printf("连接成功\n");
        //子进程监听
        int pid = fork();
        if(!pid) {
            //printf("2(port)%d (ip)%s\n", client_port, ip_addr);
            prctl(PR_SET_PDEATHSIG,SIGHUP); 
            close(sockfd);
            struct sockaddr_in c_addr;
            int addr_len = sizeof(c_addr);
            bzero(&addr_len, sizeof(c_addr));
            //监听端口8732,套接字c_fd
            int client_port = 0;
            get_conf_value(pathname, "Client_Port", temp);
            client_port = atoi(temp);
            printf("(c_fd create)%d\n", client_port);
            int c_fd = socket_create(8732);
            int s_fd, iDataNum;
            if(c_fd < 0) {
                printf("server listen failed\n");
                return -1;
            }
            //循环接受连接connect
            while(1) {
                printf("(3port)%d (ip)%s\n", client_port, ip_addr);
                //分身s_fd
                s_fd = accept(c_fd, (struct sockaddr*) &c_addr, (socklen_t*) &addr_len);
                if(s_fd < 0) {
                    perror("accept\n");
                    continue;
                }
                //孙子进程接受分身的信息
                int pid_s = fork();
                if(!pid_s) {
                    Message *rmsg = (Message*)malloc(sizeof(Message));
                    prctl(PR_SET_PDEATHSIG,SIGHUP); 
                    close(c_fd);
                    while(1){
                        //memset(recvMessage, 0, sizeof(recvMessage));
                        iDataNum = recv(s_fd, rmsg, sizeof(rmsg), 0);
                        
                        if(iDataNum < 0) {
                            printf("recv NULL\n");
                            continue;
                        }
                        if(iDataNum == 0 || iDataNum == -1) {
                            close(s_fd);
                            break;
                        }
                        printf("%s\n", rmsg->message);
                        if(write_file(rmsg, log_file)) {
                            printf("write failed\n");
                            exit(-1);
                        }
                    }
                    free(rmsg);
                }
            }
            close(c_fd);
        }
    }
    //发送名字
    send(sockfd, name, 100, 0);
    //发送信息
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
