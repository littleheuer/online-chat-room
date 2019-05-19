/*************************************************************************
	> File Name: linkedlist.c
	> Author: 
	> Mail: 
	> Created Time: 2019年02月26日 星期二 15时06分10秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Node {
    int count;
    char *ip;
    struct Node *next;
} Node, *Linkedlist;

Linkedlist init(Linkedlist head) {
    head = (Linkedlist)malloc(sizeof(Linkedlist));
    head->count = 0;
    head->ip = (char*)malloc(sizeof(char) * 10);
    head->next = NULL;
    return head;
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

Linkedlist dele(Linkedlist head, Node *node) {
    if(head == NULL) {
        printf("dele NULL\n");
        return head;
    }
    Node *p = head->next, *del = head->next->next;
    if(0 == strcmp(p->ip, node->ip)) {
        head->next = p->next;
        free(p);
        head->count--;
        return head;
    }
    while(p->next != NULL) {
        del = p->next;
        if(0 == strcmp(del->ip, node->ip)) {
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
        free(del);
        del = head;
    }
    del = NULL;
    head = NULL;
    return ;
}

int search(Linkedlist head, char *ip) {
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

void output(Linkedlist head) {
    if(head == NULL) {
        return ;
    }
    Node *p = head->next;
    while(p) {
        printf("%s ", p->ip);
        p = p->next;
    }
    printf("\n");
    p = NULL;
    return ;
}

int main() {
    Linkedlist head = NULL;
    head = init(head);
    for(int i = 0; i < 4; i++) {
        Node *node = (Node*)malloc(sizeof(Node));
        node->ip = (char*)malloc(sizeof(char) * 100);
        scanf("%s", node->ip);
        head = insert(head, node);
    }
    printf("%d\n", head->count);
    output(head);
    Node *del = (Node*)malloc(sizeof(Node));
    del->ip = (char*)malloc(sizeof(char) * 100);
    scanf("%s", del->ip);
    head = dele(head, del);
    output(head);
    char temp[10] = {0};
    scanf("%s", temp);
    printf("temp=%s\n", temp);
    if(search(head, temp) == 0) {
        printf("在线\n");
    } else {
        printf("不在\n");
    }
    free(del->ip);
    free(del);
    output(head);
    clear(head);
    return 0;
}
