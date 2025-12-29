/* 2-3 树
2-3 树是红黑树的逻辑映射，所以红黑树不需要记规则，只需要知道2-3树就行了
插入发生局部崩裂，从而使树向上生长
2-节点： 在 2-3 树中的一个 2-节点，直接对应红黑树中的一个 黑色节点。
3-节点： 在 2-3 树中的一个 3-节点（含有 A 和 B 两个值），在红黑树中被表示为：
    一个 黑色父节点 连接一个 红色子节点。红链接表示3-节点的两个值相连
*/
/* 左倾红黑树 (Left-Leaning Red-Black Tree, LLRB)
在传统红黑树的基础上，规定红链接只能在左边（这就是为什么叫“左倾”）。
不能有两个连续的红链接。（2-3树中一个节点不能存三个值）
完美黑色平衡：从根到任何叶子的路径上的黑链接数量相同。
*/
#ifndef __MY_LLRBT_H__
#define __MY_LLRBT_H__

typedef enum {
    RED,
    BLACK
}NodeColor;

typedef struct LLRBTNode{
    int key;
    int value;
    struct LLRBTNode* leftChild;
    struct LLRBTNode* rightChild;
    NodeColor color; // RED or BLACK
}LLRBTNode;

typedef struct LLRedBlackTree{
    LLRBTNode* root;
    unsigned int curSize;
}LLRedBlackTree;

LLRedBlackTree* create_LLRedBlackTree(void);
void insert_LLRBTNode(LLRedBlackTree* llrbt,int key,int value);
void delete_LLRBTNode(LLRedBlackTree* llrbt,int key);
void print_inorderLLRBT(LLRedBlackTree* llrbt);
void free_LLRedBlackTree(LLRedBlackTree* llrbt);

#endif
