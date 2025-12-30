#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_str_hashmap.h"
#include "my_str_DLList.h"

// DJB2 哈希算法
unsigned int str_hash_function(const char* str, unsigned int capacity) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return (unsigned int)(hash % capacity);
}

StringDLLNode* find_str_node(StringDLList* bucket,const char* key){
    StringDLLNode* curr = bucket->sentinel->next;
    while (curr != bucket->sentinel){
        if (!strcmp(curr->str1,key)){
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

StringHashMap* create_StringHashMap(unsigned int capacity){
    StringHashMap* map = (StringHashMap*)malloc(sizeof(StringHashMap));
    if (!map) {
        printf("StringHashMap结构空间动态分配失败\n");
        return NULL;
    }
    map->maxCapacity = (capacity > 0) ? capacity : STR_DEFAULT_CAPACITY;
    map->curSize = 0;
    map->buckets = (StringDLList**)calloc(map->maxCapacity, sizeof(StringDLList*));
    if (!map->buckets) {
        free(map);
        printf("StringDLList**数据空间动态分配失败\n");
        return NULL;
    }

    for (unsigned int i = 0; i < map->maxCapacity; i++) {
        map->buckets[i] = create_StringDLList();
    }
    return map;
}

void resize_StringHashMap(StringHashMap* map,unsigned int new_capacity){
    StringDLList** old_buckets;
    StringDLList* bucket;
    unsigned int old_capacity,i;
    StringDLLNode* current_node;
    StringDLLNode* next_node;

    old_buckets = map->buckets;
    old_capacity = map->maxCapacity;
    // 1. 创建新桶数组
    map->buckets = (StringDLList**)calloc(new_capacity,sizeof(StringDLList*));
    map->maxCapacity = new_capacity;
    if (!map->buckets){
        printf("StringDLList*数据空间动态分配失败\n");
        return;
    }
    // 2. 初始化新桶数组
    for (i = 0; i < map->maxCapacity; i++){
        map->buckets[i] = create_StringDLList();
    }
    map->curSize = 0;
    // 3. 重哈希 (Rehashing)：遍历旧的桶，将所有节点重新插入到新桶中
    for (i = 0; i < old_capacity; i++){
        bucket = old_buckets[i];
        current_node = bucket->sentinel->next;
        while (current_node != bucket->sentinel){
            next_node = current_node->next;
            put_str_kv(map,current_node->str1,current_node->str2);
            current_node = next_node;
        }
        free_StringDLList(bucket);
    }
    free(old_buckets);
}

char* get_str_value(StringHashMap* map, const char* key) {
    unsigned int index = str_hash_function(key, map->maxCapacity);
    StringDLLNode* node = find_str_node(map->buckets[index], key);
    return node ? node->str2 : NULL;
}

void put_str_kv(StringHashMap* map, const char* key, const char* value) {
    // 1. 扩容检查
    if ((float)map->curSize / map->maxCapacity > STR_MAX_LOAD_FACTOR) {
        resize_StringHashMap(map,map->maxCapacity * 2);
    }

    unsigned int index = str_hash_function(key, map->maxCapacity);
    StringDLList* bucket = map->buckets[index];
    StringDLLNode* node = find_str_node(bucket, key);

    if (node) {
        // 2. Key 已存在，更新 Value (注意释放旧内存)
        free(node->str2);
        // 使用你写的 my_strdup 复制新值
        node->str2 = my_strdup(value); 
    } else {
        // 3. Key 不存在，插入新节点 (str1 为 key, str2 为 value)
        add_tail_StrDLLNode(bucket, key, value);
        map->curSize++;
    }
}

void remove_str_kv(StringHashMap* map, const char* key) {
    unsigned int index = str_hash_function(key, map->maxCapacity);
    StringDLList* bucket = map->buckets[index];
    StringDLLNode* node = find_str_node(bucket, key);
    
    if (node) {
        remove_StrDLLNode(bucket, node);
        map->curSize--;
    }
    if ((map->curSize > 0) && (map->curSize <= map->maxCapacity / 4)) resize_StringHashMap(map,map->maxCapacity / 2);
}

void free_str_hashmap(StringHashMap* map) {
    unsigned int i;
    if (!map) return;
    for (i = 0; i < map->maxCapacity; i++) {
        free_StringDLList(map->buckets[i]);
    }
    free(map->buckets);
    free(map);
}
