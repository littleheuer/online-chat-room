/*************************************************************************
	> File Name: common.c
	> Author: 
	> Mail: 
	> Created Time: 2019年02月23日 星期六 11时00分57秒
 ************************************************************************/

#include "common.h"

#ifdef _DEBUG
#define DBG(fmt, args...) printf(fmt, ##args)
#else
#define DBG(fmt, args...)
#endif
//创建具有监听状态的socket套接字，监听port端口
int socket_create(int port) {
    printf("(func out)%d\n", port);
    struct sockaddr_in s_addr;
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0) {
        perror("socket");
        return -1;
    }
    bzero(&s_addr, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(port);
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sfd, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
        perror("bind");
        close(sfd);
        return -1;
    }
    if(listen(sfd, 30) < 0) {
        perror("listen");
        close(sfd);
        return -1;
    }
    return sfd;
}
//连接服务器connect
int socket_connect(int port, char *host) {
    int sockfd = 0;
    struct sockaddr_in serverAddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("socket");
    }
    //s_addr.sin_addr = htonl(INADDR_ANY)
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(host);
    //bzero(&(serverAddr.sin_zero), sizeof(serverAddr));
    if(connect(sockfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0) {
        printf("%d\n" , sockfd);
        perror("connect");
        close(sockfd);
        return -1;
    }
    return sockfd;
}
//获取本机wlan的ip地址
char *get_ip(char *ip) {
    int sock;
    struct sockaddr_in sin;
    struct ifreq ifr;
    char *temp_ip = NULL;
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        perror("socket");
        return NULL;                
    }
    strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ-1);
    ifr.ifr_name[IFNAMSIZ - 1] = 0;
    if (ioctl(sock, SIOCGIFADDR, &ifr) < 0)
    {
        perror("ioctl");
        return NULL;
    }
    memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
    temp_ip = inet_ntoa(sin.sin_addr);
    strcpy(ip,temp_ip);
    fprintf(stdout, "服务器ip地址(wlp2s0): %s\n", temp_ip);
}

//获取配置文件的配置
int get_conf_value(char *pathname,const char *key_name, char *value) {
    FILE *fd = NULL;
    ssize_t read = 0;
    size_t len = 0;
    char *line = NULL;
    char *substr = NULL;
    memset(value, 0, sizeof(value));
    fd = fopen(pathname, "r");
    if(fd == NULL) {
        printf("fopen error\n");
        exit(1);
    }
    while((read = getline(&line, &len, fd)) != 1) {
        DBG("(read=%d)%s", (int)read, line);
        substr = strstr(line, key_name);
        if(substr == NULL) {
            continue;
        } else {
            int temp = strlen(key_name);
            if(line[temp] == '=') {

                DBG("(value)%s(temp)%d(read)%d\n", value, temp, read);
                strncpy(value, &line[temp + 1], (int)read - temp - 1);
                temp = strlen(value);
                DBG("temp=%d\n", temp);
                *(value + temp - 1) = '\0';
                fflush(stdin);
                break;
            } else {
                printf("Next\n");
                continue;
            }
        }
    }
    return 0;
}
//freopen 方式写入文件
int write_file(Message *msg, char *log_file) {
    freopen(log_file, "a+", stdout);
    switch(msg->flag) {
        case 0 : {
            printf("\033[34m%s\033[0m%s\n", msg->from, msg->message);
            break;
        }
        case 2 : {
            printf("\033[43m服务器通知消息:\033[0m%s\n", msg->message);
            break; 
        }
        case 1 :{
          printf("\033[34m%s\033[0m:%s  \033[5m * \033[0m\n", msg->from, msg->message);
            break;
        }
        default : {
            printf("flag error\n");
            fclose(stdout);
            return 1;
        }
    }
    fclose(stdout);
    return 0;
}
//fopen方式写入文件
int fwrite_file(Message *msg, char *log_file) {
    FILE *fp = NULL;
    fp = fopen(log_file, "a+");
    if(fp == NULL) {
        printf("open file fialed\n");
        return 1;
    }
    switch(msg->flag) {
        case 0 : {
            fprintf(fp, "\033[34m%s\033[0m%s\n", msg->from, msg->message);
            printf("\033[34m%s:\033[0m%s\n", msg->from, msg->message);
            break;
        }
        case 2 : {
            fprintf(fp, "\033[43m服务器通知消息:\033[0m%s\n", msg->message);
            printf("\033[43m服务器通知消息:\033[0m%s\n", msg->message);
            break; 
        }
        case 1 :{
            fprintf(fp, "\033[34m%s\033[0m:%s\033[5m * \033[0m\n", msg->from, msg->message);
            printf("\033[34m%s\033[0m:%s\033[5m * \033[0m\n", msg->from, msg->message);
            break;
        }
        default : {
            printf("flag error\n");
            //fclose(fp);
        }
    }
    fclose(fp);
    return 0;
}

//链表
Linkedlist init(Linkedlist head) {
    head = (Linkedlist)malloc(sizeof(Linkedlist));
    head->count = 0;
    head->ip = (char*)malloc(sizeof(char) * 30);
    head->name = (char*)malloc(sizeof(char) * 30);
    head->next = NULL;
    return head;
}

