# 编译器
CC = gcc

# 编译选项：-I 指定头文件路径, -g 开启调试信息, -Wall 开启所有警告
CFLAGS = -I./include -g -Wall -Wextra -O0

# 目录路径
SRC_DIR = src
OBJ_DIR = build
TEST_DIR = tests
BIN_DIR = bin

# 自动寻找源文件
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# 最终目标: 我们要生成的测试可执行文件
TARGET = $(BIN_DIR)/test_main

# 默认动作
all: dir $(TARGET)

# 1.链接: 将测试代码与自己写的库对象文件链接
$(TARGET): $(OBJS) $(TEST_DIR)/main.c
	@echo "Linking..."
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Build successful! Run ./$(TARGET) to test."

# 2.编译: 将src下的.c文件编译成.o文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# 创建build和bin文件夹
dir:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# 清理构建文件
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean dir