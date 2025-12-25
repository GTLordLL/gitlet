#include <stdlib.h>
#include <stdio.h>
#include "dijkstra.h"
#include "my_priorityQueue.h"
#include "my_DLList.h"
#include "my_hashmap.h"
#include "my_graph.h"

ShortestPathResult* create_dijkstraResult(my_graph* graph,unsigned short start_vertex){
    unsigned short i;
    if (start_vertex >= graph->vertices){
        printf("Dijkstra错误：起点越界\n");
        return NULL;
    }
    
    // 1. 初始化结果结构体
    ShortestPathResult* result = (ShortestPathResult*)malloc(sizeof(ShortestPathResult));
    if (!result){
        printf("ShortestPathResult结构内存分配失败\n");
        return NULL;
    }
    result->vertices = graph->vertices;
    result->source = start_vertex;
    result->distTo = (unsigned int*)malloc(sizeof(unsigned int) * graph->vertices);
    if (!result->distTo){
        printf("distTo内存分配失败\n");
        free(result);
        return NULL;
    }
    result->edgeTo = (unsigned short*)malloc(sizeof(unsigned short) * graph->vertices);
    if (!result->edgeTo){
        printf("edgeTo内存分配失败\n");
        free(result->distTo);
        free(result);
        return NULL;
    }

    // 2. 创建优先级队列 (最小堆)
    MinBinaryHeap* pqueue = create_MinBinaryHeap(graph->vertices);
    if (!pqueue){
        printf("MinBinaryHeap内存分配失败\n");
        free(result->edgeTo);
        free(result->distTo);
        free(result);
        return NULL;
    }

    // 3. 初始化距离为无穷大，前驱节点为 0 (或无效值)
    for ( i = 0; i < graph->vertices; i++){
        result->distTo[i] = INFINITY_DIST;
        result->edgeTo[i] = i; // 指向自己表示没有前驱
    }
    result->distTo[start_vertex] = 0;

    // 4. 将起点加入 pqueue
    heap_insert(pqueue,start_vertex,0);

    // 5. 主循环：只要 PQ 不为空
    while (!is_empty(pqueue)){
        // (A) 提取当前距离最近的顶点 u
        PQNode min_node;
        heap_extract_min(pqueue,&min_node);
        unsigned int u = min_node.vertex;

        // (B) 遍历 u 的所有邻居

        DLList* neighbors = graph->adjList[u];
        DLListNode* currentNode = neighbors->sentinel->next;

        while (currentNode != neighbors->sentinel){
            unsigned short v = (unsigned short)currentNode->data1; // 邻居 ID
            int weight = currentNode->data2;    // 边权重

            // (C) 松弛操作 (Relaxation)
            // 如果当前 从u到v 的距离 比 之前存储的 从u到v 的距离更短
            unsigned int currentDist = result->distTo[u] + weight;
            if (result->distTo[u] != INFINITY_DIST && currentDist < result->distTo[v]){
                // 更新最短距离
                result->distTo[v] = currentDist;
                // 更新前驱节点
                result->edgeTo[v] = (unsigned short)u;

                // 这里展示了 indexMap 的强大之处：我们可以O(1)检查 v 是否在堆中
                int index_in_heap = get_value_HashMap(pqueue->indexMap,v);

                if (index_in_heap != -1){
                    // 已经在堆中,修改优先级
                    heap_change_priority(pqueue,v,result->distTo[v]);
                }else{
                    // 如果 v 之前从未被处理过 (dist 为无穷大)，我们需要 insert。
                    heap_insert(pqueue,v,result->distTo[v]);
                }
            }
            currentNode = currentNode->next;
        }
    }
    free_MinBinaryHeap(pqueue);
    return result;
}

// 辅助：使用递归逆序打印路径
void print_path_recursive(unsigned short* edgeTo, unsigned short start_vertex, unsigned short current_vertex){
    if (current_vertex == start_vertex){
        printf("%d",start_vertex);
        return;
    }
    print_path_recursive(edgeTo,start_vertex,edgeTo[current_vertex]);
    printf(" -> %d",current_vertex);
}

void print_shortest_path(ShortestPathResult* result, unsigned short target_vertex){
    if (target_vertex >= result->vertices){
        printf("print_shortest_path错误：终点越界\n");
        return;
    }
    printf("到顶点 %d 的最短距离: ", target_vertex);
    if (result->distTo[target_vertex] == INFINITY_DIST) {
        printf("不可达\n");
    } else {
        printf("%u\n路径: ", result->distTo[target_vertex]);
        print_path_recursive(result->edgeTo, result->source, target_vertex);
        printf("\n");
    }
}

void free_dijkstraResult(ShortestPathResult* result){
    if (result) {
        free(result->distTo);
        free(result->edgeTo);
        free(result);
    }
}

