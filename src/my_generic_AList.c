#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_generic_AList.h"

GenericAList* create_GenericAList(unsigned int initial_capacity,unsigned int elementSize){
    GenericAList* list = (GenericAList*)malloc(sizeof(GenericAList));
    if (list == NULL) {
        printf("GenericAList结构空间动态分配失败\n");
        return NULL;
    }

    list->data = malloc(elementSize * initial_capacity);
    if (list->data == NULL){
        printf("GenericAList数据空间动态分配失败\n");
        free(list);
        return NULL;
    }
    
    list->elementSize = elementSize;
    list->currentSize = 0;
    list->maxCapacity = initial_capacity;
    return list;
}

/* 辅助函数：计算地址
指针的加法运算是基于指针指向的数据类型的大小进行的
void* 指针算术运算被禁止：在标准C中，对 void* 指针进行加法运算是未定义行为。编译器不知道 void* 指向的数据类型有多大。
char* 指针按字节计算：char 的大小总是1字节，所以 (char*)list->data + n 会移动 n 个字节。
*/
void* get_element_address(GenericAList* list, unsigned int index){
    return (char*)list->data + index * list->elementSize;
}

void resize_GenericAList(GenericAList* list, unsigned int new_capacity){
    void* new_data = realloc(list->data, new_capacity * list->elementSize);
    if (new_data == NULL){
        printf("GenericAList扩容空间动态分配失败\n");
        return;
    }
    
    list->data = new_data;
    list->maxCapacity = new_capacity;
}

void add_last_GenericAList(GenericAList* list, void* element) {
    if (list->currentSize >= list->maxCapacity) {
        resize_GenericAList(list, list->maxCapacity * 2);
    }

    void* target = get_element_address(list, list->currentSize);
    memcpy(target, element, list->elementSize); // 将element中的数据复制到目标地址
    list->currentSize++;
}

void get_element_GenericAList(GenericAList* list, unsigned int index, void* out_element) {
    if (index >= list->currentSize) {
        printf("索引越界\n");
        return;
    }
    void* source = get_element_address(list, index);
    memcpy(out_element, source, list->elementSize); // 将source中的数据复制到out_element
}

void set_element_GenericAList(GenericAList* list, unsigned int index, void* element) {
    if (index >= list->currentSize) {
        printf("索引越界\n");
        return;
    }
    void* target = get_element_address(list, index);
    memcpy(target, element, list->elementSize); // 将element中的数据复制到目标地址
}

void remove_last_GenericAList(GenericAList* list) {
    if (list->currentSize == 0) {
        printf("列表为空，无法删除！\n");
        return;
    }

    list->currentSize--;

    // 缩容逻辑：如果利用率小于 1/4，且容量还比较大
    if ((list->currentSize > 0) && (list->currentSize <= (list->maxCapacity / 4))) {
        resize_GenericAList(list, list->maxCapacity / 2);
    }
}

void free_GenericAList(GenericAList* list) {
    if (list){
        free(list->data);
        free(list);
    }
}
