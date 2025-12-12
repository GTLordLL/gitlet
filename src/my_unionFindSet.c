#include <stdlib.h>
#include <stdio.h>
#include "my_unionFindSet.h"
#include "my_vector.h"

DisjointSet* create_unionFindSet(unsigned int initial_capacity){
    unsigned int index;
    DisjointSet* set = (DisjointSet*)malloc(sizeof(DisjointSet));
    if (set == NULL) {
        printf("DisjointSet结构空间动态分配失败\n");
        return NULL;
    }

    set->parent = create_vector(initial_capacity);
    if (set->parent == NULL){
        printf("DisjointSet数据空间动态分配失败\n");
        free(set);
        return NULL;
    }

    set->parent->currentSize = initial_capacity;        // 作为连通分量

    // 初始化：每个元素自成一个集合，大小为 1 (用 -1 表示)
    for (index = 0; index < initial_capacity; index++){
        set->parent->data[index] = -1;
    }
    return set;
}

// 查找元素 element 所属集合的根节点索引，并进行路径压缩
// 路径压缩 (Path Compression)：递归地将 element 及其祖先直接挂在根节点下。
unsigned int find_root(DisjointSet* set,unsigned int element){
    if (element >= set->parent->maxCapacity){
        printf("错误：索引超出范围 \n");
        exit(EXIT_FAILURE);
    }
    
    // 如果 parent[p] < 0，说明 p 是根节点 (因为它存的是集合大小的负值)
    if (set->parent->data[element] < 0){
        return element;
    }
    // set->parent->data[element]存储元素 element 的父节点的索引
    // 路径压缩。将 parent[p] 直接指向递归找到的根节点
    set->parent->data[element] = find_root(set,set->parent->data[element]);
    return set->parent->data[element];
}

// 合并 p 和 q 所在的集合
// 带权合并 (Weighted Union)：总是将小树的根节点指向大树的根节点，防止树结构过高。
void union_sets(DisjointSet* set,unsigned int p,unsigned int q){
    unsigned int rootP,rootQ,sizeP,sizeQ;
    rootP = find_root(set,p);
    rootQ = find_root(set,q);

    // 如果根节点相同，说明它们已经在同一个集合中，无需操作
    if (rootP == rootQ) {
        return;
    }
    // 获取两个集合的大小 (负值的绝对值)
    sizeP = -set->parent->data[rootP];
    sizeQ = -set->parent->data[rootQ];

    // 加权合并：总是将小树 (小尺寸) 连接到大树 (大尺寸) 上
    if (sizeP < sizeQ){
        // P 连到 Q：将 P 的根节点指向 Q 的根节点
        set->parent->data[rootP] = rootQ;
        // 更新 Q 的根节点的大小：合并后的大小 = 原有大小 + P 的大小
        set->parent->data[rootQ] -= sizeP;
    }else{
        set->parent->data[rootQ] = rootP;
        set->parent->data[rootP] -= sizeQ;
    }
    // 连通分量的数量减 1
    set->parent->currentSize--;
}

unsigned char is_connected(DisjointSet* set,unsigned int p,unsigned int q){
    return find_root(set,p) == find_root(set,q);
}

unsigned int get_set_size(DisjointSet* set,unsigned int element){
    unsigned int root;
    root = find_root(set,element);
    return -set->parent->data[root];
}

void print_DisjointSet(DisjointSet* set){
    unsigned int i;
    printf("Parent Array: [");
    for (i = 0; i < set->parent->maxCapacity; i++) {
        printf("%d%s", set->parent->data[i], (i == set->parent->maxCapacity - 1) ? "" : ", ");
    }
    printf("]\n");
    printf("Total elements: %d, Number of Sets: %d\n", set->parent->maxCapacity, set->parent->currentSize);
}

void free_DisjointSet(DisjointSet* set){
    if (set){
        free_vector(set->parent);
        free(set);   
    }else{
        printf("DisjointSet未分配内存空间，无法释放\n");
    }
}
