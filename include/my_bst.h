/* 二叉搜索树 (Binary Search Tree, BST)
BST 的核心规则
    左小：左子树的所有节点值都小于当前节点。
    右大：右子树的所有节点值都大于当前节点。
    递归定义：左右子树本身也必须是 BST。
*/
#ifndef __MY_BST_H__
#define __MY_BST_H__


// BST 节点结构
typedef struct BSTNode{
    int key;
    int value;
    struct BSTNode* leftChild;
    struct BSTNode* rightChild;
}BSTNode;

// BST 包装结构
typedef struct BinarySearchTree{
    BSTNode* root;
    unsigned int curSize;
}BinarySearchTree;

BinarySearchTree* create_BST(void);
void insert_bst(BinarySearchTree* bst,int key,int value);
void delete_bst(BinarySearchTree* bst,int key);
void print_inorderBST(BinarySearchTree* bst);
void print_preorderBST(BinarySearchTree* bst);
void print_postorderBST(BinarySearchTree* bst);
void free_BST(BinarySearchTree* bst);

#endif
