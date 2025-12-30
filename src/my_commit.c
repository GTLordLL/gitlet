#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_commit.h"
#include "my_str_hashmap.h"
#include "my_str_DLList.h"
#include "my_sha1.h"
#include "my_blob.h"

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

