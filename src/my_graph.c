#include <stdio.h>
#include <stdlib.h>
#include "my_DLList.h"
#include "my_graph.h"
#include "my_queue.h"

my_graph* create_graph(unsigned short vertices,int isDirected){
    unsigned short i;
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

// v和u是下标（无符号16位）, weight是边的权重
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
    unsigned short v;
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
void breadth_first_search(my_graph* graph,unsigned short start_vertex){
    if (start_vertex >= graph->vertices){
        printf("BFS错误：起始顶点索引越界！\n");
        return;
    }

    // visited 数组存储每个定点是否被访问过
    unsigned short* visited = (unsigned short*)calloc(graph->vertices, sizeof(unsigned short));
    if (!visited){
        printf("visited数组内存分配失败！\n");
        return;
    }

    // 队列存储需要被需要被访问的定点索引
    DLListQueue* queue = create_DLListQueue();
    
    visited[start_vertex] = 1; // 标记起始顶点为已访问
    enqueue_DLListQueue(queue, start_vertex);

    printf("BFS 遍历序列 (从顶点 %u 开始): ", start_vertex);

    while (queue->storage->curSize > 0){

        unsigned short current_vertex = dequeue_DLListQueue(queue);
        printf("%d ", current_vertex);

        // 遍历 current_vertex 的邻居
        DLList* neighbors = graph->adjList[current_vertex];
        Node* curr = neighbors->sentinel->next;

        while (curr != neighbors->sentinel){
            unsigned short neighbor_vertex = curr->data1;
            if (!visited[neighbor_vertex]){
                visited[neighbor_vertex] = 1; // 标记为已访问
                enqueue_DLListQueue(queue, neighbor_vertex);
            }
            curr = curr->next;
        }
    }
    printf("\n");
    free(visited);
    free_DLListQueue(queue);
}

void DFS_recursive(my_graph* graph, unsigned short vertex,unsigned short* visited){
    // 1. 访问当前顶点并标记为已访问
    visited[vertex] = 1;
    printf("%d ", vertex);

    // 2. 递归访问所有未访问的邻居
    DLList* neighbors = graph->adjList[vertex];
    Node* curr = neighbors->sentinel->next;

    while (curr != neighbors->sentinel){
        unsigned short neighbor_vertex = curr->data1; // 获取邻居顶点索引

        // 3. 核心：如果邻居未被访问，则深入 (递归调用)
        if (!visited[neighbor_vertex]){
            DFS_recursive(graph, neighbor_vertex, visited);
        }
        curr = curr->next;
    }
    // 4. 函数返回时，相当于回溯 (Backtracking) 到上一个顶点
}

// 深度优先搜索（DFS）
void depth_first_search(my_graph* graph, unsigned short start_vertex){
    if (start_vertex >= graph->vertices){
        printf("DFS错误：起始顶点索引越界！\n");
        return;
    }

    // visited 数组存储每个定点是否被访问过
    unsigned short* visited = (unsigned short*)calloc(graph->vertices, sizeof(unsigned short));
    if (!visited){
        printf("visited数组内存分配失败！\n");
        return;
    }

    printf("DFS 遍历序列 (从顶点 %u 开始): ", start_vertex);
    // 启动递归
    DFS_recursive(graph, start_vertex, visited);
    printf("\n");

    free(visited);
}

unsigned int get_connected_components(my_graph* graph){
    unsigned int count = 0;
    unsigned short* visited = (unsigned short*)calloc(graph->vertices, sizeof(unsigned short));
    if (!visited){
        printf("visited数组内存分配失败！\n");
        return 0;
    }

    for (unsigned short v = 0; v < graph->vertices; v++){
        if (!visited[v]){
            // 每次找到一个未访问的顶点，说明发现了一个新的连通分量
            count++;
            printf("发现新的连通分量 #%d (从顶点 %d 开始，包含节点: ", count, v);
            // 从该顶点开始进行 DFS，标记所有可达的顶点为已访问(visited数组置位1)
            DFS_recursive(graph, v, visited);
            printf(")\n");
        }
    }
    free(visited);
    return count;
}

void free_graph(my_graph* g){
    unsigned short i;
    if (g){
        for (i = 0; i < g->vertices; i++) {
            free_dllist(g->adjList[i]);
        }
        free(g->adjList);
        free(g);
    }
}
