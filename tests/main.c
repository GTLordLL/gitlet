#include <stdio.h>
#include <stdlib.h>
#include "my_bst.h"


int main(void){
    BinarySearchTree* myTree = create_BST();
    if (!myTree) return 1;

    printf("--- 1. 插入测试 ---\n");
    insert_bst(myTree, 50, 500);
    insert_bst(myTree, 30, 300);
    insert_bst(myTree, 70, 700);
    insert_bst(myTree, 20, 200);
    insert_bst(myTree, 40, 400);
    insert_bst(myTree, 60, 600);
    insert_bst(myTree, 80, 800);
    
    printf("当前节点数 (预期 7): %u\n", myTree->curSize);
    printf("中序遍历 (预期从小到大): ");
    print_inorderBST(myTree);

    printf("\n--- 2. 更新测试 (Key 50) ---\n");
    insert_bst(myTree, 50, 555); // 更新值
    printf("更新后的值 [50:555]: ");
    print_inorderBST(myTree);
    printf("当前节点数 (预期仍为 7): %u\n", myTree->curSize);

    printf("\n--- 3. 删除测试 ---\n");
    // 情况 1: 删除叶子节点 (20)
    printf("删除叶子节点 20...\n");
    delete_bst(myTree, 20);
    print_inorderBST(myTree);

    // 情况 2: 删除只有一个孩子的节点 (假设 30 的左孩子没了，删 30 就会剩 40)
    printf("删除只有一个孩子的节点 (逻辑测试)...\n");
    // 这里如果删除了 20，30 就只剩右孩子 40 了
    delete_bst(myTree, 30);
    print_inorderBST(myTree);

    // 情况 3: 删除有两个孩子的节点 (50)
    printf("删除根节点 (有两个孩子) 50...\n");
    delete_bst(myTree, 50);
    print_inorderBST(myTree);
    
    printf("最终节点数 (预期 4): %u\n", myTree->curSize);

    printf("\n--- 4. 遍历测试 ---\n");
    printf("前序遍历: ");
    print_preorderBST(myTree);
    printf("后序遍历: ");
    print_postorderBST(myTree);

    printf("\n清理内存...\n");
    free_BST(myTree);
    printf("测试完成。\n");

    
    return EXIT_SUCCESS;
}
