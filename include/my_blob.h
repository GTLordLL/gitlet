/* Blob 的持久化（文件备份）
Blob 是最简单的对象。它的内容就是原始文件的字节。
*/
#ifndef __MY_BLOB_H__
#define __MY_BLOB_H__

unsigned char* read_file(const char* file_path);
long get_file_size(const char* file_path);
void write_file(const char* file_path,unsigned char* data,long file_size);
void save_blob(const char* file_path);

#endif
