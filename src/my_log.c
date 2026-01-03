#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "my_commit.h"
#include "my_log.h"
#include "my_blob.h"

// 只显示从当前分支 HEAD 开始的回溯路径
int cmd_log(void){
    // 1. 获取 HEAD 的哈希（从 master 或其他分支文件读取）
    unsigned char* head_raw = read_file(".gitlet/HEAD");
    if (!head_raw) return -1;

    char branch_path[256];
    sprintf(branch_path,".gitlet/%s", (char*)head_raw + 5);

    unsigned char* current_hash_raw = read_file(branch_path);
    if (!current_hash_raw) {
        free(head_raw);
        return -1;
    }

    char current_hash[41];
    memcpy(current_hash, current_hash_raw, 40);
    current_hash[40] = '\0';

    // 2. 沿着父链回溯
    while (1){
        Commit* commit = get_commit_by_hash(current_hash);
        if (!commit) break;
        
        print_commit_details(commit);

        // 检查是否有父提交
        int has_parent = 0;
        int i;
        for (i = 0; i < 40; i++){
            // 初始提交的 parent_hash 全为 '0'
            if (commit->parent_hash[i] != '0'){
                has_parent = 1;
                break;
            }
        }

        if (!has_parent){
            free_commit(commit);
            break;
        }
        
        // 更新哈希为父哈希，继续循环
        strcpy(current_hash,commit->parent_hash);
        free_commit(commit);
    }
    free(head_raw);
    free(current_hash_raw);
    return 0;
}

// 显示所有曾经创建过的提交（Commit），无论它们是否在当前分支上。
int cmd_global_log(void){
    // 1. 打开 objects 目录
    DIR *dir = opendir(".gitlet/objects");
    if (dir == NULL) {
        return -1;
    }

    struct dirent *entry;
    // 2. 遍历目录下的所有文件
    while ((entry = readdir(dir)) != NULL) {
        // 跳过 "." 和 ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // 3. 读取该文件的内容
        char path[512];
        sprintf(path, ".gitlet/objects/%s", entry->d_name);
        
        unsigned char* data = read_file(path);
        if (!data) continue;

        // 4. 检查文件类型：Commit 对象以 "commit " 开头（7字节）
        long file_size = get_file_size(path);
        if (file_size < 7) {
            free(data);
            continue; // 文件太小，不可能是 Commit
        }
        
        // 检查前 7 个字节是否为 "commit "
        if (memcmp(data, "commit ", 7) != 0) {
            free(data);
            continue; // 不是 Commit 对象，跳过（可能是 Blob）
        }
        
        Commit* commit = deserialize_commit(data);
        
        // 关键点：由于反序列化并不知道哈希值，需要手动把文件名（哈希）填进去
        strncpy(commit->hash, entry->d_name, 40);
        commit->hash[40] = '\0';

        // 5. 打印提交信息
        print_commit_details(commit);

        free(data);
        free_commit(commit); // 记得释放内存
    }

    closedir(dir);
    return 0;
}

// 接收一个 message 字符串，找到所有具有该消息的 commit 哈希。
int cmd_find(const char* target_message){
    // 1. 打开 objects 目录
    DIR *dir = opendir(".gitlet/objects");
    if (dir == NULL) {
        return -1;
    }

    struct dirent *entry;
    int found = 0;

    // 2. 遍历 objects 目录下的所有文件
    while ((entry = readdir(dir)) != NULL) {
        // 跳过 "." 和 ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // 3. 读取文件内容
        char path[512];
        sprintf(path, ".gitlet/objects/%s", entry->d_name);
        unsigned char* data = read_file(path);
        if (!data) continue;

        // 4. 检查文件类型：Commit 对象以 "commit " 开头（7字节）
        long file_size = get_file_size(path);
        if (file_size < 7) {
            free(data);
            continue; // 文件太小，不可能是 Commit
        }
        
        // 检查前 7 个字节是否为 "commit "
        if (memcmp(data, "commit ", 7) != 0) {
            free(data);
            continue; // 不是 Commit 对象，跳过（可能是 Blob）
        }

        Commit* commit = deserialize_commit(data);
        
        // 5. 比对 Message
        // 使用 strstr 进行模糊匹配（子串匹配）
        if (commit && commit->message && strstr(commit->message, target_message) != NULL) {
            printf("%s\n", entry->d_name); // 打印该 Commit 的哈希（文件名）
            found = 1;
        }

        free(data);
        free_commit(commit);
    }

    // 6. 错误处理：如果没有找到任何匹配项
    if (!found) {
        printf("Found no commit with that message.\n");
    }

    closedir(dir);
    return 0;
}

