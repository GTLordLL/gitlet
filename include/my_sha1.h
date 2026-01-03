#ifndef __MY_SHA1_H__
#define __MY_SHA1_H__

#include <stddef.h>

char* calculate_data_sha1(const void* data,size_t length);
char* get_file_sha1(const char* filename);

#endif
