#include <stdio.h>
#include <stdlib.h>
#include "my_graph.h"

int main(void){
    // 创建一个包含 5 个顶点的无向图
    // 0: A, 1: B, 2: C, 3: D, 4: E
    my_graph* g = create_graph(5, 0); 

    printf("--- 构建图 ---\n");
    add_edge(g, 0, 1, 10); // A-B
    add_edge(g, 0, 4, 5);  // A-E
    add_edge(g, 1, 2, 6);  // B-C
    add_edge(g, 1, 3, 2);  // B-D
    add_edge(g, 1, 4, 3);  // B-E
    add_edge(g, 3, 4, 8);  // D-E

    print_graph(g);

    /* 预期输出：
    顶点 0: -> 1 -> 4
    顶点 1: -> 0 -> 2 -> 3 -> 4
    顶点 2: -> 1
    顶点 3: -> 1 -> 4
    顶点 4: -> 0 -> 1 -> 3
    */

    free_graph(g);

    return EXIT_SUCCESS; // 规定返回0,为运行成功，而且必须为int类型
}
