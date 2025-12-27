/* 左倾红黑树 (Left-Leaning Red-Black Tree, LLRB)
在普通的 BST 节点基础上，我们需要给每个节点增加一个颜色属性（红或黑）：
    红链接只能在左边（这就是为什么叫“左倾”）。
    不能有两个连续的红链接。
    完美黑色平衡：从根到任何叶子的路径上的黑链接数量相同。
*/

/* 2-3 树：红黑树的“灵魂”

*/
#ifndef __MY_LLRBT_H__
#define __MY_LLRBT_H__

#define RED 1
#define BLACK 0

typedef struct LLRBTNode{
    int key;
    int value;
    struct RBTNode* leftChild;
    struct RBTNode* rightChild;
    char color; // RED or BLACK
}LLRBTNode;

typedef struct LLRedBlackTree{
    LLRBTNode* root;
    unsigned int curSize;
}LLRedBlackTree;



#endif
