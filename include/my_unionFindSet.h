/* 并查集 (Union-Find)
底层结构： 使用一个简单的有符号整数数组 parent[] 作为存储。
父节点表示： parent[i] 存储元素 i 的父节点的索引。
根节点标识： 根节点 r 的 parent[r] 存储一个负数，这个负数的绝对值代表了这棵树（即这个集合）的元素总数（大小/权重）。
    例如：如果 parent[r] == -5，表示 r 是根节点，它所在的集合有 5 个元素。
初始化： 所有元素自成一派，所以 parent[i] 初始值为 -1 (大小为 1 的集合)。
*/
#ifndef __MY_UNIONFINDSET_H__
#define __MY_UNIONFINDSET_H__

typedef struct GenericAList GenericAList;

typedef struct DisjointSet{
    GenericAList* parent;
    unsigned int connected_component; // 连通分量/集合数量
}DisjointSet;

DisjointSet* create_unionFindSet(unsigned int initial_capacity);
unsigned int find_root(DisjointSet* set,unsigned int element);
void union_sets(DisjointSet* set,unsigned int p,unsigned int q);
char is_connected(DisjointSet* set,unsigned int p,unsigned int q);
unsigned int get_set_size(DisjointSet* set,unsigned int element);
void print_DisjointSet(DisjointSet* set);
void free_DisjointSet(DisjointSet* set);


#endif
