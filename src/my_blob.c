#include <stdio.h>
#include <stdlib.h>
#include "my_blob.h"
#include "my_sha1.h"

// 一次性将整个文件读入内存的标准做法
unsigned char* read_file(const char* file_path){
    FILE* file = fopen(file_path,"rb"); // rb以二进制只读模式打开文件
    if (!file) return NULL;
    fseek(file,0,SEEK_END); // 将光标/读写指针移动到文件最末尾
    long file_size = ftell(file); // 获取当前光标位置，即文件大小(字节数)
    fseek(file,0,SEEK_SET); // 将光标/读写指针设置到文件开头，因为等下要从头开始读取

    unsigned char* buffer = (unsigned char*)malloc(file_size + 1); // 准备内存来接收数据
    if (buffer) {
        fread(buffer,1,file_size,file); // 开始读取磁盘文件并写入buffer内存
        buffer[file_size] = '\0'; // 关键：补上字符串结束符
    }
    
    fclose(file); // 关闭文件，释放系统资源
    return buffer;
}

long get_file_size(const char* file_path){
    FILE* file = fopen(file_path,"rb"); // rb以二进制只读模式打开文件
    if (!file) {
        printf("文件路径错误,或不存在,亦或无法打开\n");
        return -1;
    }
    fseek(file,0,SEEK_END); // 将光标/读写指针移动到文件最末尾
    long file_size = ftell(file); // 获取当前光标位置，即文件大小(字节数)
    fseek(file,0,SEEK_SET); // 将光标/读写指针设置到文件开头，因为等下要从头开始读取
    fclose(file); // 关闭文件，释放系统资源
    return file_size;
}

void write_file(const char* file_path,unsigned char* data,long file_size){
    FILE* file = fopen(file_path,"wb"); // wb以二进制写模式打开文件
    if (!file) {
        printf("文件路径错误,或不存在,亦或无法打开\n");
        return;
    }
    fwrite(data,1,file_size,file);
    fclose(file);
}

// 复制文件到.gitlet/objects/并以哈希值命名
void save_blob(const char* file_path){
    // 1. 读取原始文件内容
    unsigned char* buffer = read_file(file_path);

    // 2. 计算 SHA-1
    long file_size = get_file_size(file_path);
    char* hash_str = calculate_data_sha1(buffer,file_size);

    // 3. 存储到 .gitlet/objects/hash
    char output_path[256];
    sprintf(output_path,".gitlet/objects/%s",hash_str);
    write_file(output_path,buffer,file_size);

    printf("Blob saved: %s\n", hash_str);
    free(buffer);
    free(hash_str);
}


