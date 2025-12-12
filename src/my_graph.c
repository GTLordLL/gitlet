#include <stdio.h>
#include <stdlib.h>
#include "my_DLList.h"
#include "my_graph.h"
#include "my_queue.h"

my_graph* create_graph(unsigned int vertices,int isDirected){
    unsigned int i;
    my_graph* graph = (my_graph*)malloc(sizeof(my_graph));
    graph->vertices = vertices;
    graph->edges = 0;
    graph->isDirected = isDirected;

    graph->adjList = (DLList**)malloc(vertices * sizeof(DLList*));

    for (i = 0; i < vertices; i++){
        graph->adjList[i] = create_dllist();
    }
    return graph;
}

// v和u是下标
void add_edge(my_graph* graph,unsigned short u,unsigned short v,int weight){
    if (u >= graph->vertices || v >= graph->vertices){
        printf("my_graph错误：顶点索引越界\n");
        return;
    }
    // 1. 添加 u -> v
    add_tail(graph->adjList[u],v,weight);
    graph->edges++;
    // 2. 如果是无向图，且不是自环，添加 v -> u
    if (!graph->isDirected && u != v){
        add_tail(graph->adjList[v],u,weight);
    }
}

void print_graph(my_graph* g){
    unsigned int v;
    printf("图结构 (vertices=%d, edges=%d):\n", g->vertices, g->edges);
    for (v = 0; v < g->vertices; v++) {
        printf("顶点 %d: ", v);
        
        // 遍历链表
        DLList* list = g->adjList[v];
        Node* curr = list->sentinel->next;
        
        while (curr != list->sentinel) {
            // data1 是邻居 ID
            printf("-> %d ", curr->data1);
            curr = curr->next;
        }
        printf("\n");
    }
}

// 广度优先搜索 (BFS)
// 在无权图中找到最短路径，使用队列进行层序遍历，并打印遍历序列
void breadth_first_search(my_graph* graph,unsigned int start_vertex){
    if (start_vertex >= graph->vertices){
        printf("BFS错误：起始顶点索引越界！\n");
        return;
    }


    
}

// 深度优先搜索（DFS）
void depth_first_search(my_graph* graph){


}

void free_graph(my_graph* g){
    unsigned int i;
    if (g){
        for (i = 0; i < g->vertices; i++) {
            free_dllist(g->adjList[i]);
        }
        free(g->adjList);
        free(g);
    }
}
