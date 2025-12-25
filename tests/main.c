#include <stdio.h>
#include <stdlib.h>
#include "my_priorityQueue.h"


int main(void){
    printf("--- 开始优先级队列测试 ---\n");

    MinBinaryHeap* pq = create_MinBinaryHeap(10);
    if (!pq) return -1;

    // 1. 测试插入 (Insert)
    // 模拟: 插入顶点ID和距离
    heap_insert(pq, 1, 100);
    heap_insert(pq, 2, 50);
    heap_insert(pq, 3, 80);
    heap_insert(pq, 4, 300);

    // 此时堆顶应该是 2 (距离50)
    
    // 2. 测试修改优先级 (Decrease Key)
    // 这是一个关键测试：如果哈希表同步有问题，这里会找不到节点或者更新错人
    printf("将顶点 4 的距离从 300 修改为 10 (使其成为最小)\n");
    heap_change_priority(pq, 4, 10);

    printf("将顶点 1 的距离从 100 修改为 120 (增大距离，测试下沉)\n");
    heap_change_priority(pq, 1, 120);

    // 3. 测试提取 (Extract Min)
    printf("\n依次提取最小元素 (预期顺序: 4->2->3->1):\n");
    
    PQNode node;
    while (!is_empty(pq)) {
        heap_extract_min(pq, &node);
        printf("提取: 顶点ID = %u, 距离 = %u\n", node.vertex, node.distance);
    }

    // 4. 内存释放测试
    free_MinBinaryHeap(pq);
    printf("\n--- 测试结束，内存已释放 ---\n");
    
    return EXIT_SUCCESS;
}
