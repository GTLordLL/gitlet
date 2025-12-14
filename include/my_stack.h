/* ADT 抽象数据类型：栈和队列
数据结构：动态数组，双向链表    注重存储
ADT:栈，队列                注重行为
*/

#ifndef __MY_STACK_H__
#define __MY_STACK_H__

// 前置声明，避免循环包含
typedef struct Int_vector Int_vector;
typedef struct DLList DLList;

// 基于动态数组的栈结构
typedef struct arrayStack{
    Int_vector* storage;
}arrayStack;

// 基于双向链表的栈结构
typedef struct DLListStack{
    DLList* storage;
} DLListStack;

arrayStack* create_arrayStack(unsigned int initial_capacity);
void push_arrayStack(arrayStack* stack, int value);
int pop_arrayStack(arrayStack* stack);
void free_arrayStack(arrayStack* stack);
DLListStack* create_DLListStack(void);
void push_DLListStack(DLListStack* stack, int value);
int pop_DLListStack(DLListStack* stack);
void free_DLListStack(DLListStack* stack);

#endif
