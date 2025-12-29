#include <stdio.h>
#include <stdlib.h>
#include "my_llrbt.h"

// 辅助测试：以物理结构形式打印树（旋转 90 度看）
// R 代表该节点与父节点之间是红链接
void print_tree_struct(LLRBTNode* node, int level) {
    if (node == NULL) return;
    print_tree_struct(node->rightChild, level + 1);
    for (int i = 0; i < level; i++) printf("    ");
    printf("%d(%s)\n", node->key, node->color == RED ? "R" : "B");
    print_tree_struct(node->leftChild, level + 1);
}


int main(void){
    LLRedBlackTree* tree = create_LLRedBlackTree();
    if (!tree) return 1;

    printf("=== 1. 顺序插入测试 (1 到 10) ===\n");
    printf("顺序插入会不断触发左旋、右旋和颜色翻转以保持 2-3 树的逻辑结构。\n");
    for (int i = 1; i <= 10; i++) {
        insert_LLRBTNode(tree, i, i * 10);
    }

    printf("当前树节点总数: %u\n", tree->curSize);
    printf("中序遍历 (应为升序): ");
    print_inorderLLRBT(tree);

    printf("\n树的物理结构 (R=红链接节点, B=普通黑节点):\n");
    print_tree_struct(tree->root, 0);

    printf("\n=== 2. 删除测试 ===\n");
    // 删除叶子
    printf("删除 key=1 (叶子)...\n");
    delete_LLRBTNode(tree, 1);
    
    // 删除中间节点（触发 delete_min_recursive 和 rebalance）
    printf("删除 key=4 (内部节点)...\n");
    delete_LLRBTNode(tree, 4);

    // 删除当前根节点
    printf("删除 key=2 (可能导致根变换)...\n");
    delete_LLRBTNode(tree, 2);

    printf("\n删除后的中序遍历: ");
    print_inorderLLRBT(tree);
    
    printf("当前树节点总数: %u\n", tree->curSize);
    printf("操作后的物理结构:\n");
    print_tree_struct(tree->root, 0);

    printf("\n=== 3. 释放资源 ===\n");
    free_LLRedBlackTree(tree);
    printf("内存已释放，测试结束。\n");

    
    return EXIT_SUCCESS;
}
