#include <stdio.h>
#include <stdlib.h>
#include "my_graph.h"
#include "dijkstra.h"


int main(void){
    // 创建一个包含 5 个顶点的有向图 (0-4)
    // 0: A, 1: B, 2: C, 3: D, 4: E
    my_graph* g = create_graph(5, 1); // 1 = 有向图

    printf("--- 构建图 (带权重) ---\n");
    // 构建一个经典的测试用例
    add_edge(g, 0, 1, 10); // A -> B (10)
    add_edge(g, 0, 4, 5);  // A -> E (5)
    add_edge(g, 1, 2, 1);  // B -> C (1)
    add_edge(g, 1, 4, 2);  // B -> E (2)
    add_edge(g, 2, 3, 4);  // C -> D (4)
    add_edge(g, 4, 1, 3);  // E -> B (3) - 关键边：会让 A->E->B (5+3=8) 比 A->B (10) 更短
    add_edge(g, 4, 2, 9);  // E -> C (9)
    add_edge(g, 4, 3, 2);  // E -> D (2) - 关键边：会让 A->E->D (5+2=7) 最短

    /* 图解预期路径：
       A(0) -> E(4) : 5
       A(0) -> E(4) -> B(1) : 5 + 3 = 8 (优于直接 A->B 的 10)
       A(0) -> E(4) -> B(1) -> C(2) : 5 + 3 + 1 = 9
       A(0) -> E(4) -> D(3) : 5 + 2 = 7
    */

    print_graph(g);

    printf("\n--- 运行 Dijkstra (起点: 0) ---\n");
    ShortestPathResult* result = create_dijkstraResult(g, 0);

    // 打印到所有点的最短路径
    for (int i = 0; i < 5; i++) {
        print_shortest_path(result, i);
    }

    free_dijkstraResult(result);
    free_graph(g);
    
    return EXIT_SUCCESS;
}
