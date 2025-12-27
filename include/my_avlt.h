/* 平衡二叉树
所有节点的左子树高度-右子树高度=平衡因子
平衡因子等于-1,0,1时，这颗二叉树就是平衡的。
查找，插入，构建，删除的过程和BST一致，只是失衡的时候需要调整：
    左旋转和右旋转
失衡类型：
LL型：失衡节点的平衡因子=2,失衡节点左孩子的平衡因子=1。右旋转失衡节点
RR型：失衡节点的平衡因子=-2,失衡节点右孩子的平衡因子=-1。左旋转失衡节点
LR型：失衡节点的平衡因子=2,失衡节点左孩子的平衡因子=-1。先左旋转左孩子，再右旋转失衡节点
RL型：失衡节点的平衡因子=-2,失衡节点右孩子的平衡因子=1。先右旋转右孩子，再左旋转失衡节点
*/
#ifndef __MY_AVLT_H__
#define __MY_AVLT_H__

typedef struct AVLTNode{
    int key;
    int value;
    int height; // 记录当前节点为根的树高度
    struct AVLTNode* leftChild;
    struct AVLTNode* rightChild;
}AVLTNode;

typedef struct my_AVLTree{
    AVLTNode* root;
    unsigned int curSize;
}my_AVLTree;

my_AVLTree* create_AVLTree(void);
void insert_AVLTNode(my_AVLTree* avlt,int key,int value);
void delete_AVLTNode(my_AVLTree* avlt,int key);
void print_inorderAVLT(my_AVLTree* avlt);
void free_AVLTree(my_AVLTree* avlt);

#endif
