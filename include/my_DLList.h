/* 双向链表 
Circular Sentinel (环形哨兵) 拓扑结构
双向 (Doubly): 每个节点都有 prev 和 next。
环形 (Circular):
    Head (头)节点地址 是 sentinel->next。
    Tail (尾)节点地址 是 sentinel->prev。
    空链表 时，sentinel->next 和 sentinel->prev 都指向 sentinel 自己。
*/

#ifndef __MY_DLLIST_H__
#define __MY_DLLIST_H__

// 定义节点结构体
typedef struct Node{
    struct Node* prev;      // 前驱指针
    int data1;              // 数据
    int data2;
    struct Node* next;      // 后继指针
}Node;

// 定义链表壳子 (Wrapper)
typedef struct DLList{
    Node* sentinel;         // 唯一的哨兵节点
    unsigned int curSize;
}DLList;

DLList* create_dllist(void);
void add_head(DLList* list, int data1,int data2);
void add_tail(DLList* list, int data1,int data2);
int remove_head(DLList* list);
int remove_tail(DLList* list);
void print_dllist(DLList* list);
void free_dllist(DLList* list);

#endif
