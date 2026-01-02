#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "my_commit.h"
#include "my_str_hashmap.h"
#include "my_str_DLList.h"
#include "my_sha1.h"
#include "my_blob.h"
#include "my_stage.h"

// 辅助函数：将 Commit 序列化为字节流
unsigned char* serialize_commit(Commit* commit,size_t* out_len){
    // 1. 初步预估大小 (不含 map 内部文件名长度)
    size_t msg_len = strlen(commit->message);
    /* 设计二进制序列化协议
    我们要把数据按以下顺序排列在内存缓冲区中：
    Parent Hash (40字节)
    Timestamp (8字节, long)
    Message Length (4字节, int)
    Message Content (N字节)
    Blobs Count (4字节, int, 记录有多少个文件映射)
    文件映射列表：循环写入 [文件名长度(4字节) + 文件名 + 文件哈希(40字节)]
    */
    size_t estimated_size = 40 + 8 + 4 + msg_len + 4 + (commit->blobs->curSize * 100);
    unsigned char* buffer = (unsigned char*)malloc(estimated_size);
    unsigned char* p = buffer;

    // 2. 写入 Parent Hash (40 bytes)
    memcpy(p,commit->parent_hash,40);
    p += 40;

    // 3. 写入 Timestamp (8 bytes)
    memcpy(p,&(commit->timestamp),8);
    p += 8;

    // 4. 写入 Message
    unsigned int m_len = (unsigned int)msg_len;
    memcpy(p,&m_len,4);
    p += 4;
    memcpy(p,commit->message,m_len);
    p += m_len;

    // 5. 写入 Blobs 数量
    unsigned int map_size = (unsigned int)commit->blobs->curSize;
    memcpy(p, &map_size, 4);
    p += 4;
    unsigned int i;
    // 6. 遍历 HashMap 写入文件映射
    for (i = 0; i < commit->blobs->maxCapacity; i++){
        StringDLLNode* curr = commit->blobs->buckets[i]->sentinel->next;
        while (curr != commit->blobs->buckets[i]->sentinel){
            // 写入文件名长度
            unsigned int name_len = strlen(curr->str1);
            memcpy(p,&name_len,4);
            p += 4;
            // 写入文件名
            memcpy(p,curr->str1,name_len);
            p += name_len;
            // 写入文件哈希 (40字节)
            memcpy(p,curr->str2,40);
            p += 40;

            curr = curr->next;
        }
    }
    *out_len = p - buffer;
    return buffer;
}

// 反序列化：当你运行 ./gitlet log 读回旧提交时，需要此函数将字节流还原为内存结构。
Commit* deserialize_commit(unsigned char* buffer){
    Commit* old_commit = (Commit*)malloc(sizeof(Commit));
    unsigned char* p = buffer;

    // 1. 读取 Parent Hash
    memcpy(old_commit->parent_hash,p,40);
    old_commit->parent_hash[40] = '\0';
    p += 40;

    // 2. 读取 Timestamp
    memcpy(&(old_commit->timestamp),p,8);
    p += 8;

    // 3. 读取 Message
    unsigned int m_len;
    memcpy(&m_len,p,4);
    p += 4;
    old_commit->message = (char*)malloc(m_len + 1);
    memcpy(old_commit->message,p,m_len);
    old_commit->message[m_len] = '\0';
    p += m_len;

    // 4. 读取 Blobs 数量并初始化 HashMap
    unsigned int map_size;
    memcpy(&map_size,p,4);
    p += 4;
    old_commit->blobs = create_StringHashMap(map_size > 0 ? map_size : 16);

    // 5. 循环恢复文件映射
    unsigned int i;
    for ( i = 0; i < map_size; i++){
        unsigned int name_len;
        memcpy(&name_len,p,4);
        p += 4;

        char* filename = (char*)malloc(name_len + 1);
        memcpy(filename,p,name_len);
        filename[name_len] = '\0';
        p += name_len;

        char blob_hash[41];
        memcpy(blob_hash,p,40);
        blob_hash[40] = '\0';
        p += 40;

        put_str_kv(old_commit->blobs,filename,blob_hash);
        free(filename);
    }
    return old_commit;
}

// 持久化
void save_commit_to_disk(Commit* commit){
    size_t len;
    // 序列化
    unsigned char* buffer = serialize_commit(commit,&len);

    // 计算整个 Commit 序列化数据的哈希
    char* commit_hash = calculate_data_sha1(buffer,len);
    strncpy(commit->hash,commit_hash,40);
    commit->hash[40] = '\0';

    // 拼接存储路径 .gitlet/objects/HASH
    char path[256];
    sprintf(path,".gitlet/objects/%s", commit_hash);

    // 写入文件
    write_file(path,buffer,len);

    free(buffer);
    free(commit_hash);
}

