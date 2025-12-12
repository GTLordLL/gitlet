#ifndef __MY_QUEUE_H__
#define __MY_QUEUE_H__

typedef struct DLList DLList;

// 基于双向链表的队列结构
typedef struct DLListQueue{
    DLList* storage;
}DLListQueue;

DLListQueue* create_DLListQueue(void);
void enqueue_DLListQueue(DLListQueue* queue, int value);
int dequeue_DLListQueue(DLListQueue* queue);
void free_DLListQueue(DLListQueue* queue);

#endif
