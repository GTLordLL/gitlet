#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "my_stage.h"
#include "my_str_hashmap.h"
#include "my_str_DLList.h"
#include "my_commit.h"
#include "my_blob.h"
#include "my_sha1.h"


// 将StringHashMap序列化为字节流buffer
unsigned char* serialize_map(StringHashMap* map,unsigned char* map_buffer){
    unsigned int count = map->curSize;
    memcpy(map_buffer,&count,4);
    map_buffer += 4;

    unsigned int i;
    for (i = 0; i < map->maxCapacity; i++){
        StringDLLNode* curr = map->buckets[i]->sentinel->next;
        while (curr != map->buckets[i]->sentinel){
            unsigned int name_len = (unsigned int)strlen(curr->str1);
            memcpy(map_buffer,&name_len,4);
            map_buffer += 4;
            memcpy(map_buffer,curr->str1,name_len);
            map_buffer += name_len;
            memcpy(map_buffer,curr->str2,40);
            map_buffer += 40;
            curr = curr->next;
        }
    }
    return map_buffer;
}

// 序列化整个 StagingArea
void save_staging_area(StagingArea* stage){
    size_t estimated_size = 8 + (stage->staged_files->curSize + stage->removed_files->curSize) * 200;
    unsigned char* buffer = (unsigned char*)malloc(estimated_size);
    unsigned char* p = buffer;

    p = serialize_map(stage->staged_files,p);
    p = serialize_map(stage->removed_files,p);

    write_file(".gitlet/index", buffer, p - buffer);
    free(buffer);
}

// 从 buffer 恢复一个 StringHashMap
unsigned char* deserialize_map(StringHashMap* map,unsigned char* map_buffer){
    unsigned int count;
    memcpy(&count,map_buffer,4);
    map_buffer += 4;

    unsigned int i;
    for (i = 0; i < count; i++){
        unsigned int name_len;
        memcpy(&name_len,map_buffer,4);
        map_buffer += 4;

        char* filename = (char*)malloc(name_len + 1);
        memcpy(filename,map_buffer,name_len);
        filename[name_len] = '\0';
        map_buffer += name_len;

        char hash[41];
        memcpy(hash,map_buffer,40);
        hash[40] = '\0';
        map_buffer += 40;

        put_str_kv(map,filename,hash);
        free(filename);
    }
    return map_buffer;
}

StagingArea* create_StagingArea(unsigned int capacity){
    StagingArea* stage = (StagingArea*)malloc(sizeof(StagingArea));
    if (!stage) return NULL;

    stage->staged_files = create_StringHashMap(capacity);
    if (!stage->staged_files){
        free(stage);
        return NULL;
    }
    stage->removed_files = create_StringHashMap(capacity);
    if (!stage->removed_files){
        free_str_hashmap(stage->staged_files);
        free(stage);
        return NULL;
    }
    return stage;
}

// 读取并初始化 StagingArea
StagingArea* read_staging_area(void){
    StagingArea* stage = create_StagingArea(16);
    if (!stage) return NULL;

    unsigned char* buffer = read_file(".gitlet/index");
    if (!buffer) return stage; // 如果 index 不存在，返回空的暂存区

    unsigned char* p = buffer;
    p = deserialize_map(stage->staged_files,p);
    p = deserialize_map(stage->removed_files,p);

    free(buffer);
    return stage;
}

void free_staging_area(StagingArea* stage){
    if (stage){
        free_str_hashmap(stage->staged_files);
        free_str_hashmap(stage->removed_files);
        free(stage);
    }
}

int cmd_add(const char *filename){
    // 1. 检查文件是否存在
    if (access(filename,F_OK)){
        printf("File does not exist.\n");
        return -1;
    }
    
    // 2. 加载当前状态：暂存区和 HEAD 提交
    StagingArea* stage = read_staging_area();
    Commit* head = get_head_commit();

    // 3. 计算工作区当前文件的哈希
    unsigned char* file_content = read_file(filename);
    long file_size = get_file_size(filename);
    char* current_hash = calculate_data_sha1(file_content,file_size);

    // 4. 获取 HEAD 提交中该文件的版本
    char* head_version_hash = NULL;
    if (head) head_version_hash = get_str_value(head->blobs,filename);

    // 5. 核心逻辑判断
    // 情况 A：当前文件版本与 HEAD 一致 -> 不暂存
    if (head_version_hash && strcmp(current_hash,head_version_hash) == 0){
        // 如果它之前在暂存区里，需要将其移除（取消暂存）
        remove_str_kv(stage->staged_files,filename);
    }else{ // 情况 B：文件是新的或已修改 -> 暂存
        save_blob(filename); // 持久化到 .gitlet/objects
        put_str_kv(stage->staged_files,filename,current_hash);
    }

    // 6. 如果文件在“待删除”名单里，移除标记
    remove_str_kv(stage->removed_files,filename);

    // 7. 持久化暂存区状态到 .gitlet/index
    save_staging_area(stage);
    
    // 8. 释放内存
    free(file_content);
    free(current_hash);
    free_staging_area(stage);
    free_commit(head);

    return 0;
}

int cmd_rm(const char *filename){
    // 1. 检查文件是否存在
    if (access(filename,F_OK)){
        printf("File does not exist.\n");
        return -1;
    }

    // 2. 加载当前状态：暂存区和 HEAD 提交
    StagingArea* stage = read_staging_area();
    Commit* head = get_head_commit();

    char* staged_hash = get_str_value(stage->staged_files,filename);

    // 3. 检查 HEAD 提交中是否追踪了该文件
    char* head_version_hash = NULL;
    if (head) head_version_hash = get_str_value(head->blobs,filename);

    // 4. 错误检查：既没暂存也没追踪
    if (!staged_hash && !head_version_hash){
        printf("No reason to remove the file.\n");
        free_staging_area(stage);
        free_commit(head);
        return -1;
    }
    
    // 5. 如果文件在暂存区，取消暂存
    if (staged_hash) remove_str_kv(stage->staged_files,filename);

    // 6. 如果文件在 HEAD 提交中，标记为移除并从工作目录物理删除
    if (head_version_hash){
        put_str_kv(stage->removed_files,filename,head_version_hash);

        if (remove(filename) != 0) perror("Failed to delete the file in working directory");
    }
    
    // 7. 保存暂存区状态
    save_staging_area(stage);

    // 8. 释放资源
    free_staging_area(stage);
    free_commit(head);
    return 0;
}


