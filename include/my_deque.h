// 双端队列
#ifndef __MY_DEQUE_H__
#define __MY_DEQUE_H__

typedef struct DLList DLList;
typedef struct circularArray circularArray;


// 基于双向链表的 Deque
typedef struct DLListDeque {
    DLList* storage;
} DLListDeque;

// 基于循环数组的 Deque
typedef struct arrayDeque{
    circularArray* storage;
}arrayDeque;



DLListDeque* create_DLListDeque(void);
void addFirst_DLListDeque(DLListDeque* deque, int value);
void addLast_DLListDeque(DLListDeque* deque, int value);
int removeFirst_DLListDeque(DLListDeque* deque);
int removeLast_DLListDeque(DLListDeque* deque);
void free_DLListDeque(DLListDeque* deque);

arrayDeque* create_arrayDeque(unsigned int initial_capacity);
void addFirst_arrayDeque(arrayDeque* deque,int value);
void addLast_arrayDeque(arrayDeque* deque,int value);
int removeFirst_arrayDeque(arrayDeque* deque);
int removeLast_arrayDeque(arrayDeque* deque);
int getValue_arrayDeque(arrayDeque* deque, unsigned int logic_index);
void print_arrayDeque(arrayDeque* deque);
void free_arrayDeque(arrayDeque* deque);

#endif
