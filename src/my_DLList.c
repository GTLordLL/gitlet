#include <stdio.h>
#include <stdlib.h>
#include "my_DLList.h"

// 创建双向链表
DLList* create_dllist(void){
    DLList* list = (DLList*)malloc(sizeof(DLList));
    if (list == NULL){
        printf("DLList结构空间动态分配失败\n");
        return NULL;
    }

    list->sentinel = (DLListNode*)malloc(sizeof(DLListNode));
    if (list->sentinel == NULL){
        printf("Node数据空间动态分配失败\n");
        free(list);
        return NULL;
    }

    list->sentinel->prev = list->sentinel;
    list->sentinel->next = list->sentinel;

    list->curSize = 0;
    return list;
}

// 头插法
void add_head(DLList* list, int data1,int data2){
    DLListNode* newNode = (DLListNode*)malloc(sizeof(DLListNode));
    if (!newNode){
        printf("DLListNode数据空间动态分配失败\n");
        return;
    }
    newNode->data1 = data1;
    newNode->data2 = data2;

    // list->sentinel->next永远指向头节点
    DLListNode* headNode = list->sentinel->next;

    // 将新节点与原来的头节点连接
    newNode->next = headNode;
    headNode->prev = newNode;

    // 将新节点与哨兵节点连接
    newNode->prev = list->sentinel;
    list->sentinel->next = newNode;

    list->curSize++;
}

// 尾插法
void add_tail(DLList* list, int data1,int data2){
    DLListNode* newNode = (DLListNode*)malloc(sizeof(DLListNode));
    if (!newNode){
        printf("DLListNode数据空间动态分配失败\n");
        return;
    }
    newNode->data1 = data1;
    newNode->data2 = data2;

    // list->sentinel->prev永远指向尾节点
    DLListNode* tailNode = list->sentinel->prev;

    // 将新节点与原来的尾节点连接
    newNode->prev = tailNode;
    tailNode->next = newNode;

    // 将新节点与哨兵节点连接
    newNode->next = list->sentinel;
    list->sentinel->prev = newNode;

    list->curSize++;
}

// 删除头部
int remove_head(DLList* list){
    int value;
    if (list->curSize == 0){
        printf("DLList链表为空，无法删除\n");
        return -1;
    }

    // list->sentinel->next永远指向头节点
    DLListNode* headNode = list->sentinel->next;
    value = headNode->data1;
    
    list->sentinel->next = headNode->next;
    headNode->next->prev = list->sentinel;

    free(headNode);
    list->curSize--;

    return value;
}

// 删除尾部
int remove_tail(DLList* list){
    int value;
    if (list->curSize == 0){
        printf("DLList链表为空，无法删除\n");
        return -1;
    }

    // list->sentinel->prev永远指向尾节点
    DLListNode* tailNode = list->sentinel->prev;
    value = tailNode->data1;

    list->sentinel->prev = tailNode->prev;
    tailNode->prev->next = list->sentinel;

    free(tailNode);
    list->curSize--;

    return value;
}

// 删除任意节点
int remove_node(DLList* list, DLListNode* node){
    int value;
    if (list->curSize == 0){
        printf("DLList链表为空，无法删除\n");
        return -1;
    }

    value = node->data1;

    node->prev->next = node->next;
    node->next->prev = node->prev;

    free(node);
    list->curSize--;

    return value;
}

// 遍历所有节点
void print_dllist(DLList* list){
    DLListNode* headNode = list->sentinel->next;

    while (headNode != list->sentinel){
        printf("%d\n",headNode->data1);
        headNode = headNode->next;
    }
    printf("currentSize: %d\n",list->curSize);
}

// 释放
void free_dllist(DLList* list){
    DLListNode* headNode;
    DLListNode* temp;
    if (list){
        headNode = list->sentinel->next;
        while (headNode != list->sentinel){
            temp = headNode;
            headNode = headNode->next;
            free(temp);
        }
        free(list->sentinel);
        free(list);
    }else {
        printf("DLList未分配内存空间，无法释放\n");
    }
}
