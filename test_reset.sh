#!/bin/bash

# 清理环境
rm -rf .gitlet *.txt

echo "=== 测试 RESET 命令 ==="
echo ""

echo "--- 1. 初始化仓库 ---"
./bin/gitlet init
echo ""

echo "--- 2. 创建第一个提交 ---"
echo "content1" > file1.txt
./bin/gitlet add file1.txt
./bin/gitlet commit "First commit: add file1"
echo ""

echo "--- 3. 创建第二个提交 ---"
echo "content2" > file2.txt
./bin/gitlet add file2.txt
./bin/gitlet commit "Second commit: add file2"
echo ""

echo "--- 4. 创建第三个提交 ---"
echo "content3" > file3.txt
./bin/gitlet add file3.txt
./bin/gitlet commit "Third commit: add file3"
echo ""

echo "--- 5. 查看提交历史，获取 commit 哈希值 ---"
echo "当前提交历史："
./bin/gitlet log
echo ""

echo "--- 6. 使用 global-log 获取所有 commit 哈希值 ---"
echo "所有提交的哈希值："
./bin/gitlet global-log | grep "^commit " | awk '{print $2}' > /tmp/commit_hashes.txt
cat /tmp/commit_hashes.txt
echo ""

# 获取各个提交的哈希值（按时间倒序，最新的在前面）
# 或者直接从 log 输出中提取（按提交顺序）
echo "--- 从 log 输出中提取提交哈希值 ---"
./bin/gitlet log | grep "^commit " | awk '{print $2}' > /tmp/log_hashes.txt
cat /tmp/log_hashes.txt
echo ""

# 从 log 中获取（最新的在前）
COMMIT3=$(head -n 1 /tmp/log_hashes.txt)
COMMIT2=$(head -n 2 /tmp/log_hashes.txt | tail -n 1)
COMMIT1=$(head -n 3 /tmp/log_hashes.txt | tail -n 1)
INIT_COMMIT=$(head -n 4 /tmp/log_hashes.txt | tail -n 1)

echo "提取的哈希值："
echo "  第三个提交 (最新): $COMMIT3"
echo "  第二个提交: $COMMIT2"
echo "  第一个提交: $COMMIT1"
echo "  初始提交: $INIT_COMMIT"
echo ""

echo "--- 7. 检查当前工作目录文件 ---"
echo "当前工作目录文件："
ls -1 *.txt 2>/dev/null || echo "无 .txt 文件"
echo ""

echo "--- 8. 测试 RESET 到第二个提交 ($COMMIT2) ---"
./bin/gitlet reset "$COMMIT2"
echo ""

echo "--- 9. 验证文件状态（应该只有 file1.txt 和 file2.txt） ---"
echo "当前工作目录文件："
ls -1 *.txt 2>/dev/null || echo "无 .txt 文件"
echo ""

echo "检查 file1.txt 内容："
cat file1.txt 2>/dev/null || echo "file1.txt 不存在"
echo ""

echo "检查 file2.txt 内容："
cat file2.txt 2>/dev/null || echo "file2.txt 不存在"
echo ""

echo "检查 file3.txt（应该不存在）："
if [ -f file3.txt ]; then
    echo "错误：file3.txt 仍然存在！"
else
    echo "正确：file3.txt 已被删除"
fi
echo ""

echo "--- 10. 查看当前提交历史（应该只显示到第二个提交） ---"
./bin/gitlet log
echo ""

echo "--- 11. 查看状态（暂存区应该为空） ---"
./bin/gitlet status
echo ""

echo "--- 12. 测试 RESET 到第一个提交 ($COMMIT1) ---"
./bin/gitlet reset "$COMMIT1"
echo ""

echo "--- 13. 验证文件状态（应该只有 file1.txt） ---"
echo "当前工作目录文件："
ls -1 *.txt 2>/dev/null || echo "无 .txt 文件"
echo ""

echo "检查 file1.txt 内容："
cat file1.txt 2>/dev/null || echo "file1.txt 不存在"
echo ""

echo "检查 file2.txt（应该不存在）："
if [ -f file2.txt ]; then
    echo "错误：file2.txt 仍然存在！"
else
    echo "正确：file2.txt 已被删除"
fi
echo ""

echo "--- 14. 测试 RESET 到初始提交 ($INIT_COMMIT) ---"
./bin/gitlet reset "$INIT_COMMIT"
echo ""

echo "--- 15. 验证文件状态（初始提交应该没有文件） ---"
echo "当前工作目录文件："
ls -1 *.txt 2>/dev/null || echo "无 .txt 文件（正确：初始提交没有文件）"
echo ""

echo "--- 16. 查看提交历史（应该只显示初始提交） ---"
./bin/gitlet log
echo ""

echo "--- 17. 测试 RESET 回到第三个提交 ($COMMIT3) ---"
./bin/gitlet reset "$COMMIT3"
echo ""

