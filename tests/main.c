#include <stdio.h>
#include <stdlib.h>
#include "my_graph.h"
#include "my_kruskal.h"
#include "my_generic_AList.h"

/**
 * 打印最小生成树的边列表
 */
void print_mst(EdgeList* mst){
    if (!mst || !mst->edges || mst->edges->currentSize == 0){
        printf("最小生成树为空！\n");
        return;
    }
    
    printf("\n=== 最小生成树的边 ===\n");
    int totalWeight = 0;
    for (unsigned int i = 0; i < mst->edges->currentSize; i++){
        Edge edge;
        get_element_GenericAList(mst->edges, i, &edge);
        printf("边 %u: (%u, %u), 权重: %d\n", i + 1, edge.u, edge.v, edge.weight);
        totalWeight += edge.weight;
    }
    printf("总权重: %d\n", totalWeight);
    printf("边的数量: %u\n\n", mst->edges->currentSize);
}

/**
 * 测试用例1: 5个顶点的无向图
 *     0 --- 1
 *     |\    |
 *     | \   |
 *     2--3--4
 */
void test_case_1(void){
    printf("========== 测试用例1: 5个顶点的无向图 ==========\n");
    
    // 创建无向图
    my_graph* graph = create_graph(5, 0);
    if (!graph){
        printf("图创建失败！\n");
        return;
    }
    
    // 添加边 (u, v, weight)
    add_edge(graph, 0, 1, 4);
    add_edge(graph, 0, 2, 2);
    add_edge(graph, 0, 3, 6);
    add_edge(graph, 1, 3, 5);
    add_edge(graph, 1, 4, 10);
    add_edge(graph, 2, 3, 1);
    add_edge(graph, 3, 4, 3);
    
    printf("原始图:\n");
    print_graph(graph);
    
    // 运行 Kruskal 算法
    EdgeList* mst = kruskal_MinimumSpanningTree(graph);
    if (mst){
        print_mst(mst);
        free_EdgeList(mst);
    }
    
    free_graph(graph);
}

/**
 * 测试用例2: 4个顶点的完全图
 */
void test_case_2(void){
    printf("========== 测试用例2: 4个顶点的完全图 ==========\n");
    
    my_graph* graph = create_graph(4, 0);
    if (!graph){
        printf("图创建失败！\n");
        return;
    }
    
    // 完全图的所有边
    add_edge(graph, 0, 1, 1);
    add_edge(graph, 0, 2, 4);
    add_edge(graph, 0, 3, 3);
    add_edge(graph, 1, 2, 2);
    add_edge(graph, 1, 3, 5);
    add_edge(graph, 2, 3, 6);
    
    printf("原始图:\n");
    print_graph(graph);
    
    // 运行 Kruskal 算法
    EdgeList* mst = kruskal_MinimumSpanningTree(graph);
    if (mst){
        print_mst(mst);
        free_EdgeList(mst);
    }
    
    free_graph(graph);
}

/**
 * 测试用例3: 6个顶点的图 (更复杂的场景)
 */
void test_case_3(void){
    printf("========== 测试用例3: 6个顶点的图 ==========\n");
    
    my_graph* graph = create_graph(6, 0);
    if (!graph){
        printf("图创建失败！\n");
        return;
    }
    
    // 添加边
    add_edge(graph, 0, 1, 4);
    add_edge(graph, 0, 2, 2);
    add_edge(graph, 1, 2, 1);
    add_edge(graph, 1, 3, 5);
    add_edge(graph, 2, 3, 8);
    add_edge(graph, 2, 4, 10);
    add_edge(graph, 3, 4, 2);
    add_edge(graph, 3, 5, 6);
    add_edge(graph, 4, 5, 3);
    
    printf("原始图:\n");
    print_graph(graph);
    
    // 运行 Kruskal 算法
    EdgeList* mst = kruskal_MinimumSpanningTree(graph);
    if (mst){
        print_mst(mst);
        free_EdgeList(mst);
    }
    
    free_graph(graph);
}

int main(void){
    printf("===============================================\n");
    printf("     Kruskal 最小生成树算法测试程序\n");
    printf("===============================================\n\n");
    
    // 运行所有测试用例
    test_case_1();
    printf("\n");
    
    test_case_2();
    printf("\n");
    
    test_case_3();
    
    printf("===============================================\n");
    printf("            所有测试完成！\n");
    printf("===============================================\n");
    
    return EXIT_SUCCESS;
}
