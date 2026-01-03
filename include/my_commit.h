/* 序列化（Serialization）
序列化意味着你要把内存中零散的结构体（带有指针、分布在堆上的 HashMap 节点）
压平成一段连续的字节流，这样才能存入磁盘。
*/
#ifndef __MY_COMMIT_H__
#define __MY_COMMIT_H__

typedef struct StringHashMap StringHashMap;

#include <stddef.h>

typedef struct Commit{
    char hash[41];            // 当前 Commit 的哈希
    char parent_hash[41];     // 父 Commit 的哈希字符串，空则为全0
    char *message;            // 提交信息
    long timestamp;           // 时间戳
    StringHashMap *blobs;     // 文件映射：文件名 -> Blob哈希
}Commit;

unsigned char* serialize_commit(Commit* commit,size_t* out_len);
Commit* deserialize_commit(unsigned char* buffer);
void save_commit_to_disk(Commit* commit);
Commit* get_head_commit(void);
void free_commit(Commit* commit);
Commit* get_commit_by_hash(const char* hash);
StringHashMap* clone_str_hashmap(StringHashMap* map);
int cmd_commit(const char* message);
void print_commit_details(Commit* commit);

#endif
