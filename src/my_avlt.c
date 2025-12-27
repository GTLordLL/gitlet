#include <stdio.h>
#include <stdlib.h>
#include "my_avlt.h"

// 获取高度的辅助函数（处理 NULL 情况）
int get_height(AVLTNode* node){
    return node ? node->height : 0;
}

// 计算平衡因子：左高 - 右高
int get_balance_factor(AVLTNode* node){
    return node ? (get_height(node->leftChild) - get_height(node->rightChild)) : 0;
}

void update_height(AVLTNode* node){
    int leftHeight = get_height(node->leftChild);
    int rightHeight = get_height(node->rightChild);
    node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

// 右旋转 (Right Rotate) - 修复 LL 型
AVLTNode* rotate_right(AVLTNode* parent){
    AVLTNode* l = parent->leftChild;
    AVLTNode* lr = l->rightChild;

    l->rightChild = parent; // 父母作为左孩子的右孩子
    parent->leftChild = lr; // 左孩子的右孩子作为父母的左孩子

    update_height(parent);
    update_height(l);
    return l; // 原来父母的左孩子现在作为父母
}

// 左旋转 (Left Rotate) - 修复 RR 型
AVLTNode* rotate_left(AVLTNode* parent){
    AVLTNode* r = parent->rightChild;
    AVLTNode* rl = r->leftChild;

    r->leftChild = parent;
    parent->rightChild = rl;

    update_height(parent);
    update_height(r);
    return r;
}

my_AVLTree* create_AVLTree(void){
    my_AVLTree* avlt = (my_AVLTree*)malloc(sizeof(my_AVLTree));
    if (!avlt){
        printf("my_AVLTree结构空间动态分配失败\n");
        return NULL;
    }
    avlt->root = NULL;
    avlt->curSize = 0;
    return avlt;
}

AVLTNode* create_AVLTNode(int key, int value){
    AVLTNode* node = (AVLTNode*)malloc(sizeof(AVLTNode));
    node->key = key;
    node->value = value;
    node->leftChild = NULL;
    node->rightChild = NULL;
    node->height = 1;
    return node;
}

AVLTNode* rebalance(AVLTNode* node){
    if (!node) return NULL;
    // 1. 更新高度
    update_height(node);

    // 2. 获取平衡因子
    int balance_factor = get_balance_factor(node);

    // 3. 判断失衡类型并旋转
    // LL型 或 LR型：左边太重 (bf > 1)
    if (balance_factor > 1) {
        // 如果左孩子也是左边重 (bf >= 0)，则是 LL 型 -> 右旋
        // 注意：删除操作时，左孩子 bf 可能是 0，这也是 LL 型
        if (get_balance_factor(node->leftChild) >= 0) {
            return rotate_right(node);
        } else {
            // 左孩子是右边重 (bf < 0)，则是 LR 型 -> 先左旋后右旋
            node->leftChild = rotate_left(node->leftChild);
            return rotate_right(node);
        }
    }

    // RR型 或 RL型：右边太重 (bf < -1)
    if (balance_factor < -1) {
        // 如果右孩子也是右边重 (bf <= 0)，则是 RR 型 -> 左旋
        if (get_balance_factor(node->rightChild) <= 0) {
            return rotate_left(node);
        } else {
            // 右孩子是左边重 (bf > 0)，则是 RL 型 -> 先右旋后左旋
            node->rightChild = rotate_right(node->rightChild);
            return rotate_left(node);
        }
    }
    return node; // 已经是平衡的
}

AVLTNode* insert_recursiveAVLT(AVLTNode* node,int key,int value ,int* is_new){
    // 1. 标准 BST 插入逻辑
    if (!node){
        *is_new = 1;
        return create_AVLTNode(key,value);
    }
    if (key < node->key) node->leftChild = insert_recursiveAVLT(node->leftChild,key,value,is_new);
    else if (key > node->key) node->rightChild = insert_recursiveAVLT(node->rightChild,key,value,is_new);
    else node->value = value; // Key已存在，更新值

    // 只要有插入动作（无论是否在深层），回溯时都要检查平衡
    if (*is_new) return rebalance(node);
    else return node;
}

void insert_AVLTNode(my_AVLTree* avlt,int key,int value){
    int is_new = 0;
    avlt->root = insert_recursiveAVLT(avlt->root,key,value,&is_new);
    if (is_new) avlt->curSize++;
}

AVLTNode* find_minAVLTNode(AVLTNode* node){
    if (!node) return NULL;
    while (node->leftChild) node = node->leftChild;
    return node;
}

AVLTNode* delete_recursiveAVLT(AVLTNode* node,int key,int* delete_success){
    if (!node)return NULL;
    if (key < node->key) node->leftChild = delete_recursiveAVLT(node->leftChild,key,delete_success);
    else if (key > node->key) node->rightChild = delete_recursiveAVLT(node->rightChild,key,delete_success);
    else{
        *delete_success = 1;
        // 情况 1 & 2: 只有一个孩子或无孩子
        if (!node->leftChild){
            AVLTNode* temp = node->rightChild;
            free(node);
            return temp;
        }
        if (!node->rightChild){
            AVLTNode* temp = node->leftChild;
            free(node);
            return temp;
        }
        // 情况 3: 有两个孩子
        AVLTNode* temp = find_minAVLTNode(node->rightChild);
        // 替换为右子树的后继
        node->key = temp->key;
        node->value = temp->value;
        // 删除右子树的后继
        int dummy;
        node->rightChild = delete_recursiveAVLT(node->rightChild,temp->key,&dummy);
    }
    if (*delete_success) return rebalance(node);
    else return node;
}

void delete_AVLTNode(my_AVLTree* avlt,int key){
    int delete_success = 0;
    avlt->root = delete_recursiveAVLT(avlt->root,key,&delete_success);
    if (delete_success) avlt->curSize--;
}

// 中序遍历 (In-order Traversal)
void inorder_recursiveAVLT(AVLTNode* node){
    if (!node) return;
    inorder_recursiveAVLT(node->leftChild);
    printf("[%d:%d] ", node->key, node->value);
    inorder_recursiveAVLT(node->rightChild);
}

void print_inorderAVLT(my_AVLTree* avlt){
    if (!avlt->root) printf("Empty Tree");
    inorder_recursiveAVLT(avlt->root);
    printf("\n");
}

void free_recursiveAVLT(AVLTNode* node){
    if (!node) return;
    free_recursiveAVLT(node->leftChild);
    free_recursiveAVLT(node->rightChild);
    free(node);
}

void free_AVLTree(my_AVLTree* avlt){
    if (!avlt) return;
    free_recursiveAVLT(avlt->root);
    free(avlt);
}

