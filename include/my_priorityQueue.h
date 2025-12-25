/* 堆
1.必须是完全二叉树（金字塔）：必须填满每一层，才能开始填下一层
2.堆顶：根节点
3.大顶堆：根节点存最大值
4.小顶堆：根节点存最小值
5.堆底：最后一层最右边的叶子
6.每次堆叠节点都是从左向右
7.用数组存堆比链表高效
    parent数组下标 = (child下标-1)/2
    left child数组下标 = parent下标*2 + 1
    right child数组下标 = parent下标*2 + 2
8.内存中的堆区和堆数据结构不是一回事
*/
#ifndef __MY_PRIORITYQUEUE_H__
#define __MY_PRIORITYQUEUE_H__

#define INITIAL_HEAP_CAPACITY    16      // 最小堆的初始容量

typedef struct GenericAList GenericAList;
typedef struct HashMap HashMap;


// 优先级队列的节点结构：
// 存储图的顶点 ID 和距离（即优先级）
typedef struct PQNode{
    unsigned int vertex;         // 顶点编号
    unsigned int distance;       // 距离（优先级：距离越小，优先级越高）
}PQNode;

// 最小堆结构体
typedef struct MinBinaryHeap{
    GenericAList* storage;               // 存储堆元素的数组
    HashMap* indexMap;                  // 位置映射表 Key: vertex, Value: index in storage
}MinBinaryHeap;

MinBinaryHeap* create_MinBinaryHeap(unsigned int initial_capacity);
void free_MinBinaryHeap(MinBinaryHeap* heap);
unsigned char is_empty(MinBinaryHeap* heap);
void heap_insert(MinBinaryHeap* heap,unsigned int vertex,unsigned int distance);
void heap_extract_min(MinBinaryHeap* heap,PQNode* out_node);
void heap_change_priority(MinBinaryHeap* heap, int vertex, int new_distance);

#endif
