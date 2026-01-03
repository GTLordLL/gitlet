#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

// 删除 .gitlet/refs/heads/[branch_name] 文件，但是不能删除当前所在分支。
/* 深度思考：分支删除后，提交（Commits）会消失吗？
这是一个理解 Git 的好机会。
    分支只是指针：当你删除 dev 分支时，你只是删除了 .gitlet/refs/heads/dev 这个只有 40 字节的小文件。
    对象依然存在：原先属于 dev 分支的提交对象（Commits）和文件内容（Blobs）依然静静地躺在 .gitlet/objects 文件夹里。
    Dangling Commits（游离提交）：这些没有被任何分支指向的提交被称为“游离提交”。
这就是为什么你之前实现的 global-log 如此重要——即使分支被删除了，只要你没有运行垃圾回收（GC），
你依然可以通过 global-log 找到那个提交的哈希值，并通过 checkout [hash] 找回你的代码。
*/
int cmd_rm_branch(const char* branch_name) {
    // 1. 获取当前所在的 HEAD 分支
    unsigned char* head_content = read_file(".gitlet/HEAD");
    if (!head_content) {
        printf("Not in an initialized gitlet directory.\n");
        return -1;
    }

    // HEAD 格式通常是 "ref: refs/heads/master"
    // 我们需要提取出 "master" 部分
    char* current_branch = (char*)head_content + 16; 

    // 2. 安全检查：不能删除当前分支
    if (strcmp(current_branch, branch_name) == 0) {
        printf("Cannot remove the current branch.\n");
        free(head_content);
        return -1;
    }

    // 3. 拼接目标分支的路径
    char branch_path[256];
    sprintf(branch_path, ".gitlet/refs/heads/%s", branch_name);

    // 4. 检查该分支是否存在
    if (access(branch_path, F_OK) != 0) {
        printf("A branch with that name does not exist.\n");
        free(head_content);
        return -1;
    }

    // 5. 执行删除
    if (unlink(branch_path) == 0) {
        printf("Removed branch %s.\n", branch_name); // 根据规范，成功通常不打印
    } else {
        perror("Failed to remove branch");
    }

    free(head_content);
    return 0;
}
