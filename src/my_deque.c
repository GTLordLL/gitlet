#include "my_deque.h"
#include "my_DLList.h"
#include <stdlib.h>
#include <stdio.h>

DLListDeque* create_DLListDeque(void){
    DLListDeque* deque = (DLListDeque*)malloc(sizeof(DLListDeque));
    if (deque == NULL){
        printf("DLListDeque结构空间动态分配失败\n");
        return NULL;
    }

    deque->storage = create_dllist();
    if (deque->storage == NULL){
        printf("DLListDeque数据空间动态分配失败\n");
        free(deque);
        return NULL;
    }

    return deque;
}

void addFirst_DLListDeque(DLListDeque* deque, int value){
    add_head(deque->storage,value,0);
}

void addLast_DLListDeque(DLListDeque* deque, int value){
    add_tail(deque->storage,value,0);
}

int removeFirst_DLListDeque(DLListDeque* deque){
    if (deque->storage->curSize == 0){
        printf("DLListDeque为空，无法删除！\n");
        return -1;
    }
    
    return remove_head(deque->storage);
}

int removeLast_DLListDeque(DLListDeque* deque){
    if (deque->storage->curSize == 0){
        printf("DLListDeque为空，无法删除！\n");
        return -1;
    }
    return remove_tail(deque->storage);
}

void free_DLListDeque(DLListDeque* deque){
    if (deque){
        free_dllist(deque->storage);
        free(deque);
    } else {
        printf("DLListDeque未分配内存空间，无法释放\n");
    }
}

// **********************辅助函数：处理环形索引***************************
// 计算 index - 1 (用于 addFirst),技巧：+ capacity 再模，防止结果为负数
unsigned int minus_one(unsigned int index, unsigned int capacity) {
    return (index - 1 + capacity) % capacity;
}

// 计算 index + 1 (用于 addLast)
unsigned int plus_one(unsigned int index, unsigned int capacity) {
    return (index + 1) % capacity;
}

arrayDeque* create_arrayDeque(unsigned int initial_capacity){
    arrayDeque* deque = (arrayDeque*)malloc(sizeof(arrayDeque));
    if (deque == NULL) {
        printf("arrayDeque结构空间动态分配失败\n");
        return NULL;
    }
    deque->data = (int*)malloc(initial_capacity * sizeof(int));
    if (deque->data == NULL){
        printf("arrayDeque数据空间动态分配失败\n");
        free(deque);
        return NULL;
    }

    deque->curSize = 0;
    deque->maxCapacity = initial_capacity;

    // 常用初始化策略：让 nextFirst 和 nextLast 在数组中间或者错开
    deque->headIndex = 0;
    deque->tailIndex = 1;
    return deque;
}

void resize_arrayDeque(arrayDeque* deque, unsigned int new_capacity){
    unsigned int curHeadIndex,newHeadIndex;

    int* new_data = (int*)malloc(sizeof(int) * new_capacity);
    if (new_data == NULL){
        printf("arrayDeque扩容空间失败\n");
        return;
    }

    // 因为 headIndex 指向的是"待插入的空位"，所以要退回到有元素的位置
    curHeadIndex = plus_one(deque->headIndex,deque->maxCapacity);

    for (newHeadIndex = 0; newHeadIndex < deque->curSize; newHeadIndex++){
        new_data[newHeadIndex] = deque->data[curHeadIndex];
        curHeadIndex = plus_one(curHeadIndex,deque->maxCapacity);
    }

    if (deque->data){
        free(deque->data);
    }else{
        printf("arrayDeque未分配内存空间，无法释放\n");
        return;
    }
    deque->data = new_data;
    deque->maxCapacity = new_capacity;
    deque->headIndex = new_capacity - 1;
    deque->tailIndex = deque->curSize;
}

void addFirst_arrayDeque(arrayDeque* deque,int value){
    if (deque->curSize >= deque->maxCapacity){
        resize(deque,deque->maxCapacity * 2);
    }
    
    deque->data[deque->headIndex] = value;
    deque->headIndex = minus_one(deque->headIndex,deque->maxCapacity);
    deque->curSize++;
}

void addLast_arrayDeque(arrayDeque* deque,int value){
    if (deque->curSize >= deque->maxCapacity){
        resize(deque,deque->maxCapacity * 2);
    }

    deque->data[deque->tailIndex] = value;
    deque->tailIndex = plus_one(deque->tailIndex,deque->maxCapacity);
    deque->curSize++;
}

int removeFirst_arrayDeque(arrayDeque* deque){
    int value;
    unsigned int newIndex;
    if (deque->curSize == 0){
        printf("arrayDeque为空，无法删除！\n");
        return -1;
    } 
    
    newIndex = plus_one(deque->headIndex,deque->maxCapacity);
    value = deque->data[newIndex];

    deque->headIndex = newIndex;
    deque->curSize--;

    if ((deque->maxCapacity >= 16) && (deque->curSize < (deque->maxCapacity / 4))){
        resize(deque,deque->maxCapacity / 2);
    }
    return value;
}

int removeLast_arrayDeque(arrayDeque* deque){
    int value;
    unsigned int newIndex;
    if (deque->curSize == 0){
        printf("arrayDeque为空，无法删除！\n");
        return -1;
    } 

    newIndex = minus_one(deque->tailIndex,deque->maxCapacity);
    value = deque->data[newIndex];

    deque->tailIndex = newIndex;
    deque->curSize--;

    if ((deque->maxCapacity >= 16) && (deque->curSize < (deque->maxCapacity / 4))){
        resize(deque,deque->maxCapacity / 2);
    }
    return value;
}

// 支持 O(1) 的随机访问
int getValue_arrayDeque(arrayDeque* deque, unsigned int logic_index){
    unsigned int logic_index_0,physic_index;
    if (logic_index >= deque->curSize){
        printf("arrayDeque索引越界！\n");
        return -1;
    }
    // 逻辑索引 0 对应的物理索引是 nextFirst + 1
    // 逻辑索引 index 对应的物理索引是 (nextFirst + 1 + index) % capacity
    logic_index_0 = plus_one(deque->headIndex,deque->maxCapacity);
    physic_index = (logic_index_0 + logic_index) % deque->maxCapacity;

    return deque->data[physic_index];
}

void print_arrayDeque(arrayDeque* deque){
    unsigned int i;
    printf("arrayDeque (size=%d):\n ", deque->curSize);
    for (i = 0; i < deque->curSize; i++){
        printf("%d\n",getValue_arrayDeque(deque,i));
    }
}

void free_arrayDeque(arrayDeque* deque) {
    if (deque) {
        free(deque->data);
        free(deque);
    }else {
        printf("arrayDeque未分配内存空间，无法释放\n");
    }
}
