#include "../common/common.h"

char *config="./qq.conf";
char name[20] = {0};
int client_port, server_port;
char server_ip[20] = {0};
char buff[1024] = {0};
char log[100] = {0};


void logout(int sig) {
    freopen(log, "a+", stdout);
    printf("\033[43m您已退出登录, 我们下次见！\033[0m \n");
    fclose(stdout);
    _exit(0);
}

int main(int argc, char *argv[]) {
    
    
    int sock_fd;
    char temp_client[5] = {0};
    char temp_server[5] = {0};
    if (argc != 1) {
        printf("Usage: ./client\n");
        exit(1);
    }
   
    signal(SIGINT, logout);
    get_conf_value(config, "Server_Port", temp_server);
    get_conf_value(config, "My_Name", name);
    get_conf_value(config, "Log_File", log);
    get_conf_value(config, "Server_Ip", server_ip);
    client_port = atoi(temp_client);
    server_port = atoi(temp_server);

    if ((sock_fd = socket_connect(server_port, server_ip)) < 0) {
            printf("Error in connect!\n");
            return -1;
        }

    

    send(sock_fd, name, strlen(name) + 1, 0);
    
    int pid = 0;

    if ((pid = fork())< 0) {
        perror("fork");
        exit(1);
    }

    if (pid != 0) {
        struct Message recv_message;
        while (recv(sock_fd, (char *)&recv_message, sizeof(recv_message), 0) > 0) {
            freopen(log, "a+", stdout);
            if (recv_message.flag == 0) {
                printf("\033[34m%s\033[0m: %s\n", recv_message.from, recv_message.message);
            } else if (recv_message.flag == 2) {
                printf("\033[43m服务器通知消息: \033[0m %s\n", recv_message.message);
            } else {
                printf("\033[34m%s\033[0m: %s   \033[5m \033[32m * \033[0m\n", recv_message.from, recv_message.message);
            }

            fclose(stdout);

        }
        close(sock_fd);

    } else {
        
        // DBG("After connect!\n");
        //system("clear");
        while(1) {
            printf("请在以下的对话框输入信息，按回车发送\n");
            scanf("%[^\n]s", buff);
            getchar();
            int send_num = send(sock_fd, buff, strlen(buff), 0);
            if (send_num < 0) {
                perror("send");
            }
            memset(buff, 0, sizeof(buff));
            //system("clear");
        }
        close(sock_fd);
    }
    return 0;
}
