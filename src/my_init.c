#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_init.h"
#include "my_blob.h"
#include "my_commit.h"
#include "my_str_hashmap.h"
#include "my_str_DLList.h"

void make_dir(const char* path){
    if (mkdir(path,0777) == -1){
        if (errno != EEXIST){
            perror("创建目录失败");
            exit(EXIT_FAILURE);
        }
    }
}

int cmd_init(void){
    // 1. 检查是否已经初始化过
    struct stat st; // 使用stat()获取.gitlet文件的属性，并填充到st中
    if (stat(".gitlet",&st) == 0){
        printf("A Gitlet version-control system already exists in the current directory.\n");
        return -1;
    }

    /* 2. 创建目录结构
        .gitlet/
    ├── objects/        # 所有的 Blob 和 Commit 对象（以哈希值命名）
    ├── refs/
    │   └── heads/      # 存储分支文件（文件内容是该分支最新的 Commit 哈希）
    │       └── master  
    ├── HEAD            # 指向当前分支的引用（例如 "ref: refs/heads/master"）
    └── index           # 你的 StagingArea 的序列化文件
    */
    make_dir(".gitlet");
    make_dir(".gitlet/objects");
    make_dir(".gitlet/refs");
    make_dir(".gitlet/refs/heads");
    
    // 3. 创建初始提交 (Initial Commit)
    Commit* initial_commit = (Commit*)malloc(sizeof(Commit));
    // 初始提交没有父级，哈希设为全 '0'
    memset(initial_commit->parent_hash,'0',40);
    initial_commit->parent_hash[40] = '\0';

    initial_commit->message = my_strdup("initial commit");
    initial_commit->timestamp = 0; // 规范要求：Epoch 时间
    initial_commit->blobs = create_StringHashMap(16); // 空映射

    // 4. 持久化初始提交到 .gitlet/objects/
    save_commit_to_disk(initial_commit);

    // 5. 设置 master 分支指向初始提交
    char master_path[256];
    sprintf(master_path,".gitlet/refs/heads/master");
    write_file(master_path,(unsigned char*)initial_commit->hash,40);

    // 6. 设置 HEAD 指向 master 分支引用
    // 格式通常为 "ref: refs/heads/master"
    const char *head_content = "ref: refs/heads/master";
    write_file(".gitlet/HEAD",(unsigned char*)head_content,strlen(head_content));

    printf("Gitlet initialized with commit: %s\n", initial_commit->hash);

    // 释放内存
    free(initial_commit->message);
    free_str_hashmap(initial_commit->blobs);
    free(initial_commit);

    return 0;
}

