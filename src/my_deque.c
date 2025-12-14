#include "my_deque.h"
#include "my_DLList.h"
#include "my_circularArray.h"
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

arrayDeque* create_arrayDeque(unsigned int initial_capacity){
    arrayDeque* deque = (arrayDeque*)malloc(sizeof(arrayDeque));
    if (deque == NULL) {
        printf("arrayDeque结构空间动态分配失败\n");
        return NULL;
    }
    deque->storage = create_circularArray(initial_capacity);
    if (deque->storage == NULL){
        printf("arrayDeque数据空间动态分配失败\n");
        free(deque);
        return NULL;
    }
    return deque;
}

void addFirst_arrayDeque(arrayDeque* deque,int value){
    addFirst_circularArray(deque->storage,value);
}

void addLast_arrayDeque(arrayDeque* deque,int value){
    addLast_circularArray(deque->storage,value);
}

int removeFirst_arrayDeque(arrayDeque* deque){
    if (deque->storage->curSize == 0){
        printf("arrayDeque为空，无法删除！\n");
        return -1;
    }
    return removeFirst_circularArray(deque->storage);
}

int removeLast_arrayDeque(arrayDeque* deque){
    if (deque->storage->curSize == 0){
        printf("arrayDeque为空，无法删除！\n");
        return -1;
    }
    return removeLast_circularArray(deque->storage);
}

int getValue_arrayDeque(arrayDeque* deque, unsigned int logic_index){
    return getValue_circularArray(deque->storage, logic_index);
}

void print_arrayDeque(arrayDeque* deque){
    unsigned int i;
    printf("arrayDeque (size=%d):\n ", deque->storage->curSize);
    for (i = 0; i < deque->storage->curSize; i++){
        printf("%d\n",getValue_circularArray(deque->storage, i));
    }
}

void free_arrayDeque(arrayDeque* deque){
    if (deque){
        free_circularArray(deque->storage);
        free(deque);
    } else {
        printf("arrayDeque未分配内存空间，无法释放\n");
    }
}

