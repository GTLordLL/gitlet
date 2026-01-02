#!/bin/bash

# 清理环境
rm -rf .gitlet *.txt

echo "--- 1. 测试 INIT ---"
./bin/gitlet init
ls -a .gitlet  # 检查目录结构是否生成

./bin/gitlet log

./bin/gitlet status

echo -e "\n--- 2. 测试 ADD ---"
echo "hello world" > a.txt
echo "hello gitlet" > b.txt
./bin/gitlet add a.txt
./bin/gitlet add b.txt
# 此时应该在 .gitlet/objects 里生成了对应的 blob

./bin/gitlet status

echo -e "\n--- 3. 测试 COMMIT 1 ---"
./bin/gitlet commit "First commit with a and b"

./bin/gitlet log

./bin/gitlet status

echo -e "\n--- 4. 测试 RM (从暂存区移除) ---"
echo "new content" > c.txt
./bin/gitlet add c.txt
./bin/gitlet rm c.txt  # 应该取消暂存，且 c.txt 不会被删除（因为它没被追踪）
ls c.txt               # 检查文件是否还在

./bin/gitlet status

echo -e "\n--- 5. 测试 RM (追踪移除) ---"
./bin/gitlet rm a.txt  # a.txt 已经在 commit 1 中，此处应标记为删除并从物理磁盘抹除
ls a.txt               # 应该显示文件已不存在

./bin/gitlet status

echo -e "\n--- 6. 测试 COMMIT 2 (处理删除) ---"
./bin/gitlet commit "Second commit: removed a.txt"

./bin/gitlet log

./bin/gitlet status

echo -e "\n--- 7. 使用 Valgrind 进行最终压力测试 ---"
valgrind --leak-check=full ./bin/gitlet add b.txt # 虽然没变化，但也走一遍逻辑