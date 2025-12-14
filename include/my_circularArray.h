#ifndef __MY_CIRCULARARRAY_H__
#define __MY_CIRCULARARRAY_H__

typedef struct circularArray{
    int* data;
    unsigned int curSize;
    unsigned int maxCapacity;
    unsigned int headIndex;         // 游标：头部插入位置
    unsigned int tailIndex;         // 游标：尾部插入位置
}circularArray;

circularArray* create_circularArray(unsigned int initial_capacity);
void resize_circularArray(circularArray* array, unsigned int new_capacity);
void addFirst_circularArray(circularArray* array,int value);
void addLast_circularArray(circularArray* array,int value);
int removeFirst_circularArray(circularArray* array);
int removeLast_circularArray(circularArray* array);
int getValue_circularArray(circularArray* array, unsigned int logic_index);
void free_circularArray(circularArray* array);

#endif
