#ifndef __MY_STR_HASHMAP_H__
#define __MY_STR_HASHMAP_H__

#define STR_DEFAULT_CAPACITY 16
#define STR_MAX_LOAD_FACTOR  0.75

typedef struct StringDLList StringDLList;

typedef struct StringHashMap{
    StringDLList** buckets;    // 桶数组，每个桶是一个字符串双向链表
    unsigned int curSize;      // 当前键值对数量
    unsigned int maxCapacity;  // 桶的总数
}StringHashMap;

unsigned int str_hash_function(const char* str, unsigned int capacity);
StringHashMap* create_StringHashMap(unsigned int capacity);
char* get_str_value(StringHashMap* map, const char* key);
void put_str_kv(StringHashMap* map, const char* key, const char* value);
void remove_str_kv(StringHashMap* map, const char* key);
void free_str_hashmap(StringHashMap* map);

#endif
