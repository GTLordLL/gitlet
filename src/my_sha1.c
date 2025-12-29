#include <openssl/evp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "my_sha1.h"

// 将二进制哈希转换为 40 位 16 进制字符串
char* sha1_to_hex(unsigned char* hash,unsigned int length){
    // 一个 unsigned char（二进制）占 1 字节，但转成十六进制字符串（如 0xFF）需要 2 个字符。字符串必须以 \0 结尾，所以需要额外的 +1。
    char* hex_hash = (char*)malloc(length * 2 + 1);
    unsigned int i;
    for (i = 0; i < length; i++){
        sprintf(hex_hash + (i * 2),"%02x",hash[i]);
    }
    hex_hash[length * 2] = '\0';
    return hex_hash;
}

// 通用哈希计算函数
char* calculate_data_sha1(const void* data,size_t length){
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    // 创建上下文
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    // 初始化步骤：指定使用 SHA-1 算法
    if (EVP_DigestInit_ex(mdctx, EVP_sha1(), NULL) != 1) {
        EVP_MD_CTX_free(mdctx);
        printf("初始化失败\n");
        return NULL;
    }
    // 处理数据：可以多次调用 EVP_DigestUpdate 处理大文件
    if (EVP_DigestUpdate(mdctx, data, length) != 1) {
        EVP_MD_CTX_free(mdctx);
        printf("处理数据失败\n");
        return NULL;
    }
    // 结束计算并获取结果
    if (EVP_DigestFinal_ex(mdctx, hash, &hash_len) != 1) {
        EVP_MD_CTX_free(mdctx);
        printf("计算哈希失败\n");
        return NULL;
    }
    // 释放上下文
    EVP_MD_CTX_free(mdctx);

    return sha1_to_hex(hash,hash_len); // 记得在使用后 free 这个字符串
}