echo "--- 18. 最终验证文件状态 ---"
echo "当前工作目录文件："
ls -1 *.txt 2>/dev/null || echo "无 .txt 文件"
echo ""

echo "检查所有文件内容："
for file in file1.txt file2.txt file3.txt; do
    if [ -f "$file" ]; then
        echo "$file: $(cat $file)"
    else
        echo "$file: 不存在"
    fi
done
echo ""

echo "--- 19. 最终提交历史 ---"
./bin/gitlet log
echo ""

echo "=== 测试完成 ==="
echo ""
echo "测试要点："
echo "  1. reset 命令使用 objects 目录中的 commit 哈希文件名（40字符）"
echo "  2. reset 会更新工作目录文件到目标提交的状态"
echo "  3. reset 会删除目标提交中不存在的文件"
echo "  4. reset 会更新当前分支的 HEAD 指针"
echo "  5. reset 会清空暂存区"
echo ""

echo "--- 20. 使用 Valgrind 检测内存泄漏 ---"
echo ""

# 重新获取所有 commit 哈希用于测试
./bin/gitlet log | grep "^commit " | awk '{print $2}' > /tmp/test_log_hashes.txt
TEST_COMMIT3=$(head -n 1 /tmp/test_log_hashes.txt)
TEST_COMMIT2=$(head -n 2 /tmp/test_log_hashes.txt | tail -n 1)
TEST_COMMIT1=$(head -n 3 /tmp/test_log_hashes.txt | tail -n 1)

if [ -z "$TEST_COMMIT3" ] || [ -z "$TEST_COMMIT2" ]; then
    echo "警告：无法获取 commit 哈希，跳过 Valgrind 测试"
else
    echo "--- 测试 status 命令的内存泄漏 ---"
    echo "运行: valgrind --leak-check=full ./bin/gitlet status"
    valgrind --leak-check=full --error-exitcode=1 ./bin/gitlet status > /tmp/valgrind_status_output.txt 2>&1
    STATUS_EXIT=$?
    cat /tmp/valgrind_status_output.txt
    echo ""
    
    # 检查内存泄漏摘要
    if grep -q "definitely lost: 0 bytes" /tmp/valgrind_status_output.txt && \
       grep -q "indirectly lost: 0 bytes" /tmp/valgrind_status_output.txt; then
        echo "✓ status 命令：未检测到内存泄漏"
    else
        echo "✗ status 命令：检测到内存泄漏"
        echo "泄漏详情："
        grep -A 8 "LEAK SUMMARY" /tmp/valgrind_status_output.txt | head -n 9
    fi
    echo ""
    
    echo "--- 测试 reset 命令的内存泄漏（重置到第二个提交） ---"
    echo "运行: valgrind --leak-check=full ./bin/gitlet reset $TEST_COMMIT2"
    valgrind --leak-check=full --error-exitcode=1 ./bin/gitlet reset "$TEST_COMMIT2" > /tmp/valgrind_reset_output.txt 2>&1
    RESET_EXIT=$?
    cat /tmp/valgrind_reset_output.txt
    echo ""
    
    # 检查内存泄漏摘要
    if grep -q "definitely lost: 0 bytes" /tmp/valgrind_reset_output.txt && \
       grep -q "indirectly lost: 0 bytes" /tmp/valgrind_reset_output.txt; then
        echo "✓ reset 命令：未检测到内存泄漏"
    else
        echo "✗ reset 命令：检测到内存泄漏"
        echo "泄漏详情："
        grep -A 8 "LEAK SUMMARY" /tmp/valgrind_reset_output.txt | head -n 9
    fi
    echo ""
    
    # 恢复到最后提交，以便后续测试（如果有）
    if [ -n "$TEST_COMMIT3" ]; then
        ./bin/gitlet reset "$TEST_COMMIT3" > /dev/null 2>&1
    fi
    
    echo "--- Valgrind 测试总结 ---"
    # 检查是否有内存泄漏（definitely lost 或 indirectly lost 不为 0）
    STATUS_HAS_LEAK=$(grep "definitely lost:" /tmp/valgrind_status_output.txt | grep -v "definitely lost: 0 bytes" | wc -l)
    RESET_HAS_LEAK=$(grep "definitely lost:" /tmp/valgrind_reset_output.txt | grep -v "definitely lost: 0 bytes" | wc -l)
    
    if [ "$STATUS_HAS_LEAK" -eq 0 ] && [ "$RESET_HAS_LEAK" -eq 0 ]; then
        echo "✓ 所有命令通过内存泄漏检测（definitely lost 和 indirectly lost 均为 0）"
    else
        echo "✗ 检测到内存泄漏，请检查上述 Valgrind 输出详情"
    fi
fi
echo ""

# 清理临时文件
rm -f /tmp/commit_hashes.txt /tmp/log_hashes.txt /tmp/test_log_hashes.txt /tmp/valgrind_status_output.txt /tmp/valgrind_reset_output.txt

