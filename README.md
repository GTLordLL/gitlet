<details><summary><strong>中文 / Chinese</strong></summary>

# C语言数据结构与算法库

这是一个用C语言实现的常用数据结构与算法库，提供了多种基础数据结构和经典算法的实现，适合学习和实际项目使用。

## 支持的数据结构与算法

### 数据结构
- **二叉搜索树 (BST)** - 高效的动态查找、插入和删除操作
- **循环数组 (Circular Array)** - 高效的数组循环操作
- **双端队列 (Deque)** - 支持两端插入和删除的队列
- **双向链表 (DLList)** - 支持前后遍历和节点操作的链表
- **通用数组列表 (Generic AList)** - 可存储任意类型的动态数组
- **图 (Graph)** - 图的邻接表表示及相关操作
- **哈希表 (Hashmap)** - 基于哈希的键值对存储结构
- **整数向量 (Int Vector)** - 专门用于存储整数的动态数组
- **优先级队列 (Priority Queue)** - 基于堆的优先级队列
- **队列 (Queue)** - 先进先出的数据结构
- **栈 (Stack)** - 后进先出的数据结构
- **并查集 (Union Find Set)** - 高效的集合合并与查找结构

### 算法
- **Dijkstra算法** - 最短路径算法
- **Kruskal算法** - 最小生成树算法

## 项目结构

```
p1-main/
├── include/        # 头文件目录
│   ├── my_avlt.h
│   ├── my_bst.h
│   ├── my_circularArray.h
│   ├── my_deque.h
│   ├── my_dijkstra.h
│   ├── my_DLList.h
│   ├── my_generic_AList.h
│   ├── my_graph.h
│   ├── my_hashmap.h
│   ├── my_int_vector.h
│   ├── my_kruskal.h
│   ├── my_priorityQueue.h
│   ├── my_queue.h
│   ├── my_stack.h
│   └── my_unionFindSet.h
├── src/            # 源文件目录
│   ├── my_avlt.c
│   ├── my_bst.c
│   ├── my_circularArray.c
│   ├── my_deque.c
│   ├── my_dijkstra.c
│   ├── my_DLList.c
│   ├── my_generic_AList.c
│   ├── my_graph.c
│   ├── my_hashmap.c
│   ├── my_int_vector.c
│   ├── my_kruskal.c
│   ├── my_priorityQueue.c
│   ├── my_queue.c
│   ├── my_stack.c
│   └── my_unionFindSet.c
├── tests/          # 测试文件目录
│   └── main.c
├── build/          # 构建输出目录（自动创建）
├── bin/            # 可执行文件目录（自动创建）
├── Makefile        # 构建脚本
└── README.md       # 项目说明文档
```

## 构建与运行

### 环境要求
- GCC编译器
- Make工具

### 构建步骤

1. 克隆仓库到本地
```bash
git clone <repository-url>
cd p1-main
```

2. 使用Makefile构建项目
```bash
make all
```

3. 运行测试程序
```bash
./bin/test_main
```

4. 清理构建文件
```bash
make clean
```

## 使用示例

以下是二叉搜索树的基本使用示例：

```c
#include <stdio.h>
#include <stdlib.h>
#include "my_bst.h"

int main(void) {
    // 创建二叉搜索树
    BinarySearchTree* myTree = create_BST();
    if (!myTree) return 1;

    // 插入节点
    insert_bst(myTree, 50, 500);
    insert_bst(myTree, 30, 300);
    insert_bst(myTree, 70, 700);

    // 遍历输出
    printf("中序遍历: ");
    print_inorderBST(myTree);
    printf("\n");

    // 更新节点
    insert_bst(myTree, 50, 555); // 更新值

    // 删除节点
    delete_bst(myTree, 30);

    // 清理内存
    free_BST(myTree);

    return EXIT_SUCCESS;
}
```

## 如何扩展

1. 在`include/`目录下创建新的数据结构头文件
2. 在`src/`目录下创建对应的实现文件
3. 修改`tests/main.c`添加测试代码
4. 使用`make all`重新构建

## 贡献指南

欢迎提交Issue和Pull Request来帮助改进这个库！

1. Fork仓库
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启Pull Request

## 许可证

本项目采用MIT许可证，详情请查看LICENSE文件。

## 联系方式

