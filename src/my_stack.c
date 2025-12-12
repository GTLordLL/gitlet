#include "my_DLList.h"
#include "my_vector.h"
#include "my_stack.h"
#include <stdlib.h>
#include <stdio.h>


arrayStack* create_arrayStack(unsigned int initial_capacity){
    arrayStack* stack = (arrayStack*)malloc(sizeof(arrayStack));
    if (stack == NULL){
        printf("arrayStack结构空间动态分配失败\n");
        return NULL;
    }

    stack->storage = create_vector(initial_capacity);
    if (stack->storage == NULL){
        printf("arrayStack数据空间动态分配失败\n");
        free(stack);
        return NULL;
    }

    return stack;
}

void push_arrayStack(arrayStack* stack, int value){
    add_last_value(stack->storage, value);
}

int pop_arrayStack(arrayStack* stack){
    if (stack->storage->currentSize == 0){
        printf("arrayStack为空，无法删除！\n");
        return -1;
    }
    return remove_last_value(stack->storage);
}

void free_arrayStack(arrayStack* stack){
    if (stack){
        free_vector(stack->storage);
        free(stack);
    }else {
        printf("arrayStack未分配内存空间，无法释放\n");
    }
}

DLListStack* create_DLListStack(void){
    DLListStack* stack = (DLListStack*)malloc(sizeof(DLListStack));
    if (stack == NULL){
        printf("DLListStack结构空间动态分配失败\n");
        return NULL;
    }

    stack->storage = create_dllist();
    if (stack->storage == NULL){
        printf("DLListStack数据空间动态分配失败\n");
        free(stack);
        return NULL;
    }

    return stack;
}

void push_DLListStack(DLListStack* stack, int value){
    add_head(stack->storage,value,0);
}

int pop_DLListStack(DLListStack* stack){
    if (stack->storage->curSize == 0){
        printf("DLListStack为空，无法删除！\n");
        return -1;
    }
    
    return remove_head(stack->storage);
}

void free_DLListStack(DLListStack* stack){
    if (stack){
        free_dllist(stack->storage);
        free(stack);
    } else {
        printf("DLListStack未分配内存空间，无法释放\n");
    }
}