void free_commit(Commit* commit){
    if (commit){
        free(commit->message);
        free_str_hashmap(commit->blobs);
        free(commit);
    }
}

StringHashMap* clone_str_hashmap(StringHashMap* map){
    StringHashMap* new_map = create_StringHashMap(map->maxCapacity);
    unsigned int i;
    for (i = 0; i < map->maxCapacity; i++){
        StringDLLNode* curr = map->buckets[i]->sentinel->next;
        while (curr != map->buckets[i]->sentinel){
            put_str_kv(new_map,curr->str1,curr->str2);
            curr = curr->next;
        }
    }
    return new_map;
}

int cmd_commit(const char* message){
    // 1. 校验参数：提交信息（Message）不能为空
    if (!message || strlen(message) == 0){
        printf("Please enter a commit message.\n");
        return -1;
    }

    // 2. 检查变化：如果暂存区中没有任何待增加（staged）或待删除（removed）的文件，报错 No changes added to the commit.。
    StagingArea* stage = read_staging_area();
    if (stage->staged_files->curSize == 0 && stage->removed_files->curSize == 0){
        printf("No changes added to the commit.\n");
        free_staging_area(stage);
        return -1;
    }

    // 3. 克隆父级快照：新的提交应该继承父提交（HEAD）所有的文件映射（Blob 映射）。
    Commit* head_commit = get_head_commit();
    Commit* new_commit = (Commit*)malloc(sizeof(Commit));

    strcpy(new_commit->parent_hash,head_commit->parent_hash);
    new_commit->message = my_strdup(message);
    new_commit->timestamp = (long)time(NULL); // 当前时间
    // 合并逻辑：先复制父级所有的文件，再根据暂存区增删
    new_commit->blobs = clone_str_hashmap(head_commit->blobs);

    // 4. 更新映射：
    // 遍历暂存区的 staged_files，在新的映射中添加或更新这些文件。
    unsigned int i;
    for (i = 0; i < stage->staged_files->maxCapacity; i++){
        StringDLLNode* curr = stage->staged_files->buckets[i]->sentinel->next;
        while (curr != stage->staged_files->buckets[i]->sentinel){
            put_str_kv(new_commit->blobs,curr->str1,curr->str2);
            curr = curr->next;
        }
    }
    // 遍历暂存区的 removed_files，在新的映射中移除这些文件。
    for (i = 0; i < stage->removed_files->maxCapacity; i++){
        StringDLLNode* curr = stage->removed_files->buckets[i]->sentinel->next;
        while (curr != stage->removed_files->buckets[i]->sentinel){
            remove_str_kv(new_commit->blobs,curr->str1);
            curr = curr->next;
        }
    }

    // 5. 创建并持久化 Commit 对象：
    save_commit_to_disk(new_commit);

    // 6. 更新分支指针 (master)
    // 需要读取 HEAD 确定当前分支文件路径，再写入 new_commit->hash
    unsigned char* head_file = read_file(".gitlet/HEAD");
    char branch_path[256];
    sprintf(branch_path,".gitlet/%s", (char*)head_file + 5);
    write_file(branch_path,(unsigned char*)new_commit->hash,40);

    // 6. 清空暂存区
    free_staging_area(stage);
    StagingArea* empty_stage = read_staging_area();
    save_staging_area(empty_stage);
    free_staging_area(empty_stage);

    // 7. 内存清理
    free_commit(head_commit);
    free_commit(new_commit);
    free(head_file);

    return 0;
}

// 根据哈希值直接从磁盘加载读取并还原 Commit 对象
Commit* get_commit_by_hash(const char* hash){
    char path[256];
    sprintf(path,".gitlet/objects/%s", hash);

    unsigned char* buffer = read_file(path);
    if (!buffer) return NULL;

    Commit* commit = deserialize_commit(buffer);
    // 注意：deserialize_commit 并没有填充 hash 字段，因为哈希是文件名
    // 我们手动填入以便 log 打印
    strncpy(commit->hash,hash,40);
    commit->hash[40] = '\0';

    free(buffer);
    return commit;
}

// 将 timestamp 转换为 Git 风格的字符串
void print_commit_details(Commit* commit){
    struct tm* tm_info = localtime(&(commit->timestamp));
    char time_buf[64];
    // 格式：Thu Nov 9 17:01:33 2017 -0800
    strftime(time_buf, sizeof(time_buf), "%a %b %d %H:%M:%S %Y %z", tm_info);

    printf("===\n");
    printf("commit %s\n", commit->hash);
    printf("Date: %s\n", time_buf);
    printf("%s\n\n", commit->message);
}

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

