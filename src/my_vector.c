#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "my_vector.h"

// 创建动态数组
my_vector* create_vector(unsigned int initial_capacity){
    my_vector* v = (my_vector*)malloc(sizeof(my_vector));
    if (v == NULL) {
        printf("my_vector结构空间动态分配失败\n");
        return NULL;
    }

    v->data = (int*)malloc(initial_capacity * sizeof(int));
    if (v->data == NULL){
        printf("my_vector数据空间动态分配失败\n");
        free(v);
        return NULL;
    }
    
    v->currentSize = 0;
    v->maxCapacity = initial_capacity;
    return v;
}

// 修改数组大小
void resize_vector(my_vector* v, unsigned int new_capacity){
    // realloc 会尝试在原内存后扩展，如果不够，它会申请新内存、拷贝数据、释放旧内存
    int* new_data = (int*)realloc(v->data, new_capacity * sizeof(int));

    if (new_data == NULL){
        printf("my_vector扩容空间动态分配失败\n");
        return;
    }
    
    v->data = new_data;
    v->maxCapacity = new_capacity;
}

// 在末尾添加元素
void add_last_value(my_vector* v, int value) {
    // 检查容量是否已满
    if (v->currentSize >= v->maxCapacity) {
        // 扩容策略：扩容为原来的 2 倍 (CS61B 推荐策略)
        resize_vector(v, v->maxCapacity * 2);
    }

    // 填入数据
    v->data[v->currentSize] = value;
    v->currentSize++;
}

// 通过索引获取元素
int get_value(my_vector* v, unsigned int index){
    if (index >= v->currentSize){
        printf("my_vector索引越界！\n");
        return -1;
    }
    return v->data[index];
}

// 通过索引修改元素
void set_value(my_vector* v, unsigned int index, int value){
    if (index >= v->currentSize){
        printf("my_vector索引越界！\n");
        return;
    }
    v->data[index] = value;
}

// 在末尾删除元素
int remove_last_value(my_vector* v){
    int value;
    if (v->currentSize == 0){
        printf("my_vector数组为空，无法删除！\n");
        return -1;
    }

    // 获取要删除的元素（其实不需要真正"删除"内存中的值，只需把 size 减 1）
    value = v->data[v->currentSize - 1];
    v->currentSize--;

    // 缩容逻辑：如果利用率小于 1/4，且容量还比较大
    if ((v->currentSize > 0) && (v->currentSize == (v->maxCapacity / 4))) {
        resize_vector(v, v->maxCapacity / 2);
    }

    return value;
}

// 在指定索引前处插入元素
void insert_value(my_vector* v, unsigned int index, int value){
    if (index > v->currentSize){
        printf("my_vector索引无效，只能在[0,curSize]之间！\n");
        return;
    }
    if (v->currentSize >= v->maxCapacity){
        resize_vector(v,v->maxCapacity * 2);
    }

    memmove(
        &v->data[index+1],
        &v->data[index],
        (v->currentSize - index) * sizeof(int)
    );

    set_value(v,index,value);

    v->currentSize++;
}

// 删除制定索引处的元素
int delete_value(my_vector* v, unsigned int index){
    int value;
    if (index >= v->currentSize){
        printf("my_vector索引无效，只能在[0,curSize)之间！\n");
        return -1;
    }

    value = get_value(v,index);

    memmove(
        &v->data[index],
        &v->data[index+1],
        (v->currentSize - index -1) * sizeof(int)
    );
    
    v->currentSize--;

    if ((v->currentSize > 2) && (v->currentSize <= (v->maxCapacity / 4))) {
        resize_vector(v, v->maxCapacity / 2);
    }
    return value;
}

// 释放堆内存
void free_vector(my_vector* v) {
    if (v) {
        free(v->data); // 先释放底层数组
        free(v);       // 再释放结构体本身
    } else {
        printf("my_vector未分配内存空间，无法释放\n");
    }
}
