// 暂存区（Staging Area / Index）
#ifndef __MY_STAGE_H__
#define __MY_STAGE_H__

typedef struct StringHashMap StringHashMap;
typedef struct Commit Commit;

typedef struct StagingArea{
    StringHashMap *staged_files;  // 文件名 -> Blob哈希 (待提交)
    StringHashMap *removed_files; // 文件名 -> Blob哈希 (待删除/取消追踪)
}StagingArea;

Commit* get_head_commit(void);
unsigned char* serialize_map(StringHashMap* map,unsigned char* map_buffer);
void save_staging_area(StagingArea* stage);
unsigned char* deserialize_map(StringHashMap* map,unsigned char* map_buffer);
StagingArea* read_staging_area(void);
void free_staging_area(StagingArea* stage);
int cmd_add(const char *filename);
int cmd_rm(const char *filename);

#endif
