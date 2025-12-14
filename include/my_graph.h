/*
BFS广度优先搜索:
DFS深度优先搜索:
获取图的连通分量数量
*/
#ifndef __MY_GRAPH_H__
#define __MY_GRAPH_H__

typedef struct DLList DLList;

typedef struct my_graph{
    DLList** adjList;       // 邻接表数组：adj[i] 是一个链表，存着 i 的所有邻居
    unsigned int edges;     // 边的数量 (Edges)
    int isDirected;         // 0: 无向图, 1: 有向图
    unsigned short vertices;  // 顶点的数量 (Vertices)
}my_graph;

my_graph* create_graph(unsigned short vertices,int isDirected);
void add_edge(my_graph* graph,unsigned short u,unsigned short v,int weight);
void print_graph(my_graph* g);
void free_graph(my_graph* g);
void breadth_first_search(my_graph* graph,unsigned short start_vertex);
void depth_first_search(my_graph* graph, unsigned short start_vertex);
unsigned int get_connected_components(my_graph* graph);

#endif
