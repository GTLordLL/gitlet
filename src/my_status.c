#include "my_status.h"
#include "my_blob.h"
#include "my_stage.h"
#include "my_str_hashmap.h"
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

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

    // 7. 占位：待实现的部分 (=== Untracked Files ===)
    printf("=== Untracked Files ===\n\n");

    free_staging_area(stage);
    free(head_content);
    return 0;
}

