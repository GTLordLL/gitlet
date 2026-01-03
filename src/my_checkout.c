#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include "my_checkout.h"
#include "my_commit.h"
#include "my_stage.h"
#include "my_str_hashmap.h"
#include "my_blob.h"

// 用法 1: 从 HEAD 恢复单个文件
int cmd_checkout_file(const char* filename){
    // 1. 获取 HEAD 提交
    Commit* head = get_head_commit();
    if (!head) return -1;

    // 2. 在该提交的 blobs 中查找文件名对应的哈希
    char* blob_hash = get_str_value(head->blobs,filename);
    if (!blob_hash) {
        printf("File does not exist in that commit.\n");
        free_commit(head);
        return -1;
    }

    // 3. 从 .gitlet/objects 读取 Blob 内容
    char object_path[256];
    sprintf(object_path, ".gitlet/objects/%s", blob_hash);
    unsigned char* content = read_file(object_path);
    long size = get_file_size(object_path);

    // 4. 写回工作目录（覆盖当前文件）
    write_file(filename, content, size);

    free(content);
    free_commit(head);
    return 0;
}

// 用法 2: 从特定commit提交恢复单个文件
int cmd_checkout_commit_file(const char* commit_id, const char* filename){
    // 1. 根据哈希读取特定的提交 (支持简短 ID 暂不实现，先用 40 位全称)
    Commit* target_commit = get_commit_by_hash(commit_id);
    if (!target_commit) {
        printf("No commit with that id exists.\n");
        return -1;
    }

    // 2. 查找文件对应的 Blob
    char* blob_hash = get_str_value(target_commit->blobs, filename);
    if (!blob_hash) {
        printf("File does not exist in that commit.\n");
        free_commit(target_commit);
        return -1;
    }

    // 3. 读取并恢复
    char object_path[256];
    sprintf(object_path, ".gitlet/objects/%s", blob_hash);
    unsigned char* content = read_file(object_path);
    long size = get_file_size(object_path);

    // 4. 写回工作目录（覆盖当前文件）
    write_file(filename, content, size);

    free(content);
    free_commit(target_commit);
    return 0;
}

// 辅助函数：判断文件是否属于当前工作目录的“受管理文件”
// 排除 .gitlet 目录、可执行文件、脚本等（在 Gitlet 实验中通常只关心 a.txt 等普通文件）


