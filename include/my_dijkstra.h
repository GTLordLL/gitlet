// 单源最短路径算法
#ifndef __MY_DIJKSTRA_H__
#define __MY_DIJKSTRA_H__

// 定义无穷大 (用于初始化距离)
#define INFINITY_DIST 0xFFFFFFFF // max unsigned int

typedef struct my_graph my_graph;

// 结果结构体：封装最短路径计算结果
typedef struct ShortestPathResult{
    unsigned int* distTo; // 记录从起点到每个顶点的当前已知最短距离。初始时起点为 0，其余为无穷大。
    unsigned short* edgeTo; // 记录最短路径树。edgeTo[v] = u 表示到达顶点 v 的最短路径是从 u 来的（用于最后还原路径）。
    unsigned short source; // 起点
    unsigned short vertices;
}ShortestPathResult;

ShortestPathResult* create_dijkstraResult(my_graph* graph,unsigned short start_vertex);
void print_shortest_path(ShortestPathResult* result, unsigned short target_vertex);
void free_dijkstraResult(ShortestPathResult* result);


#endif
