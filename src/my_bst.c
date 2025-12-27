#include <stdio.h>
#include <stdlib.h>
#include "my_bst.h"


BinarySearchTree* create_BST(void){
    BinarySearchTree* bst = (BinarySearchTree*)malloc(sizeof(BinarySearchTree));
    if (!bst){
        printf("BinarySearchTree结构空间动态分配失败\n");
        return NULL;
    }
    bst->root = NULL;
    bst->curSize = 0;
    return bst;
}

BSTNode* create_BSTNode(int key, int value){
    BSTNode* node = (BSTNode*)malloc(sizeof(BSTNode));
    node->key = key;
    node->value = value;
    node->leftChild = NULL;
    node->rightChild = NULL;
    return node;
}

BSTNode* insert_recursive(BSTNode* node,int key,int value ,char* is_new){
    if (!node){
        *is_new = 1;
        return create_BSTNode(key,value);
    }
    if (key < node->key) node->leftChild = insert_recursive(node->leftChild,key,value,is_new);
    else if (key > node->key) node->rightChild = insert_recursive(node->rightChild,key,value,is_new);
    else node->value = value; // Key已存在，更新值
    
    return node;
}

void insert_bst(BinarySearchTree* bst,int key,int value){
    char is_new = 0;
    bst->root = insert_recursive(bst->root,key,value,&is_new);
    if (is_new) bst->curSize++;
}

BSTNode* find_minBSTNode(BSTNode* node){
    if (!node) return NULL;
    while (node->leftChild) node = node->leftChild;
    return node;
}

BSTNode* delete_recursive(BSTNode* node,int key,char* delete_success){
    if (!node)return NULL;
    
    if (key < node->key){
        node->leftChild = delete_recursive(node->leftChild,key,delete_success);
    }else if (key > node->key){
        node->rightChild = delete_recursive(node->rightChild,key,delete_success);
    }else{
        *delete_success = 1;
        // 情况 1 & 2: 只有一个孩子或无孩子
        if (!node->leftChild){
            BSTNode* temp = node->rightChild;
            free(node);
            return temp;
        }
        if (!node->rightChild){
            BSTNode* temp = node->leftChild;
            free(node);
            return temp;
        }
        // 情况 3: 有两个孩子
        BSTNode* temp = find_minBSTNode(node->rightChild);
        // 替换为右子树的后继
        node->key = temp->key;
        node->value = temp->value;
        // 删除右子树的后继
        char dummy;
        node->rightChild = delete_recursive(node->rightChild,temp->key,&dummy);
    }
    return node;
}

void delete_bst(BinarySearchTree* bst,int key){
    char delete_success = 0;
    bst->root = delete_recursive(bst->root,key,&delete_success);
    if (delete_success) bst->curSize--;
}

// 中序遍历 (In-order Traversal)
// BST 有一个神奇的特性：如果你按照 左 -> 根 -> 右 的顺序遍历，输出的结果一定是从小到大排序的。
void inorder_recursive(BSTNode* node){
    if (!node) return;
    inorder_recursive(node->leftChild);
    printf("[%d:%d] ", node->key, node->value);
    inorder_recursive(node->rightChild);
}

void print_inorderBST(BinarySearchTree* bst){
    if (!bst->root) printf("Empty Tree");
    inorder_recursive(bst->root);
    printf("\n");
}

// 前序遍历 (Preorder traversal)
void preorder_recursive(BSTNode* node){
    if (!node) return;
    printf("[%d:%d] ", node->key, node->value);
    preorder_recursive(node->leftChild);
    preorder_recursive(node->rightChild);
}

void print_preorderBST(BinarySearchTree* bst){
    if (!bst->root) printf("Empty Tree");
    preorder_recursive(bst->root);
    printf("\n");
}

// 后序遍历 (postorder traversal)
void postorder_recursive(BSTNode* node){
    if (!node) return;
    postorder_recursive(node->leftChild);
    postorder_recursive(node->rightChild);
    printf("[%d:%d] ", node->key, node->value);
}

void print_postorderBST(BinarySearchTree* bst){
    if (!bst->root) printf("Empty Tree");
    postorder_recursive(bst->root);
    printf("\n");
}

void free_recursive(BSTNode* node){
    if (!node) return;
    free_recursive(node->leftChild);
    free_recursive(node->rightChild);
    free(node);
}

void free_BST(BinarySearchTree* bst){
    if (!bst) return;
    free_recursive(bst->root);
    free(bst);
}

