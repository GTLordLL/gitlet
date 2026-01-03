#include "my_status.h"
#include "my_blob.h"
#include "my_stage.h"
#include "my_str_hashmap.h"
#include "my_str_DLList.h"
#include "my_sha1.h"
#include "my_commit.h"
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int cmd_status(void){
    // 1. 获取当前分支名
    unsigned char* head_content = read_file(".gitlet/HEAD");
    if (!head_content) return -1;
    // HEAD 内容格式为 "ref: refs/heads/master"
    char* current_branch = (char*)head_content + 16;

    // 2. 打印分支 (=== Branches ===)
    printf("=== Branches ===\n");
    DIR* directory = opendir(".gitlet/refs/heads");
    struct dirent* dir;
    if (directory){
        // 这里为了简单先直接读取，若追求完美也需要排序
        dir = readdir(directory);
        while (dir != NULL){
            if (dir->d_name[0] != '.'){
                if (strcmp(dir->d_name,current_branch) == 0){
                    printf("*%s\n", dir->d_name);
                }else {
                    printf("%s\n", dir->d_name);
                } 
            }
            dir = readdir(directory);
        }
        closedir(directory);
    }
    printf("\n");

    // 3. 读取暂存区
    StagingArea* stage = read_staging_area();

    // 4. 打印暂存文件 (=== Staged Files ===)
    printf("=== Staged Files ===\n");
    print_sorted_keys(stage->staged_files);
    printf("\n");

    // 5. 打印移除文件 (=== Removed Files ===)
    printf("=== Removed Files ===\n");
    print_sorted_keys(stage->removed_files);
    printf("\n");

    // 6. 占位：待实现的部分 (=== Modifications Not Staged For Commit ===)
    printf("=== Modifications Not Staged For Commit ===\n\n");
    // 逻辑 A: 检查已追踪但被修改或删除的文件
    // 1. 检查 Staged 里的文件是否被修改或删除
    unsigned int i;
    for (i = 0; i < stage->staged_files->maxCapacity; i++) {
        StringDLLNode* curr = stage->staged_files->buckets[i]->sentinel->next;
        while (curr != stage->staged_files->buckets[i]->sentinel) {
            char* filename = curr->str1;
            char* staged_hash = curr->str2;
            
            if (access(filename, F_OK) != 0) {
                printf("%s (deleted)\n", filename);
            } else {
                char* current_hash = get_file_sha1(filename);
                if (strcmp(staged_hash, current_hash) != 0) {
                    printf("%s (modified)\n", filename);
                }
                free(current_hash);
            }
            curr = curr->next;
        }
    }
    // 2. 检查 HEAD 里的文件（且不在 Stage 中）是否被删除或修改
    Commit* head = get_head_commit();
    for (i = 0; i < head->blobs->maxCapacity; i++) {
        StringDLLNode* curr = head->blobs->buckets[i]->sentinel->next;
        while (curr != head->blobs->buckets[i]->sentinel) {
            char* filename = curr->str1;
            // 如果已经在 staged 或 removed 中处理过了，跳过
            if (get_str_value(stage->staged_files, filename) == NULL &&
                get_str_value(stage->removed_files, filename) == NULL) {
                
                if (access(filename, F_OK) != 0) {
                    printf("%s (deleted)\n", filename);
                } else {
                    char* current_hash = get_file_sha1(filename);
                    if (strcmp(curr->str2, current_hash) != 0) {
                        printf("%s (modified)\n", filename);
                    }
                    free(current_hash);
                }
            }
            curr = curr->next;
        }
    }
    printf("\n");

    // 7. 占位：待实现的部分 (=== Untracked Files ===)
    printf("=== Untracked Files ===\n\n");
    // 逻辑 B: 检查磁盘上有但不在 Gitlet 管理下的文件
    directory = opendir(".");
    while ((dir = readdir(directory)) != NULL) {
        if (dir->d_name[0] == '.') continue; // 跳过 .gitlet, .DS_Store 等
        
        struct stat st;
        stat(dir->d_name, &st);
        if (S_ISDIR(st.st_mode)) continue; // 跳过文件夹

        char* filename = dir->d_name;
        // 判定条件：不在当前提交，不在暂存添加区，且不在暂存删除区
        if (get_str_value(head->blobs, filename) == NULL &&
            get_str_value(stage->staged_files, filename) == NULL &&
            get_str_value(stage->removed_files, filename) == NULL) {
            printf("%s\n", filename);
        }
    }
    closedir(directory);
    printf("\n");

    free_commit(head);
    free_staging_area(stage);
    free(head_content);
    return 0;
}