Node* create_node(Node *node) {
    node = (Node*)malloc(sizeof(Node));
    node->count = 0;
    node->ip = (char*)malloc(sizeof(char) * 30);
    node->name = (char*)malloc(sizeof(char) * 30);
    node->next = NULL;
    return node;
}

Node* init_node(Node *node, int count, char *name, char *ip) {
    if(node == NULL) {
        printf("init node failed\n");
        return node;
    }
    node->count = count;
    strcpy(node->name, name);
    strcpy(node->ip, ip);
    return node;
}

Linkedlist insert(Linkedlist head, Node *node) {
    if(head == NULL) {
        printf("head NULL\n");
        return head;
    }
    node->next = head->next;
    head->next = node;
    head->count++;
    return head;
}

Linkedlist dele_name(Linkedlist head, Node *node) {
    if(head == NULL) {
        printf("dele NULL\n");
        return head;
    }
    Node *p = head->next, *del = head->next->next;
    if(0 == memcmp(p, node, sizeof(p))) {
        head->next = p->next;
        free(p);
        head->count--;
        return head;
    }
    while(p->next != NULL) {
        del = p->next;
        if(0 == memcmp(del, node, sizeof(del))) {
            p->next = del->next;
            free(del);
            del = NULL;
            return head;
        }
        p = del;
    }
    printf("删除失败\n");
    return head;
}

void clear(Linkedlist head) {
    if(head == NULL) {
        return ;
    }
    Node *del = head;
    while(head) {
        head = head->next;
        free(del->name);
        free(del->ip);
        free(del);
        del = head;
    }
    del = NULL;
    head = NULL;
    return ;
}

int search_ip(Linkedlist head, char *ip) {
    if(head == NULL) {
        return 1;
    }
    Node *p = head->next;
    while(p) {
        if(0 == strcmp(p->ip, ip)) {
            return 0;
        }
        p = p->next;
    }
    return 1;
}

int search_name(Linkedlist head, char *name) {
    if(head == NULL) {
        return 1;
    }
    Node *p = head->next;
    while(p) {
        if(0 == strcmp(p->name, name)) {
            return p->count;
        }
        p = p->next;
    }
    return -1;
}

void output(Linkedlist head) {
    if(head == NULL) {
        return ;
    }
    Node *p = head->next;
    while(p) {
        //printf("ip=%s name=%s\n", p->ip, p->name);
        p = p->next;
    }
    p = NULL;
    return ;
}

int init_msg(Message *msg, char *from, int flag, char *message) {
    if(msg == NULL) return 1;
    strcpy(msg->from, from);
    printf("(init_msg)%s\n", message);
    strcpy(msg->message, message);
    msg->flag = flag;
    return 0;
}

int send_msg(Linkedlist head, Message *smsg) {
    //char *temp = (char*)malloc(sizeof(smsg));
    //memset(temp, 0, sizeof(temp));
    //memcpy(temp, smsg, sizeof(smsg));
    switch(smsg->flag) {
        case 0 : {
            if(!head->count) {
                printf("无用户在线\n");
                break;
            }
            Node *p = head->next;
            while(p) {
                printf("0(send_msg)=%s\n", smsg->message);
                send(p->count, (char*)smsg, sizeof(smsg), 0);
                printf("send->%s\n", p->name);
                p = p->next;
            }
            p = NULL;
            break;
        }
        case 1 : {
            int fd;
            printf("1(send_msg)%s\n", smsg->message);
            if(smsg->message[0] == '@') {
                char *p = NULL, b_copy[1024], s_name[50];
                strcpy(b_copy, smsg->message);
                p = strtok(b_copy, " ");
                strcpy(s_name, p + 1);
                p = strtok(NULL, "\n");
                strcpy(smsg->message, p);
                p = NULL;
                fd = search_name(head, s_name);
                if(fd < 0) {
                    strcpy(smsg->message, "用户未在线");
                    fd = search_name(head, smsg->from);
                    //memset(temp, 0, sizeof(temp));
                    //memcpy(temp, smsg, sizeof(smsg));
                    send(fd, (char*)smsg, sizeof(smsg), 0);
                    break;
                }
                printf("%s->%s\n", smsg->from, s_name);
                send(fd, (char*)smsg, sizeof(smsg), 0);
                break;
            } else if(strcmp(smsg->message, "#") == 0) {
                fd = search_name(head, smsg->from);
                send(fd, (char*)smsg, sizeof(smsg), 0);
                break;
            } else {
                printf("flag=1,error\n");
                break;
            }
        }
        case 2 : {
            if(!head->count) {
                printf("无用户在线\n");
                break;
            }
            Node *p = head->next;
            while(p) {
                send(p->count, (char*)smsg, sizeof(smsg), 0);
                printf("send->%s\n", p->name);
                p = p->next;
            }
            p = NULL;
            break;
        }
        default : {
            printf("flag error\n");
        }
    }
}
