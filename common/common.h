/*************************************************************************
	> File Name: common.h
	> Author: 
	> Mail: 
	> Created Time: 2019年02月23日 星期六 10时59分17秒
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H
#include<stdio.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>    
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include <sys/prctl.h>

#include <pthread.h>
#define ETH_NAME "wlp2s0"
#define DE_PORT 8731
#define LI_PORT 8732


//flag为1为私聊信息，0为公聊信息，2则为服务器的通知
typedef struct Message {
    char from[20];
    int flag;
    char message[1024];
} Message;

typedef struct Node {
    int count;
    char *ip, *name;
    struct Node *next;
} Node, *Linkedlist;

int get_conf_value(char *,const char *, char *);
int write_file(Message *, char *);
int fwrite_file(Message *, char *);

Linkedlist init(Linkedlist);
Node* create_node(Node *);
Node* init_node(Node *, int, char *, char *);
Linkedlist insert(Linkedlist, Node *);
Linkedlist dele_name(Linkedlist, Node *);
void clear(Linkedlist);
int search_ip(Linkedlist,char *);
int search_name(Linkedlist, char *);
void output(Linkedlist);

int init_msg(Message *, char *, int, char *);
int send_msg(Linkedlist, Message *);

int socket_create(int);
int socket_connect(int , char *);
char* get_ip(char *);


#endif
