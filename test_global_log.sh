#!/bin/bash

# 清理环境
rm -rf .gitlet *.txt

echo "=== 测试 GLOBAL-LOG 命令 ==="
echo ""

echo "--- 1. 初始化仓库 ---"
./bin/gitlet init
echo ""

echo "--- 2. 在 master 分支上创建第一个提交 ---"
echo "master file 1" > file1.txt
./bin/gitlet add file1.txt
./bin/gitlet commit "First commit on master"
echo ""

echo "--- 3. 在 master 分支上创建第二个提交 ---"
echo "master file 2" > file2.txt
./bin/gitlet add file2.txt
./bin/gitlet commit "Second commit on master"
echo ""

echo "--- 4. 查看当前分支 (master) 的 log ---"
echo "应该显示 2 个提交："
./bin/gitlet log
echo ""

echo "--- 5. 创建新分支 feature ---"
./bin/gitlet branch feature
echo ""

echo "--- 6. 切换到 feature 分支 ---"
./bin/gitlet checkout feature
echo ""

echo "--- 7. 在 feature 分支上创建提交 ---"
echo "feature file 1" > file3.txt
./bin/gitlet add file3.txt
./bin/gitlet commit "First commit on feature branch"
echo ""

echo "--- 8. 在 feature 分支上创建第二个提交 ---"
echo "feature file 2" > file4.txt
./bin/gitlet add file4.txt
./bin/gitlet commit "Second commit on feature branch"
echo ""

echo "--- 9. 查看当前分支 (feature) 的 log ---"
echo "应该显示 4 个提交（2个master + 2个feature）："
./bin/gitlet log
echo ""

echo "--- 10. 切换回 master 分支 ---"
./bin/gitlet checkout master
echo ""

echo "--- 11. 在 master 分支上创建第三个提交 ---"
echo "master file 3" > file5.txt
./bin/gitlet add file5.txt
./bin/gitlet commit "Third commit on master"
echo ""

echo "--- 12. 查看当前分支 (master) 的 log ---"
echo "应该显示 3 个提交（仅在 master 分支上）："
./bin/gitlet log
echo ""

echo "--- 13. 测试 GLOBAL-LOG：应该显示所有提交（包括其他分支上的） ---"
echo "应该显示 5 个提交（3个master + 2个feature）："
./bin/gitlet global-log
echo ""

echo "--- 14. 验证 objects 目录中的文件类型 ---"
echo "列出 objects 目录（包含 Commit 和 Blob 对象）："
ls -la .gitlet/objects/ | head -20
echo ""

echo "=== 测试完成 ==="
echo ""
echo "预期结果："
echo "  - log 命令只显示当前分支的提交历史"
echo "  - global-log 命令显示所有分支的所有提交"
echo "  - global-log 应该正确识别 Commit 对象（以 'commit ' 开头）"
echo "  - global-log 应该跳过 Blob 对象"

