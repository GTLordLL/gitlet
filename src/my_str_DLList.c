#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_str_DLList.h"

char* my_strdup(const char* str){
    if (!str) return NULL;
    size_t length = strlen(str) + 1; // 1. 获取长度（包括 \0）
    char* new_str = (char*)malloc(length); // 2. 申请内存
    if (!new_str) return NULL;
    return (char*)memcpy(new_str,str,length); // 3. 拷贝内容并返回
}

StringDLList* create_StringDLList(void){
    StringDLList* list = (StringDLList*)malloc(sizeof(StringDLList));
    if (list == NULL){
        printf("StringDLList结构空间动态分配失败\n");
        return NULL;
    }
    list->sentinel = (StringDLLNode*)malloc(sizeof(StringDLLNode));
    if (list->sentinel == NULL){
        printf("StringDLLNode数据空间动态分配失败\n");
        free(list);
        return NULL;
    }
    list->sentinel->prev = list->sentinel;
    list->sentinel->str1 = NULL;
    list->sentinel->str2 = NULL;
    list->sentinel->next = list->sentinel;
    list->currentSize = 0;
    return list;
}

void add_head_StrDLLNode(StringDLList* list,const char* str1,const char* str2){
    StringDLLNode* newNode = (StringDLLNode*)malloc(sizeof(StringDLLNode));
    if (!newNode){
        printf("StringDLLNode数据空间动态分配失败\n");
        return;
    }
    newNode->str1 = my_strdup(str1);
    newNode->str2 = my_strdup(str2);

    StringDLLNode* headNode = list->sentinel->next;

    newNode->next = headNode;
    headNode->prev = newNode;

    newNode->prev = list->sentinel;
    list->sentinel->next = newNode;

    list->currentSize++;
}

void add_tail_StrDLLNode(StringDLList* list,const char* str1,const char* str2){
    StringDLLNode* newNode = (StringDLLNode*)malloc(sizeof(StringDLLNode));
    if (!newNode){
        printf("StringDLLNode数据空间动态分配失败\n");
        return;
    }
    newNode->str1 = my_strdup(str1);
    newNode->str2 = my_strdup(str2);

    StringDLLNode* tailNode = list->sentinel->prev;

    newNode->prev = tailNode;
    tailNode->next = newNode;

    newNode->next = list->sentinel;
    list->sentinel->prev = newNode;

    list->currentSize++;
}

void remove_head_StrDLLNode(StringDLList* list){
    if (list->currentSize == 0){
        printf("StringDLList链表为空，无法删除\n");
        return;
    }

    StringDLLNode* headNode = list->sentinel->next;

    list->sentinel->next = headNode->next;
    headNode->next->prev = list->sentinel;

    free(headNode->str1);
    free(headNode->str2);
    free(headNode);
    list->currentSize--;
}

void remove_tail_StrDLLNode(StringDLList* list){
    if (list->currentSize == 0){
        printf("StringDLList链表为空，无法删除\n");
        return;
    }

    StringDLLNode* tailNode = list->sentinel->prev;

    list->sentinel->prev = tailNode->prev;
    tailNode->prev->next = list->sentinel;

    free(tailNode->str1);
    free(tailNode->str2);
    free(tailNode);
    list->currentSize--;
}

void remove_StrDLLNode(StringDLList* list,StringDLLNode* node){
    if (list->currentSize == 0){
        printf("StringDLList链表为空，无法删除\n");
        return;
    }

    node->prev->next = node->next;
    node->next->prev = node->prev;

    free(node->str1);
    free(node->str2);
    free(node);
    list->currentSize--;
}

void free_StringDLList(StringDLList* list){
    if (list){
        StringDLLNode* headNode = list->sentinel->next;
        while (headNode != list->sentinel){
            StringDLLNode* temp = headNode;
            headNode = headNode->next;
            free(temp->str1);
            free(temp->str2);
            free(temp);
        }
        free(list->sentinel);
        free(list);
    }
}

// 辅助打印函数：遍历并显示链表内容
void print_StringDLList(StringDLList* list) {
    if (!list || list->currentSize == 0) {
        printf("链表为空。\n");
        return;
    }
    printf("当前链表 (大小: %u):\n", list->currentSize);
    StringDLLNode* curr = list->sentinel->next;
    int i = 1;
    while (curr != list->sentinel) {
        printf(" [%d] str1: %-10s | str2: %-10s\n", i++, curr->str1, curr->str2);
        curr = curr->next;
    }
    printf("------------------------------------\n");
}

