#include <stdio.h>
#include <stdlib.h>
#include "my_llrbt.h"

// 辅助函数：检查节点颜色
int is_red(LLRBTNode* node){
    if (!node) return 0; // 空节点视为黑色
    return node->color == RED;
}

// 使用以下三个基本操作，解决一个3-节点不能同时存在3个值，需要向上分裂一个值，
// 合并新的3-节点（红节点），然后原来的两个值分裂成独立的2-节点（黑节点）。
// 最坏情况：左旋转->右旋转->颜色反转

// 颜色翻转：对应 2-3 树中 4-节点的分裂
void move_red_up(LLRBTNode* node){
    node->color = RED;
    node->leftChild->color = BLACK;
    node->rightChild->color = BLACK;
}

// 左旋转：修复右倾的红链接，转化为右旋转需要处理的情况
LLRBTNode* rotate_left_LLRBT(LLRBTNode* parent){
    LLRBTNode* r = parent->rightChild;
    parent->rightChild = r->leftChild; // 父母继承右孩子的左孩子
    r->leftChild = parent; // 父母作为右孩子的左孩子
    r->color = parent->color; 
    parent->color = RED;
    return r;
}

// 右旋转：处理连续两个红左链接,转换为颜色翻转需要处理的情况
LLRBTNode* rotate_right_LLRBT(LLRBTNode* parent){
    LLRBTNode* l = parent->leftChild;
    parent->leftChild = l->rightChild;
    l->rightChild = parent;
    l->color = parent->color;
    parent->color = RED;
    return l;
}

/* LLRB 删除的核心思想 “借红色” (Pushing Red Down) 
为了保证删除时不破坏黑色平衡，我们必须保证当前要被删除的节点，
或者当前正在处理的节点是红色的。如果它是黑色的，
我们必须从父节点或兄弟节点那里“借”一个红色过来。
*/
// 删除时的颜色翻转
// 颜色翻转（用于删除过程中的借红）：将红色下沉
void move_red_down(LLRBTNode* node){
    node->color = BLACK;
    node->leftChild->color = RED;
    node->rightChild->color = RED;
}

// 假设 node 是红色的，node->left 和 node->left->left 都是黑色的，执行此操作使 node->left 变红
LLRBTNode* move_red_left(LLRBTNode* node) {
    move_red_down(node); // 1. 颜色翻转，把红拉下来
    // 2. 如果右兄弟有红左子，说明兄弟很富有，借一个过来
    if (node->rightChild && is_red(node->rightChild->leftChild)) { 
        node->rightChild = rotate_right_LLRBT(node->rightChild);
        node = rotate_left_LLRBT(node);
        move_red_up(node); // 恢复颜色（注意这里用回普通的 move_red_up 提上去）
    }
    return node;
}

// 假设 node 是红色的，node->right 和 node->right->left 都是黑色的，执行此操作使 node->right 变红
LLRBTNode* move_red_right(LLRBTNode* node) {
    move_red_down(node);
    // 如果左兄弟有红左子，借过来
    if (node->leftChild && is_red(node->leftChild->leftChild)) {
        node = rotate_right_LLRBT(node);
        move_red_up(node);
    }
    return node;
}

LLRedBlackTree* create_LLRedBlackTree(void){
    LLRedBlackTree* llrbt = (LLRedBlackTree*)malloc(sizeof(LLRedBlackTree));
    if (!llrbt){
        printf("LLRedBlackTree结构空间动态分配失败\n");
        return NULL;
    }
    llrbt->root = NULL;
    llrbt->curSize = 0;
    return llrbt;
}

LLRBTNode* create_LLRBTNode(int key,int value){
    LLRBTNode* node = (LLRBTNode*)malloc(sizeof(LLRBTNode));
    node->key = key;
    node->value = value;
    node->color = RED; // 新插入节点默认为红色
    node->leftChild = NULL;
    node->rightChild = NULL;
    return node;
}

LLRBTNode* rebalanceLLRBT(LLRBTNode* node){
    if (!node) return NULL;
    
    // 情况 A: 右红左黑 -> 左旋（强制左倾）
    if (is_red(node->rightChild) && !is_red(node->leftChild)) node = rotate_left_LLRBT(node);
    
    // 情况 B: 左红且左孩子的左孩子也是红 -> 右旋（平衡 3-节点）
    if (is_red(node->leftChild) && is_red(node->leftChild->leftChild)) node = rotate_right_LLRBT(node);
    
    // 情况 C: 左右皆红 -> 变色（分裂 4-节点）
    if (is_red(node->leftChild) && is_red(node->rightChild)) move_red_up(node);
    
    return node;
}

