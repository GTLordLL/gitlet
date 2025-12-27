#include <stdio.h>
#include <stdlib.h>
#include "my_avlt.h"

// 辅助函数：打印树的根节点及高度，验证平衡性
void verify_balance(my_AVLTree* avlt) {
    if (avlt->root) {
        printf("当前根节点: [%d], 树总高度: %d, 节点总数: %u\n", 
                avlt->root->key, avlt->root->height, avlt->curSize);
    } else {
        printf("树为空\n");
    }
}


int main(void){
    my_AVLTree* avlt = create_AVLTree();

    printf("--- 测试 1: 顺序插入 1 到 10 ---\n");
    // 如果是普通 BST，高度会是 10；AVL 树高度应在 4 左右
    for (int i = 1; i <= 10; i++) {
        insert_AVLTNode(avlt, i, i * 100);
    }
    
    printf("中序遍历结果: ");
    print_inorderAVLT(avlt);
    verify_balance(avlt);
    printf("\n");

    printf("--- 测试 2: 删除节点 ---\n");
    // 删除根节点或其他节点，观察是否触发再平衡
    printf("删除节点 4 (此时可能会触发旋转)...\n");
    delete_AVLTNode(avlt, 4);
    printf("删除节点 8...\n");
    delete_AVLTNode(avlt, 8);
    
    printf("中序遍历结果: ");
    print_inorderAVLT(avlt);
    verify_balance(avlt);
    printf("\n");

    printf("--- 测试 3: 覆盖更新 ---\n");
    insert_AVLTNode(avlt, 5, 9999);
    printf("更新 key 5 的值为 9999 后: ");
    print_inorderAVLT(avlt);

    free_AVLTree(avlt);
    printf("\n测试完成，内存已释放。\n");

    
    return EXIT_SUCCESS;
}
