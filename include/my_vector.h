/* malloc 的内部机制：
1.内存池管理：维护空闲内存块的链表/树结构
2.首次适配算法：寻找第一个足够大的空闲块
3.内存碎片处理：可能分割大块或合并相邻空闲块
4.向操作系统申请：当内存池不足时，通过 brk()或 mmap()系统调用扩展堆空间
*/

/* x86-64架构下的内存布局：
高地址：
    1.内核空间：操作系统代码段，PCB进程控制块，堆栈指针表，程序入口表等
低地址：
    2.进程栈（硬件）：PUSH，POP，CALL，RET
    3.内存映射段
    4.堆（软件）：由malloc/free控制，无硬件电路
    5.BSS段：未初始化（程序开始执行前没有写入值，默认为0）的全局变量/静态变量
    6.数据段：已初始化（程序开始执行前就写入了值）的全局变量/静态变量
    7.代码段：应用程序，用户程序
*/

//动态数组需要连续的内存
// *************************************************
#ifndef __MY_VECTOR_H__
#define __MY_VECTOR_H__

// 定义动态数组结构
typedef struct my_vector{                     // 内存对齐会浪费空间，尽量保证大小一样
    unsigned int currentSize;       // 当前元素个数
    unsigned int maxCapacity;       // 当前最大容量
    int* data;                      // 指向实际存储数据的内存区域
}my_vector;

my_vector* create_vector(unsigned int initial_capacity);
void resize_vector(my_vector* v, unsigned int new_capacity);
void add_last_value(my_vector* v, int value);
int get_value(my_vector* v, unsigned int index);
void set_value(my_vector* v, unsigned int index, int value);
int remove_last_value(my_vector* v);
void insert_value(my_vector* v, unsigned int index, int value);
int delete_value(my_vector* v, unsigned int index);
void free_vector(my_vector* v);

#endif
