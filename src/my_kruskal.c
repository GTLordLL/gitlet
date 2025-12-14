#include <stdlib.h>
#include <stdio.h>
#include "my_kruskal.h"
#include "my_graph.h"
#include "my_unionFindSet.h"
#include "my_generic_AList.h"
#include "my_DLList.h"


EdgeList* create_EdgeList(unsigned int initialCapacity){
    EdgeList* edgeList = (EdgeList*)malloc(sizeof(EdgeList));
    if (!edgeList){
        printf("EdgeList内存分配失败！\n");
        return NULL;
    }
    
    edgeList->edges = create_GenericAList(initialCapacity,sizeof(Edge));
    if (!edgeList->edges){
        printf("EdgeList的GenericAList创建失败！\n");
        free(edgeList);
        return NULL;
    }
    
    return edgeList;
}

void add_edge_EdgeList(EdgeList* edgeList, Edge* edge){
    add_last_GenericAList(edgeList->edges, edge);
}

void free_EdgeList(EdgeList* edgeList){
    if (edgeList){
        free_GenericAList(edgeList->edges);
        free(edgeList);
    }
}

// --- Kruskal's Step 1: 提取边 ---
// Kruskal 算法一般只需要无向图的边列表。
// 对于有向图，边 (u, v) 和 (v, u) 被视为不同的边。
// 对于无向图，边 (u, v) 和 (v, u) 只添加一次。
EdgeList* extract_edges_from_graph(my_graph* graph){
    EdgeList* edgeList = create_EdgeList(graph->edges);
    if (!edgeList){
        printf("EdgeList创建失败！\n");
        return NULL;
    }
    
    for (unsigned short u = 0; u < graph->vertices; u++){
        DLList* neighbors = graph->adjList[u];
        DLListNode* current = neighbors->sentinel->next;
        while (current != neighbors->sentinel){
            unsigned short v = (unsigned short)current->data1;
            int weight = current->data2;
            
            /*
            graph->isDirected 为 0 表示无向图
            graph->isDirected 为 1 表示有向图
            如果是有向图，直接添加边 (u, v)
            如果是无向图，只添加 u < v 的边，避免重复
            */
            if (graph->isDirected || u < v){
                Edge edge;
                edge.u = u;
                edge.v = v;
                edge.weight = weight;
                add_edge_EdgeList(edgeList, &edge);
            }
            current = current->next;
        }
    }
    return edgeList;
}

// --- Kruskal's Step 2: 排序边的权重 ---

// 辅助函数：比较两个边的权重，用于 qsort
int compare_edges(const void* a, const void* b){
    Edge* edgeA = (Edge*)a;
    Edge* edgeB = (Edge*)b;
    if (edgeA->weight < edgeB->weight) return -1;
    if (edgeA->weight > edgeB->weight) return 1;
    return 0;  // 避免整数溢出
}

/* 使用 C 标准库的 qsort快速排序 进行高效排序 (O(E log E))
qsort(array, count, size, compare_function);
array - 要排序的数组指针
count - 元素个数
size - 每个元素的大小（字节）
compare_function - 比较函数指针
*/
void sort_edges_by_weight(EdgeList* edgeList){
    qsort(edgeList->edges->data, edgeList->edges->currentSize, edgeList->edges->elementSize, compare_edges);
}

// --- Kruskal's Step 3: 构建最小生成树 (MST) ---
EdgeList* kruskal_MinimumSpanningTree(my_graph* graph){
    // 1. 提取图中的所有边 (O(V+E))
    EdgeList* edgeList = extract_edges_from_graph(graph);
    if (!edgeList){
        printf("EdgeList为空！\n");
        return NULL;
    }
    
    // 2. 按权重对边进行排序 (O(E log E))
    sort_edges_by_weight(edgeList);
    
    // 3. 初始化并查集，用于检测环路 (O(V))
    DisjointSet* ufs = create_unionFindSet(graph->vertices);
    if (!ufs){
        printf("DisjointSet创建失败！\n");
        free_EdgeList(edgeList);
        return NULL;
    }
    
    // 4. 构建最小生成树的边列表
    EdgeList* mstEdgeList = create_EdgeList(graph->vertices - 1); // MST 最多有 V-1 条边
    if (!mstEdgeList){
        printf("MST EdgeList创建失败！\n");
        free_EdgeList(edgeList);
        free_DisjointSet(ufs);
        return NULL;
    }
    
    // 5. 遍历排序后的边，选择不形成环路的边加入 MST (O(E * alpha(V)))
    for (unsigned int i = 0; i < edgeList->edges->currentSize; i++){
        Edge edge;
        get_element_GenericAList(edgeList->edges, i, &edge);
        
        unsigned int setU = find_root(ufs, edge.u);
        unsigned int setV = find_root(ufs, edge.v);
        
        // 如果 u 和 v 不在同一个集合中，添加该边到 MST 并合并集合
        if (setU != setV){
            add_edge_EdgeList(mstEdgeList, &edge);
            union_sets(ufs, setU, setV);
        }
    }
    
    // 6. 清理资源
    free_EdgeList(edgeList);
    free_DisjointSet(ufs);
    
    return mstEdgeList;
}
