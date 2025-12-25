/* 哈希表
拉链法解决冲突机制
装载因子 (Load Factor=capacitysize​) 是衡量哈希表性能的最重要指标，
因为它直接代表了在理想均匀分布下，你期望在每个桶（链表）中找到多少个元素。
*/
#ifndef __MY_HASHMAP_H__
#define __MY_HASHMAP_H__

#define DEFALUT_CAPACITY    16          // 默认容量
#define MAX_LOAD_FACTOR     0.75        // 装载因子

typedef struct DLList DLList;
typedef struct DLListNode DLListNode;


typedef struct HashMap{
    DLList** buckets;           // 桶数组，哈希表的主体数组。
                                // 每个数组元素都是一个链表（一个桶/Bucket），用于存放哈希值相同的键值对。
    unsigned int curSize;       // 实际存储的键值对总数
    unsigned int maxCapacity;   // 桶数组的大小
}HashMap;

unsigned int hash_function(int key,unsigned int capacity);
DLListNode* find_kvPair(HashMap* map, int key);
HashMap* create_hashmap(unsigned int initial_capacity);
void resize_map(HashMap* map,unsigned int new_capacity);
int get_value_HashMap(HashMap* map,int key);
void put_kvPair(HashMap* map,int key,int value);
void remove_kvPair(HashMap* map,int key);
void free_hashmap(HashMap* map);

#endif
