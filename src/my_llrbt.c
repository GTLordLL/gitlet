#include <stdio.h>
#include <stdlib.h>
#include "my_llrbt.h"

// 辅助函数：检查节点颜色
char is_red(LLRBTNode* node){
    if (!node) return BLACK;
    return node->color == RED;
}

// --- 操作 1: 左旋转 ---

