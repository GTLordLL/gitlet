#include <stdio.h>
#include <stdlib.h>
#include "my_priorityQueue.h"
#include "my_generic_AList.h"
#include "my_hashmap.h"

// --- 辅助函数：定位父节点和子节点 ---

// curIndex的父节点索引
unsigned int Parent(unsigned int curIndex){
    if (curIndex == 0) return 0; // 根节点没有父节点
    return (curIndex - 1) / 2;
}

unsigned int LeftChild(unsigned int curIndex){
    return curIndex * 2 + 1;
}

unsigned int RightChild(unsigned int curIndex){
    return curIndex * 2 + 2;
}

void swap_node(MinBinaryHeap* heap, unsigned int i, unsigned int j){
    PQNode i_node,j_node;

    get_element_GenericAList(heap->storage,i,&i_node);
    get_element_GenericAList(heap->storage,j,&j_node);

    set_element_GenericAList(heap->storage,i,&j_node);
    set_element_GenericAList(heap->storage,j,&i_node);

    // 2. 【核心更新】同步哈希表中的索引信息
    put_kvPair(heap->indexMap,j_node.vertex,i);
    put_kvPair(heap->indexMap,i_node.vertex,j);
}

// 上浮：将新插入的节点移动到正确的位置 (O(log N))，满足小顶堆属性
void bubble_up(MinBinaryHeap* heap,unsigned int curIndex){
    PQNode cur_node,parent_node;

    while (curIndex > 0){
        unsigned int parent = Parent(curIndex);

        get_element_GenericAList(heap->storage,curIndex,&cur_node);
        get_element_GenericAList(heap->storage,parent,&parent_node);

        if (cur_node.distance < parent_node.distance){
            swap_node(heap,curIndex,parent);
            curIndex = parent; // 移动到父节点的位置继续检查
        }else{
            break; // 堆属性满足，停止上浮
        }
    }
}

// 下沉：将堆顶元素移动到正确的位置 (O(log N))，满足小顶堆属性
void bubble_down(MinBinaryHeap* heap,unsigned int curIndex){
    unsigned int curSize = heap->storage->currentSize;
    PQNode child_node,cur_node;

    while (LeftChild(curIndex) < curSize){
        unsigned int left_child = LeftChild(curIndex);
        unsigned int right_child = RightChild(curIndex);
        unsigned int smallest = left_child; // 假设左孩子最小

        // 检查右孩子是否存在且更小
        if (right_child < curSize){
            get_element_GenericAList(heap->storage,left_child,&child_node);
            unsigned int left_distance = child_node.distance;
            get_element_GenericAList(heap->storage,right_child,&child_node);
            unsigned int right_distance = child_node.distance;
            if (left_distance > right_distance){
                smallest = right_child;
            }
        }

        get_element_GenericAList(heap->storage,curIndex,&cur_node);
        get_element_GenericAList(heap->storage,smallest,&child_node);

        if (child_node.distance < cur_node.distance){
            swap_node(heap,curIndex,smallest);
            curIndex = smallest;
        }else{
            break; // 堆属性满足，停止下沉
        }
    }
}

MinBinaryHeap* create_MinBinaryHeap(unsigned int initial_capacity){
    MinBinaryHeap* heap = (MinBinaryHeap*)malloc(sizeof(MinBinaryHeap));
    if (!heap){
        printf("MinBinaryHeap结构内存分配失败\n");
        return NULL;
    }
    
    heap->storage = create_GenericAList(
        initial_capacity > 0 ? initial_capacity : INITIAL_HEAP_CAPACITY,
        sizeof(PQNode)
    );
    if (!heap->storage){
        printf("GenericAList内存分配失败\n");
        free(heap);
        return NULL;
    }

    heap->indexMap = create_hashmap(initial_capacity);
    if (!heap->indexMap){
        printf("hashmap内存分配失败\n");
        free_GenericAList(heap->storage);
        free(heap);
        return NULL;
    }

    return heap;
}

void free_MinBinaryHeap(MinBinaryHeap* heap){
    if (heap){
        free_GenericAList(heap->storage);
        free_hashmap(heap->indexMap);
        free(heap);
    }
}

unsigned char is_empty(MinBinaryHeap* heap){
    return heap->storage->currentSize == 0;
}

// 插入操作 (O(log N))
void heap_insert(MinBinaryHeap* heap,unsigned int vertex,unsigned int distance){
    PQNode new_node = {vertex,distance};

    // 1. 将新节点添加到数组末尾
    add_last_GenericAList(heap->storage,&new_node);

    // 在哈希表中记录：该顶点目前在数组末尾
    unsigned int current_index = heap->storage->currentSize - 1;
    put_kvPair(heap->indexMap,vertex,current_index);

    // 2. 上浮新节点以维护堆属性
    bubble_up(heap,heap->storage->currentSize -1);
}

// 提取最小元素操作 (O(log N))
void heap_extract_min(MinBinaryHeap* heap,PQNode* out_node){
    if (is_empty(heap)){
        printf("堆为空，无法提取最小元素\n");
        return;
    }

    // 1. 获取堆顶元素（最小元素）
    get_element_GenericAList(heap->storage,0,out_node);

    // 2. 将最后一个元素移动到堆顶
    unsigned int last_index = heap->storage->currentSize -1;
    PQNode last_node;
    if (last_index > 0){
        get_element_GenericAList(heap->storage,last_index,&last_node);
        set_element_GenericAList(heap->storage,0,&last_node);
        put_kvPair(heap->indexMap, last_node.vertex, 0);
    }

    // 3. 移除最后一个元素
    remove_last_GenericAList(heap->storage);
    remove_kvPair(heap->indexMap,out_node->vertex);

    // 4. 下沉堆顶元素以维护堆属性
    if (heap->storage->currentSize > 0){
        bubble_down(heap,0);
    }
}

void heap_change_priority(MinBinaryHeap* heap, int vertex, int new_distance){
    // 1. O(1) 从哈希表中获取该顶点在堆数组中的位置
    int index = get_value_HashMap(heap->indexMap,vertex);

    if (index == -1)return ;
    
    PQNode node;
    get_element_GenericAList(heap->storage,index,&node);
    int old_distance = node.distance;
    node.distance = new_distance;

    // 2. 更新数组中的值
    set_element_GenericAList(heap->storage,index,&node);

    if (new_distance < old_distance){
        bubble_up(heap,index);
    }else{
        bubble_down(heap,index);
    }
}