LLRBTNode* insert_recursiveLLRBT(LLRBTNode* node,int key,int value ,int* is_new){
    if (!node){
        *is_new = 1;
        return create_LLRBTNode(key,value);
    }
    // 1. 标准 BST 插入
    if (key < node->key) node->leftChild = insert_recursiveLLRBT(node->leftChild,key,value,is_new);
    else if (key > node->key) node->rightChild = insert_recursiveLLRBT(node->rightChild,key,value,is_new);
    else node->value = value; // Key已存在，更新值

    // 只要有插入动作（无论是否在深层），回溯时都要检查平衡
    if (*is_new) return rebalanceLLRBT(node);
    else return node;
}

void insert_LLRBTNode(LLRedBlackTree* llrbt,int key,int value){
    int is_new = 0;
    llrbt->root = insert_recursiveLLRBT(llrbt->root,key,value,&is_new);
    if (is_new) llrbt->curSize++;
}

LLRBTNode* find_minLLRBTNode(LLRBTNode* node){
    if (!node) return NULL;
    while (node->leftChild) node = node->leftChild;
    return node;
}

// 专门用于删除最小节点的函数
LLRBTNode* delete_min_recursive(LLRBTNode* node){
    if (!node->leftChild){
        free(node);
        return NULL;
    }

    // 往左走之前，检查左边是否太穷
    if (!is_red(node->leftChild) && !is_red(node->leftChild->leftChild)){
        node = move_red_left(node);
    }

    node->leftChild = delete_min_recursive(node->leftChild);
    return rebalanceLLRBT(node);
}

LLRBTNode* delete_recursiveLLRBT(LLRBTNode* node,int key,int* delete_success){
    if (!node) return NULL; // 安全性检查
    if (key < node->key){
        // 往左走之前，检查左边是不是太穷（全黑）
        if (node->leftChild && !is_red(node->leftChild) && !is_red(node->leftChild->leftChild)){
            node = move_red_left(node);
        }
        node->leftChild = delete_recursiveLLRBT(node->leftChild,key,delete_success);
    }else{
        // 往右走之前，如果是左倾红树，先右旋让右边容易操作
        // 如果左孩子是红，先右旋。这有两个目的：
        // a. 将红色推向右边，方便后面删除
        // b. 如果 key == node->key，旋转后 node 会沉下去，方便在叶子处删除
        if (is_red(node->leftChild)){
            node = rotate_right_LLRBT(node);
        }

        // 找到了且是叶子节点（在右旋之后，node 可能是被换下来的那个）
        if (key == node->key && (node->rightChild == NULL)){
            *delete_success = 1;
            free(node);
            return NULL;
        }

        // 往右走之前，检查右边是不是太穷,是否需要借红
        if (node->rightChild && !is_red(node->rightChild) && !is_red(node->rightChild->leftChild)){
            node = move_red_right(node);
        }

        if (key == node->key){
            *delete_success = 1;
            // 找到中间节点，用右子树最小节点替换
            LLRBTNode* min_node = find_minLLRBTNode(node->rightChild);
            node->key = min_node->key;
            node->value = min_node->value;
            
            // 删除右子树中的最小节点
            node->rightChild = delete_min_recursive(node->rightChild);
        }else{
            node->rightChild = delete_recursiveLLRBT(node->rightChild,key,delete_success);
        }
    }
    return rebalanceLLRBT(node);
}

void delete_LLRBTNode(LLRedBlackTree* llrbt,int key){
    if (!llrbt->root) return;
    int delete_success = 0;

    // 【重要】LLRB删除的特殊前置操作
    // 如果根节点是黑色的，且两个孩子也是黑的（或孩子不存在），我们人为把它染红。
    // 这样才能保证 move_red_down 有红色可以推下去。
    if (!is_red(llrbt->root) && !is_red(llrbt->root->leftChild) && !is_red(llrbt->root->rightChild)) {
        llrbt->root->color = RED;
    }

    llrbt->root = delete_recursiveLLRBT(llrbt->root, key, &delete_success);
    
    // 无论如何，根节点最后必须变黑
    if (llrbt->root) llrbt->root->color = BLACK;
    
    if (delete_success) llrbt->curSize--;
}

// 中序遍历 (In-order Traversal)
void inorder_recursiveLLRBT(LLRBTNode* node){
    if (!node) return;
    inorder_recursiveLLRBT(node->leftChild);
    printf("[%d:%d] ", node->key, node->value);
    inorder_recursiveLLRBT(node->rightChild);
}

void print_inorderLLRBT(LLRedBlackTree* llrbt){
    if (!llrbt->root) printf("Empty Tree");
    inorder_recursiveLLRBT(llrbt->root);
    printf("\n");
}

void free_recursiveLLRBT(LLRBTNode* node){
    if (!node) return;
    free_recursiveLLRBT(node->leftChild);
    free_recursiveLLRBT(node->rightChild);
    free(node);
}

void free_LLRedBlackTree(LLRedBlackTree* llrbt){
    if (!llrbt) return;
    free_recursiveLLRBT(llrbt->root);
    free(llrbt);
}

