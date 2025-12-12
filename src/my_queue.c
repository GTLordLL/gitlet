#include "my_queue.h"
#include "my_DLList.h"
#include <stdlib.h>
#include <stdio.h>

DLListQueue* create_DLListQueue(void){
    DLListQueue* queue = (DLListQueue*)malloc(sizeof(DLListQueue));
    if (queue == NULL){
        printf("DLListQueue结构空间动态分配失败\n");
        return NULL;
    }
    queue->storage = create_dllist();
    if (queue->storage == NULL){
        printf("DLListQueue数据空间动态分配失败\n");
        free(queue);
        return NULL;
    }
    return queue;
}

// 入队：使用链表的末尾
void enqueue_DLListQueue(DLListQueue* queue, int value){
    add_tail(queue->storage,value,0);
}

// 出队：使用链表的头部
int dequeue_DLListQueue(DLListQueue* queue){
    if (queue->storage->curSize == 0){
        printf("DLListQueue为空，无法删除！\n");
        return -1;
    }
    return remove_head(queue->storage);
}

void free_DLListQueue(DLListQueue* queue){
    if (queue){
        free_dllist(queue->storage);
        free(queue);
    } else {
        printf("DLListQueue未分配内存空间，无法释放\n");
    }
}
