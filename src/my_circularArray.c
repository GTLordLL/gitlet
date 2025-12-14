#include <stdio.h>
#include <stdlib.h>
#include "my_circularArray.h"

// **********************辅助函数：处理环形索引***************************
// 计算 index - 1 (用于 addFirst),技巧：+ capacity 再模，防止结果为负数
unsigned int minus_one(unsigned int index, unsigned int capacity) {
    return (index - 1 + capacity) % capacity;
}

// 计算 index + 1 (用于 addLast)
unsigned int plus_one(unsigned int index, unsigned int capacity) {
    return (index + 1) % capacity;
}

circularArray* create_circularArray(unsigned int initial_capacity){
    circularArray* array = (circularArray*)malloc(sizeof(circularArray));
    if (array == NULL) {
        printf("circularArray结构空间动态分配失败\n");
        return NULL;
    }
    array->data = (int*)malloc(initial_capacity * sizeof(int));
    if (array->data == NULL){
        printf("circularArray数据空间动态分配失败\n");
        free(array);
        return NULL;
    }

    array->curSize = 0;
    array->maxCapacity = initial_capacity;

    // 常用初始化策略：让 nextFirst 和 nextLast 在数组中间或者错开
    array->headIndex = 0;
    array->tailIndex = 1;
    return array;
}

void resize_circularArray(circularArray* array, unsigned int new_capacity){
    unsigned int curHeadIndex,newHeadIndex;

    int* new_data = (int*)malloc(sizeof(int) * new_capacity);
    if (new_data == NULL){
        printf("circularArray扩容空间失败\n");
        return;
    }

    // 因为 headIndex 指向的是"待插入的空位"，所以要退回到有元素的位置
    curHeadIndex = plus_one(array->headIndex,array->maxCapacity);

    for (newHeadIndex = 0; newHeadIndex < array->curSize; newHeadIndex++){
        new_data[newHeadIndex] = array->data[curHeadIndex];
        curHeadIndex = plus_one(curHeadIndex,array->maxCapacity);
    }

    if (array->data){
        free(array->data);
    }else{
        printf("circularArray未分配内存空间，无法释放\n");
        return;
    }
    array->data = new_data;
    array->maxCapacity = new_capacity;
    array->headIndex = new_capacity - 1;
    array->tailIndex = array->curSize;
}

void addFirst_circularArray(circularArray* array,int value){
    if (array->curSize >= array->maxCapacity){
        resize_circularArray(array,array->maxCapacity * 2);
    }
    
    array->data[array->headIndex] = value;
    array->headIndex = minus_one(array->headIndex,array->maxCapacity);
    array->curSize++;
}

void addLast_circularArray(circularArray* array,int value){
    if (array->curSize >= array->maxCapacity){
        resize_circularArray(array,array->maxCapacity * 2);
    }

    array->data[array->tailIndex] = value;
    array->tailIndex = plus_one(array->tailIndex,array->maxCapacity);
    array->curSize++;
}

int removeFirst_circularArray(circularArray* array){
    int value;
    unsigned int newIndex;
    if (array->curSize == 0){
        printf("circularArray为空，无法删除！\n");
        return -1;
    } 
    
    newIndex = plus_one(array->headIndex,array->maxCapacity);
    value = array->data[newIndex];

    array->headIndex = newIndex;
    array->curSize--;

    if ((array->maxCapacity >= 16) && (array->curSize < (array->maxCapacity / 4))){
        resize_circularArray(array,array->maxCapacity / 2);
    }
    return value;
}

int removeLast_circularArray(circularArray* array){
    int value;
    unsigned int newIndex;
    if (array->curSize == 0){
        printf("circularArray为空，无法删除！\n");
        return -1;
    } 

    newIndex = minus_one(array->tailIndex,array->maxCapacity);
    value = array->data[newIndex];

    array->tailIndex = newIndex;
    array->curSize--;

    if ((array->maxCapacity >= 16) && (array->curSize < (array->maxCapacity / 4))){
        resize_circularArray(array,array->maxCapacity / 2);
    }
    return value;
}

// 支持 O(1) 的随机访问
int getValue_circularArray(circularArray* array, unsigned int logic_index){
    unsigned int logic_index_0,physic_index;
    if (logic_index >= array->curSize){
        printf("circularArray索引越界！\n");
        return -1;
    }
    // 逻辑索引 0 对应的物理索引是 nextFirst + 1
    // 逻辑索引 index 对应的物理索引是 (nextFirst + 1 + index) % capacity
    logic_index_0 = plus_one(array->headIndex,array->maxCapacity);
    physic_index = (logic_index_0 + logic_index) % array->maxCapacity;

    return array->data[physic_index];
}

void free_circularArray(circularArray* array) {
    if (array) {
        free(array->data);
        free(array);
    }else {
        printf("circularArray未分配内存空间，无法释放\n");
    }
}
