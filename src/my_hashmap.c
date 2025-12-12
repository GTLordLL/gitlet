#include "my_hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include "my_DLList.h"

// 核心哈希函数：将 Key 映射到数组下标,返回数组下标
// 使用简单的取模法。
unsigned int hash_function(int key,unsigned int capacity){
    return (unsigned int)abs(key) % capacity;
}

// 查找指定 Key 的节点，返回 Node*，找不到返回 NULL
Node* find_node(HashMap* map, int key){
    unsigned int index;
    DLList* bucket;
    index = hash_function(key,map->maxCapacity);
    bucket = map->buckets[index];
    // 遍历桶中的链表
    // 遍历从哨兵的下一个节点 (head) 开始
    Node* current = bucket->sentinel->next;
    
    // 只要 current 不是哨兵（即未回到起点），就继续查找
    while (current != bucket->sentinel) {
        if (current->data1 == key) { // 检查 Key (存储在 data1)
            return current;
        }
        current = current->next;
    }
    return NULL;
}

HashMap* create_hashmap(unsigned int initial_capacity){
    unsigned int i;
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    if (!map) {
        printf("HashMap结构空间动态分配失败\n");
        return NULL;
    }

    map->maxCapacity = (initial_capacity > 0) ? initial_capacity : DEFALUT_CAPACITY;
    map->curSize = 0;

    // calloc 自动初始化为 NULL
    map->buckets = (DLList**)calloc(map->maxCapacity,sizeof(DLList*));
    if (!map->buckets) {
        free(map);
        printf("DLList*数据空间动态分配失败\n");
        return NULL;
    }

    // 初始化每个桶（创建哨兵链表）
    for (i = 0; i < map->maxCapacity; i++){
        map->buckets[i] = create_dllist();
    }
    return map;
}

void resize_map(HashMap* map,unsigned int new_capacity){
    DLList** old_buckets;
    DLList* bucket;
    unsigned int old_capacity,i;
    Node* current_node;
    Node* next_node;

    old_buckets = map->buckets;
    old_capacity = map->maxCapacity;
    // 1. 创建新桶数组
    map->buckets = (DLList**)calloc(new_capacity,sizeof(DLList*));
    map->maxCapacity = new_capacity;
    if (!map->buckets){
        printf("DLList*数据空间动态分配失败\n");
        exit(EXIT_FAILURE);
    }
    // 2. 初始化新桶数组
    for (i = 0; i < map->maxCapacity; i++){
        map->buckets[i] = create_dllist();
    }
    map->curSize = 0;
    // 3. 重哈希 (Rehashing)：遍历旧的桶，将所有节点重新插入到新桶中
    for (i = 0; i < old_capacity; i++){
        bucket = old_buckets[i];
        current_node = bucket->sentinel->next;
        while (current_node != bucket->sentinel){
            next_node = current_node->next;
            put_kvPair(map,current_node->data1,current_node->data2);
            current_node = next_node;
        }
        free_dllist(bucket);
    }
    free(old_buckets);
}

int get_value_HashMap(HashMap* map,int key){
    Node* node = find_node(map,key);
    if (node){
        return node->data2;
    }else{
        printf("未找到键值对\n");
        return -1;
    }
}

void put_kvPair(HashMap* map,int key,int value){
    Node* node;
    unsigned int index;
    DLList* bucket;

    // 1. 检查是否需要扩容
    if ((float)map->curSize / map->maxCapacity > MAX_LOAD_FACTOR) {
        resize_map(map, map->maxCapacity * 2);
    }

    node = find_node(map,key);
    // 3. Key 存在：更新 Value
    if (node){
        node->data2 = value;
    }
    // 4. Key 不存在：插入新
    else{
        index = hash_function(key,map->maxCapacity);
        bucket = map->buckets[index];
        add_tail(bucket,key,value);
        map->curSize++;
    }
}

void free_hashmap(HashMap* map){
    unsigned int i;
    if (map){
        for (i = 0; i < map->maxCapacity; i++){
            free_dllist(map->buckets[i]);
        }
        free(map->buckets);
        free(map);
    }else{
        printf("HashMap未分配内存空间，无法释放\n");
    }
}
