/*
BFS广度优先搜索:
用途： 查找无权图中的最短路径，或确定图的连通分量。
核心机制： 使用 队列 (Queue) 实现。它像水波一样，一层一层向外扩散，确保先访问距离起点近的节点。

*/
#ifndef __MY_GRAPH_H__
#define __MY_GRAPH_H__

typedef struct DLList DLList;

typedef struct my_graph{
    unsigned int vertices;  // 顶点的数量 (Vertices)
    unsigned int edges;     // 边的数量 (Edges)
    DLList** adjList;       // 邻接表数组：adj[i] 是一个链表，存着 i 的所有邻居
    int isDirected;         // 0: 无向图, 1: 有向图
    int data;
}my_graph;

my_graph* create_graph(unsigned int vertices,int isDirected);
void add_edge(my_graph* graph,unsigned short u,unsigned short v,int weight);
void print_graph(my_graph* g);
void free_graph(my_graph* g);


#endif