如有问题或建议，欢迎通过以下方式联系：
- GitHub Issues: <https://github.com/GTLordLL/p1/issues>

---

**注意**：本项目仅用于学习和参考，在生产环境中使用前请进行充分测试。

</details>

<details><summary><strong>英文 / English</strong></summary>

# C Language Data Structures and Algorithms Library

This is a library of commonly used data structures and algorithms implemented in C language, suitable for learning and practical project use.

## Supported Data Structures and Algorithms

### Data Structures
- **Binary Search Tree (BST)** - Efficient dynamic search, insertion, and deletion operations
- **Circular Array** - Efficient array circular operations
- **Deque** - Queue supporting insertion and deletion at both ends
- **Doubly Linked List (DLList)** - Linked list supporting forward and backward traversal and node operations
- **Generic Array List (AList)** - Dynamic array that can store any type
- **Graph** - Adjacency list representation of graphs and related operations
- **Hashmap** - Hash-based key-value storage structure
- **Int Vector** - Dynamic array specifically for storing integers
- **Priority Queue** - Heap-based priority queue
- **Queue** - First-in, first-out data structure
- **Stack** - Last-in, first-out data structure
- **Union Find Set** - Efficient set merging and searching structure

### Algorithms
- **Dijkstra's Algorithm** - Shortest path algorithm
- **Kruskal's Algorithm** - Minimum spanning tree algorithm

## Project Structure

```
p1-main/
├── include/        # Header files directory
│   ├── my_bst.h
│   ├── my_circularArray.h
│   ├── my_deque.h
│   ├── my_dijkstra.h
│   ├── my_DLList.h
│   ├── my_generic_AList.h
│   ├── my_graph.h
│   ├── my_hashmap.h
│   ├── my_int_vector.h
│   ├── my_kruskal.h
│   ├── my_priorityQueue.h
│   ├── my_queue.h
│   ├── my_stack.h
│   └── my_unionFindSet.h
├── src/            # Source files directory
│   ├── my_bst.c
│   ├── my_circularArray.c
│   ├── my_deque.c
│   ├── my_dijkstra.c
│   ├── my_DLList.c
│   ├── my_generic_AList.c
│   ├── my_graph.c
│   ├── my_hashmap.c
│   ├── my_int_vector.c
│   ├── my_kruskal.c
│   ├── my_priorityQueue.c
│   ├── my_queue.c
│   ├── my_stack.c
│   └── my_unionFindSet.c
├── tests/          # Test files directory
│   └── main.c
├── build/          # Build output directory (automatically created)
├── bin/            # Executable files directory (automatically created)
├── Makefile        # Build script
└── README.md       # Project documentation
```

## Build and Run

### Requirements
- GCC compiler
- Make tool

### Build Steps

1. Clone the repository to your local machine
```bash
git clone <repository-url>
cd p1-main
```

2. Build the project using Makefile
```bash
make all
```

3. Run the test program
```bash
./bin/test_main
```

4. Clean build files
```bash
make clean
```

## Usage Example

Here's a basic usage example of the Binary Search Tree:

```c
#include <stdio.h>
#include <stdlib.h>
#include "my_bst.h"

int main(void) {
    // Create a binary search tree
    BinarySearchTree* myTree = create_BST();
    if (!myTree) return 1;

    // Insert nodes
    insert_bst(myTree, 50, 500);
    insert_bst(myTree, 30, 300);
    insert_bst(myTree, 70, 700);

    // Traverse and output
    printf("In-order traversal: ");
    print_inorderBST(myTree);
    printf("\n");

    // Update node
    insert_bst(myTree, 50, 555); // Update value

    // Delete node
    delete_bst(myTree, 30);

    // Free memory
    free_BST(myTree);

    return EXIT_SUCCESS;
}
```

## How to Extend

1. Create a new data structure header file in the `include/` directory
2. Create the corresponding implementation file in the `src/` directory
3. Modify `tests/main.c` to add test code
4. Rebuild using `make all`

## Contribution Guidelines

Contributions are welcome! Please follow these steps to submit your changes:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Contact

If you have any questions or suggestions, please feel free to contact us:
- GitHub Issues: <https://github.com/GTLordLL/p1/issues>

---

**Note**: This project is for learning and reference purposes only. Please thoroughly test before using in production environments.

</details>
