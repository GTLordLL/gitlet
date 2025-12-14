#ifndef __MY_CIRCULARLIST_H__
#define __MY_CIRCULARLIST_H__

typedef struct GenericAList{
    void* data;               // 泛型数据指针,指向实际存储数据的内存区域
    unsigned int elementSize; // 每个元素的大小（字节数）
    unsigned int currentSize; // 当前元素个数
    unsigned int maxCapacity; // 当前最大容量
}GenericAList;

GenericAList* create_GenericAList(unsigned int initial_capacity,unsigned int elementSize);
void add_last_GenericAList(GenericAList* list, void* element);
void get_element_GenericAList(GenericAList* list, unsigned int index, void* out_element);
void set_element_GenericAList(GenericAList* list, unsigned int index, void* element);
void remove_last_GenericAList(GenericAList* list);
void free_GenericAList(GenericAList* list);

#endif