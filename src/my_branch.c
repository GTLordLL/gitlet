#include <unistd.h>
#include <stdio.h>
#include "my_branch.h"
#include "my_commit.h"
#include "my_blob.h"


// 它只是在 .gitlet/refs/heads/ 下创建一个新文件，文件内容是当前 HEAD 提交的哈希值。

int cmd_branch(const char* branch_name){
    // 1. 检查分支是否已存在
    char branch_path[256];
    sprintf(branch_path, ".gitlet/refs/heads/%s", branch_name);
    if (access(branch_path, F_OK) == 0) {
        printf("A branch with that name already exists.\n");
        return -1;
    }

    // 2. 获取当前 HEAD 提交
    Commit* head = get_head_commit();
    if (!head) {
        // 理论上只有在没 init 的情况下会发生
        return -1;
    }

    // 3. 创建分支文件，内容为当前 Commit 的哈希
    // 注意：head->hash 是 40 位字符串
    write_file(branch_path, (unsigned char*)head->hash, 40);

    free_commit(head);
    return 0;
}


