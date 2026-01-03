#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include "my_checkout.h"
#include "my_commit.h"
#include "my_str_hashmap.h"
#include "my_blob.h"
#include "my_str_DLList.h"

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
int is_regular_file(const char* filename){
    if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) return 0;
    if (strcmp(filename, "bin") == 0 || strcmp(filename, ".gitlet") == 0) return 0;
    // 你可以根据需要增加排除项，比如 test.sh, Makefile 等
    if (strstr(filename, ".sh") || strcmp(filename, "Makefile") == 0) return 0;
    return 1;
}

int is_file_untracked(Commit* current_commit,Commit* target_commit){
    DIR* d = opendir(".");
    struct dirent* dir;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (is_regular_file(dir->d_name)) {
                // 如果文件在工作区，但不在当前 Commit 中（Untracked）
                if (get_str_value(current_commit->blobs, dir->d_name) == NULL) {
                    // 且该文件在目标分支提交中存在
                    if (get_str_value(target_commit->blobs, dir->d_name) != NULL) {
                        // 必须报错并退出，因为直接切换会把这个未追踪的文件覆盖掉。
                        printf("There is an untracked file in the way; delete it, or add and commit it first.\n");
                        closedir(d);
                        free_commit(current_commit);
                        free_commit(target_commit);
                        return 1;
                    }
                }
            }
        }
        closedir(d);
    }
    return 0;
}

// 用法 3: 切换到另一个分支，并将整个工作目录更新为该分支最新提交的状态。
int cmd_checkout_branch(const char* branch_name){
    // 1. 检查分支文件是否存在
    char target_branch_path[256];
    sprintf(target_branch_path, ".gitlet/refs/heads/%s", branch_name);
    if (access(target_branch_path, F_OK) != 0) {
        printf("No such branch exists.\n");
        return -1;
    }

    // 2. 检查是否已经是当前分支
    unsigned char* head_content = read_file(".gitlet/HEAD");
    char current_branch_ref[256];
    sprintf(current_branch_ref, "ref: refs/heads/%s", branch_name);
    if (strcmp((char*)head_content, current_branch_ref) == 0) {
        printf("No need to checkout the current branch.\n");
        free(head_content);
        return -1;
    }
    free(head_content);

    // 3. 获取 HEAD 提交和目标分支提交
    Commit* current_commit = get_head_commit();
    if (!current_commit){
        printf("无法获取当前head提交\n");
        return -1;
    }
    

    unsigned char* target_hash_raw = read_file(target_branch_path);
    char target_hash[41];
    memcpy(target_hash, target_hash_raw, 40);
    target_hash[40] = '\0';
    free(target_hash_raw);
    Commit* target_commit = get_commit_by_hash(target_hash);
    if (target_commit == NULL) {
        printf("No commit with that id exists.\n");
        free_commit(current_commit);
        // 做一些清理工作，比如 closedir(directory);
        return -1;
    }

    // 4. 安全性检查：是否存在会被覆盖的 Untracked 文件
    if (is_file_untracked(current_commit,target_commit)) return -1;

    // 5. 更新工作目录：覆盖或新增目标分支的文件
    // 遍历 target_commit 的 blobs
    unsigned int i;
    for (i = 0; i < target_commit->blobs->maxCapacity; i++) {
        StringDLLNode* curr = target_commit->blobs->buckets[i]->sentinel->next;
        while (curr != target_commit->blobs->buckets[i]->sentinel) {
            char* filename = curr->str1;
            char* blob_hash = curr->str2;
            
            char obj_path[256];
            sprintf(obj_path, ".gitlet/objects/%s", blob_hash);
            unsigned char* content = read_file(obj_path);
            long size = get_file_size(obj_path);
            write_file(filename, content, size);
            free(content);
            
            curr = curr->next;
        }
    }

    // 6. 删除在当前分支有，但在目标分支没有的文件
    for (i = 0; i < current_commit->blobs->maxCapacity; i++) {
        StringDLLNode* curr = current_commit->blobs->buckets[i]->sentinel->next;
        while (curr != current_commit->blobs->buckets[i]->sentinel) {
            char* filename = curr->str1;
            if (get_str_value(target_commit->blobs, filename) == NULL) {
                unlink(filename); // 从物理磁盘删除
            }
            curr = curr->next;
        }
    }

    // 7. 更新 HEAD 指针
    write_file(".gitlet/HEAD", (unsigned char*)current_branch_ref, strlen(current_branch_ref));

    // 8. 清空暂存区
    unlink(".gitlet/index");

    free_commit(current_commit);
    free_commit(target_commit);
    return 0;
}

int cmd_reset(const char* commit_id){
    // 1. 获取目标提交
    Commit* target_commit = get_commit_by_hash(commit_id);
    if (!target_commit) {
        printf("No commit with that id exists.\n");
        return -1;
    }

    // 2. 获取当前提交 (用于安全检查)
    Commit* current_commit = get_head_commit();
    if (!current_commit){
        printf("无法获取当前head提交\n");
        return -1;
    }
    
    // 3. 安全检查：Untracked files
    // 逻辑：如果一个文件在工作区且未被追踪，但它出现在了目标提交中，
    // 那么执行 reset 会覆盖它。按照规范，此时应报错。
    if (is_file_untracked(current_commit,target_commit)) return -1;
    
    // 4. 覆盖/更新工作目录文件
    unsigned int i;
    for (i = 0; i < target_commit->blobs->maxCapacity; i++) {
        StringDLLNode* curr = target_commit->blobs->buckets[i]->sentinel->next;
        while (curr != target_commit->blobs->buckets[i]->sentinel) {
            char* filename = curr->str1;
            char* blob_hash = curr->str2;

            // 写入文件到工作目录
            char obj_path[256];
            sprintf(obj_path, ".gitlet/objects/%s", blob_hash);
            unsigned char* content = read_file(obj_path);
            long size = get_file_size(obj_path);
            write_file(filename, content, size);
            free(content);

            curr = curr->next;
        }
    }

    // 5. 删除不再需要的文件
    // 如果文件在当前提交中，但不在目标提交中，则物理删除
    for (i = 0; i < current_commit->blobs->maxCapacity; i++) {
        StringDLLNode* curr = current_commit->blobs->buckets[i]->sentinel->next;
        while (curr != current_commit->blobs->buckets[i]->sentinel) {
            if (get_str_value(target_commit->blobs, curr->str1) == NULL) {
                unlink(curr->str1);
            }
            curr = curr->next;
        }
    }

    // 6. 更新当前分支的指针
    unsigned char* head_raw = read_file(".gitlet/HEAD");
    char branch_path[256];
    sprintf(branch_path, ".gitlet/%s", (char*)head_raw + 5);
    write_file(branch_path, (unsigned char*)commit_id, 40);

    // 7. 清空暂存区
    // 简单暴力：直接删除 index 文件，或者保存一个空的 StagingArea
    unlink(".gitlet/index"); 

    // 清理内存
    free_commit(target_commit);
    free_commit(current_commit);
    free(head_raw);

    return 0;
}

