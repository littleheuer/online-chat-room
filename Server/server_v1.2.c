/*************************************************************************
	> File Name: server_v1.1.c
	> Author: 
	> Mail: 
	> Created Time: 2019年02月28日 星期四 09时10分08秒
 ************************************************************************/

#include<stdio.h>
#include "../common/common.h"
#include "DEBUG.h"
typedef struct user {
    int fd;
    char name[20];
    struct user *next;
} User, *List;

List list;
int sum = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



List add(List head, User *user) {
    pthread_mutex_lock(&mutex);
    User *p = head;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = user;
    sum++;
    pthread_mutex_unlock(&mutex);
    return head;
}


void dele(List head, char *name) {
    pthread_mutex_lock(&mutex);
    User *p = head, *dele = NULL;
    while(p->next != NULL && strcmp(p->next->name, name)) {
        p = p->next;
    }
    if(p->next == NULL) {
        return ;
    }
    dele = p->next;
    p->next = p->next->next;
    free(dele);
    dele = NULL;
    p = NULL;
    sum--;
    pthread_mutex_unlock(&mutex);
    return ;
}

void *print(void *) {
    List temp = NULL;
    //if(temp->next == NULL) {
    //    printf("<---当前无用户--->\n");
    //}
    while(1) {
        sleep(30);
        pthread_mutex_lock(&mutex);
        temp = list;
        fflush(stdout);
        printf("<...........>\n");
        while (temp->next != NULL) {
            printf("%s->%d(fd)\n", temp->next->name, temp->next->fd);
            temp = temp->next;
        }
        printf("<----------->\n");
        pthread_mutex_unlock(&mutex);
    }
}

int check(char *name) {
    pthread_mutex_lock(&mutex);
    User *temp = list;
    while(temp->next != NULL) {
        if(strcmp(temp->next->name, name) == 0) {
            return 1;
        }
        temp = temp->next;
    }
    pthread_mutex_unlock(&mutex);
    return 0;
}

int check_fd (char *name) {
    pthread_mutex_lock(&mutex);
    User *temp = list;
    while(temp->next != NULL) {
        if(strcmp(temp->next->name, name) == 0) {
            return temp->next->fd;
        }
        temp = temp->next;
    }
    pthread_mutex_unlock(&mutex);
    return 0;
}

void send_to (char *name, Message msg) {
    int fd = check_fd(name);
    send(fd, (char *)&msg, sizeof(msg), 0);
}

void send_all (List head, Message msg) {
    pthread_mutex_lock(&mutex);
    List temp = head;
    while(temp->next != NULL) {
        send_to(temp->next->name, msg);
        temp = temp->next;
    }
    pthread_mutex_unlock(&mutex);
}

void *addnode(void *argv) {
    User *p = (User*)argv;
    list = add(list, p);
    char welcome[1024];
    Message msg;
    sprintf(msg.message, "%s 您已登录服务器", p->name);
    msg.flag = 2;
    
    //send_to(p->name, msg);
    send_all(list, msg);
    char buffer[1024] = {0};
    while(recv(p->fd, buffer, sizeof(buffer), 0) > 0) {
        //Message msg;
        printf("(dbg)%s:%s\n", p->name, buffer);
        strcpy(msg.from, p->name);
        msg.flag = 0;
        strcpy(msg.message, buffer);
        if(buffer[0] == '#') {
            DBG("%s查询在线人数\n", p->name);
            msg.flag = 1;

            sprintf(msg.message, "在线人数:<%d> : ", sum);
            User *p = list;
            while (p->next) {
                p = p->next;
                strcat(msg.message, p->name);
                strcat(msg.message, " ");
            }
            strcat(msg.message,"\n");
            send_to(msg.from, msg);
        } else if(buffer[0] == '@') {
            msg.flag = 1;
            
            char to_name[20] = {0};
            char temp[1024] = {0};
            int i;
            for(i = 1; i < 20; i++) {
                if(buffer[i] == ' ') break;
            }
            strncpy(to_name, buffer + 1, i - 1);
            strcpy(temp, buffer + i + 1);
            strcpy(msg.message, temp);
            if(check(to_name)) {
                printf("发送私聊信息%s->%s\n", msg.from, to_name);
                send_to(to_name, msg);
            } else {
                printf("非法用户名\n");
                sprintf(msg.message, "非法用户名\n");
                send_to(msg.from, msg);
            }
        } else {
            send_all(list, msg);
        }
        memset(buffer, 0, sizeof(buffer));
    }
    printf("用户%s已下线\n", p->name);
    sprintf(msg.message, "用户%s已下线\n", p->name);
    dele(list, p->name);
    //提示其他用户已下线
    strcpy(msg.from, "服务器");
    msg.flag = 2;
    send_all(list, msg);
}

int main() {
    pthread_t print_t, addnode_t;
    int socketfd, server_port, server_listen;
    char temp_port[6] = {0};
    User *p = NULL;
    p = (User *)malloc(sizeof(User));
    strcpy(p->name, "init");
    p->next = NULL;
    list = p;

    pthread_create(&print_t, NULL, print, NULL);

    char config[] = "./qq.sh";

    get_conf_value(config, "Server_Port", temp_port);
    
    server_port = atoi(temp_port);

    if((server_listen = socket_create(server_port)) < 0) {
        printf("error in create socket");
        return 1;
    }
    
    while(1) {
        char name[20] = {0};

        if((socketfd = accept(server_listen, NULL, NULL)) < 0) {
            DBG("%s", strerror(errno));
            return 1;
        }
        recv(socketfd, name, sizeof(name), 0);
        
        if(!check(name)){

            User *user;
            user = (User *)malloc(sizeof(User));
            user->fd = socketfd;
            user->next = NULL;
            strcpy(user->name, name);
            pthread_create(&addnode_t, NULL, addnode, (void *)user);
        }


    }
    pthread_join(print_t, NULL);
    return 0;
}
