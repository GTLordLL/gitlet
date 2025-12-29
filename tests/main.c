#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>


void print_hex(unsigned char *hash, unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

int main() {
    const char *data = "Hello, OpenSSL!"; // 测试字符串
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    // 创建上下文
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    
    // 初始化步骤：指定使用 SHA-1 算法
    if (EVP_DigestInit_ex(mdctx, EVP_sha1(), NULL) != 1) {
        printf("初始化失败\n");
        return 1;
    }

    // 处理数据：可以多次调用 EVP_DigestUpdate 处理大文件
    if (EVP_DigestUpdate(mdctx, data, strlen(data)) != 1) {
        printf("处理数据失败\n");
        return 1;
    }

    // 结束计算并获取结果
    if (EVP_DigestFinal_ex(mdctx, hash, &hash_len) != 1) {
        printf("计算哈希失败\n");
        return 1;
    }

    // 释放上下文
    EVP_MD_CTX_free(mdctx);

    printf("原始数据: %s\n", data);
    printf("SHA-1 哈希值: ");
    print_hex(hash, hash_len);

    return EXIT_SUCCESS;
}
