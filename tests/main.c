#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "my_str_hashmap.h"

void test_basic_operations() {
    printf("--- 开始基本操作测试 ---\n");
    StringHashMap* map = create_StringHashMap(4); // 从较小的容量开始以触发扩容

    // 1. 测试插入 (Put)
    put_str_kv(map, "name", "Gemini");
    put_str_kv(map, "version", "3.0");
    put_str_kv(map, "language", "C");
    
    // 2. 测试查询 (Get)
    char* val1 = get_str_value(map, "name");
    printf("查找 name: %s\n", val1 ? val1 : "NULL");
    assert(val1 != NULL && strcmp(val1, "Gemini") == 0);

    // 3. 测试更新 (Update)
    put_str_kv(map, "version", "3.5-Flash");
    char* val2 = get_str_value(map, "version");
    printf("更新 version 后: %s\n", val2);
    assert(strcmp(val2, "3.5-Flash") == 0);

    // 4. 测试删除 (Remove)
    printf("删除 'language'...\n");
    remove_str_kv(map, "language");
    char* val3 = get_str_value(map, "language");
    assert(val3 == NULL);
    printf("删除成功，当前大小: %u\n", map->curSize);

    free_str_hashmap(map);
    printf("基本操作测试通过！\n\n");
}

void test_expansion_and_collision() {
    printf("--- 开始扩容与冲突测试 ---\n");
    // 初始容量为 8，装载因子 0.75，意味着超过 6 个元素就会扩容
    StringHashMap* map = create_StringHashMap(8);

    // 插入 20 个元素，强制触发多次 resize
    char key[20];
    char value[20];
    for (int i = 0; i < 20; i++) {
        sprintf(key, "key_%d", i);
        sprintf(value, "val_%d", i);
        put_str_kv(map, key, value);
    }

    printf("插入 20 个元素后，当前容量: %u, 当前大小: %u\n", map->maxCapacity, map->curSize);
    assert(map->maxCapacity > 8);
    assert(map->curSize == 20);

    // 随机验证几个数据
    assert(strcmp(get_str_value(map, "key_0"), "val_0") == 0);
    assert(strcmp(get_str_value(map, "key_19"), "val_19") == 0);
    assert(get_str_value(map, "key_25") == NULL);

    // 测试缩容：删除大部分元素
    for (int i = 5; i < 20; i++) {
        sprintf(key, "key_%d", i);
        remove_str_kv(map, key);
    }
    printf("大量删除后，当前容量: %u, 当前大小: %u\n", map->maxCapacity, map->curSize);
    
    free_str_hashmap(map);
    printf("扩容与缩容测试通过！\n");
}

int main() {
    test_basic_operations();
    test_expansion_and_collision();
    printf("\n所有测试均已成功完成！\n");

    return EXIT_SUCCESS;
}
