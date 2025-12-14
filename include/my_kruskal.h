// 如何以最小代价连接所有点 (MST)
#ifndef __MY_KRUSKAL_H__
#define __MY_KRUSKAL_H__

typedef struct GenericAList GenericAList;
typedef struct my_graph my_graph;

// 边的结构体
typedef struct Edge{
    unsigned short u;      // 边的起点
    unsigned short v;      // 边的终点
    int weight;           // 边的权重
}Edge;

typedef struct EdgeList{
    GenericAList* edges;        // 存储Edge类型元素的泛型列表
}EdgeList;

// 函数声明
EdgeList* create_EdgeList(unsigned int initialCapacity);
void add_edge_EdgeList(EdgeList* edgeList, Edge* edge);
void free_EdgeList(EdgeList* edgeList);
EdgeList* extract_edges_from_graph(my_graph* graph);
void sort_edges_by_weight(EdgeList* edgeList);
EdgeList* kruskal_MinimumSpanningTree(my_graph* graph);

#endif
