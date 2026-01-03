<details><summary><strong>中文 / Chinese</strong></summary>

# Gitlet - 简易的Git版本控制系统

Gitlet 是一个用 C 语言实现的简化版 Git 版本控制系统，旨在帮助理解版本控制系统的核心原理和实现机制。

## 📋 项目简介

Gitlet 实现了 Git 的核心功能，包括文件版本管理、提交历史追踪、分支管理等。项目采用模块化设计，使用自定义数据结构（哈希表、双向链表）和序列化机制来存储和管理版本信息。

## ✨ 功能特性

- **仓库初始化** (`init`): 创建 `.gitlet` 目录结构，初始化版本控制系统
- **文件暂存** (`add`): 将文件添加到暂存区，准备提交
- **文件移除** (`rm`): 从暂存区移除文件或标记删除
- **提交更改** (`commit`): 创建新的提交，保存当前工作区状态
- **查看历史** (`log`): 显示提交历史记录
- **查看状态** (`status`): 显示当前分支、暂存文件和修改状态
- **文件检出** (`checkout`): 从 HEAD 或指定提交恢复文件

## 📁 项目结构

```
Gitlet/
├── src/              # 源代码文件
│   ├── my_init.c    # 初始化功能
│   ├── my_stage.c   # 暂存区管理
│   ├── my_commit.c  # 提交管理
│   ├── my_status.c  # 状态查看
│   ├── my_checkout.c # 文件检出
│   ├── my_blob.c    # Blob对象处理
│   ├── my_sha1.c    # SHA-1哈希计算
│   ├── my_str_hashmap.c  # 字符串哈希表
│   └── my_str_DLList.c   # 双向链表
├── include/          # 头文件
├── build/            # 编译生成的目标文件
├── bin/              # 可执行文件
│   └── gitlet        # 主程序
├── tests/            # 测试代码
│   └── main.c        # 主程序入口
└── Makefile          # 构建脚本
```

## 🔧 环境要求

- **编译器**: GCC
- **构建工具**: Make
- **依赖库**: OpenSSL (用于 SHA-1 哈希计算)
  - Ubuntu/Debian: `sudo apt-get install libssl-dev`
  - CentOS/RHEL: `sudo yum install openssl-devel`
  - macOS: `brew install openssl`

## 🚀 构建与运行

### 1. 克隆仓库

```bash
git clone <repository-url>
cd Gitlet
```

### 2. 构建项目

```bash
make all
```

这将编译所有源文件并生成可执行文件 `bin/gitlet`。

### 3. 运行程序

```bash
./bin/gitlet <command> [arguments]
```

### 4. 清理构建文件

```bash
make clean
```

## 📖 使用示例

### 初始化仓库

```bash
./bin/gitlet init
```

这将在当前目录创建 `.gitlet` 目录结构。

### 添加文件到暂存区

```bash
./bin/gitlet add file.txt
```

### 提交更改

```bash
./bin/gitlet commit "Initial commit"
```

### 查看提交历史

```bash
./bin/gitlet log
```

### 查看仓库状态

```bash
./bin/gitlet status
```

### 从 HEAD 恢复文件

```bash
./bin/gitlet checkout -- file.txt
```

### 从指定提交恢复文件

```bash
./bin/gitlet checkout <commit-id> -- file.txt
```

### 移除文件

```bash
./bin/gitlet rm file.txt
```

## 📚 命令说明

| 命令 | 用法 | 说明 |
|------|------|------|
| `init` | `gitlet init` | 初始化 Gitlet 仓库 |
| `add` | `gitlet add <file>` | 将文件添加到暂存区 |
| `rm` | `gitlet rm <file>` | 从暂存区移除文件或标记删除 |
| `commit` | `gitlet commit "<message>"` | 提交暂存区的更改 |
| `log` | `gitlet log` | 显示提交历史 |
| `status` | `gitlet status` | 显示仓库状态 |
| `checkout` | `gitlet checkout -- <file>` | 从 HEAD 恢复文件 |
| `checkout` | `gitlet checkout <id> -- <file>` | 从指定提交恢复文件 |

## 🏗️ 技术实现

### 核心数据结构

- **Commit**: 提交对象，包含父提交哈希、时间戳、提交信息和文件映射
- **Blob**: 文件对象，存储文件内容的 SHA-1 哈希
- **StagingArea**: 暂存区，管理待提交的文件
- **StringHashMap**: 字符串键值对哈希表，用于文件映射
- **StringDLLList**: 双向链表，用于哈希表冲突处理

### 存储机制

- **对象存储**: 所有对象（Commit、Blob）存储在 `.gitlet/objects/` 目录，以 SHA-1 哈希值命名
- **引用系统**: 分支引用存储在 `.gitlet/refs/heads/` 目录
- **HEAD 指针**: `.gitlet/HEAD` 文件指向当前分支
- **暂存区**: `.gitlet/index` 文件存储暂存区状态

### 序列化

项目实现了二进制序列化机制，将内存中的数据结构转换为字节流存储到磁盘，支持：
- Commit 对象的序列化/反序列化
- StagingArea 的序列化/反序列化
- 高效的存储和读取

## 🔍 内部结构

Gitlet 仓库的目录结构：

```
.gitlet/
├── objects/          # 所有对象（Blob 和 Commit）
├── refs/
│   └── heads/        # 分支引用文件
│       └── master    # master 分支指向的提交哈希
├── HEAD              # 指向当前分支（如 "ref: refs/heads/master"）
└── index             # 暂存区序列化文件
```

## 🧪 测试

项目包含测试代码，可以通过修改 `tests/main.c` 来添加自定义测试用例。

## 🤝 贡献指南

欢迎提交 Issue 和 Pull Request 来帮助改进这个项目！

1. Fork 仓库
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

## 📝 开发说明

### 如何扩展功能

1. 在 `include/` 目录下创建新的数据结构头文件
2. 在 `src/` 目录下创建对应的实现文件
3. 在 `tests/main.c` 中添加命令处理逻辑
4. 使用 `make all` 重新构建

### 代码规范

- 使用有意义的变量和函数名
- 添加必要的注释说明
- 确保内存管理正确（malloc/free 配对）
- 处理错误情况并返回适当的错误码

## 📄 许可证

本项目采用 MIT 许可证，详情请查看 LICENSE 文件。

## 📧 联系方式

如有问题或建议，欢迎通过以下方式联系：
- GitHub Issues: <https://github.com/GTLordLL/gitlet/issues>

---

**注意**：本项目仅用于学习和参考，在生产环境中使用前请进行充分测试。

</details>

<details><summary><strong>英文 / English</strong></summary>

# Gitlet - A Simplified Git Version Control System

Gitlet is a simplified Git version control system implemented in C, designed to help understand the core principles and implementation mechanisms of version control systems.

## 📋 Project Overview

Gitlet implements core Git functionality including file version management, commit history tracking, and branch management. The project uses a modular design with custom data structures (hash tables, doubly-linked lists) and serialization mechanisms to store and manage version information.

## ✨ Features

- **Repository Initialization** (`init`): Create `.gitlet` directory structure and initialize the version control system
- **File Staging** (`add`): Add files to the staging area for commit
- **File Removal** (`rm`): Remove files from staging area or mark for deletion
- **Commit Changes** (`commit`): Create new commits to save current working directory state
- **View History** (`log`): Display commit history
- **View Status** (`status`): Show current branch, staged files, and modification status
- **File Checkout** (`checkout`): Restore files from HEAD or specific commits

## 📁 Project Structure

```
Gitlet/
├── src/              # Source code files
│   ├── my_init.c    # Initialization functionality
│   ├── my_stage.c   # Staging area management
│   ├── my_commit.c  # Commit management
│   ├── my_status.c  # Status viewing
│   ├── my_checkout.c # File checkout
│   ├── my_blob.c    # Blob object handling
│   ├── my_sha1.c    # SHA-1 hash calculation
│   ├── my_str_hashmap.c  # String hash table
│   └── my_str_DLList.c   # Doubly-linked list
├── include/          # Header files
├── build/            # Compiled object files
├── bin/              # Executable files
│   └── gitlet        # Main program
├── tests/            # Test code
│   └── main.c        # Main program entry
└── Makefile          # Build script
```

## 🔧 Requirements

- **Compiler**: GCC
- **Build Tool**: Make
- **Dependencies**: OpenSSL (for SHA-1 hash calculation)
  - Ubuntu/Debian: `sudo apt-get install libssl-dev`
  - CentOS/RHEL: `sudo yum install openssl-devel`
  - macOS: `brew install openssl`

## 🚀 Build and Run

### 1. Clone Repository

```bash
git clone <repository-url>
cd Gitlet
```

### 2. Build Project

```bash
make all
```

This will compile all source files and generate the executable `bin/gitlet`.

### 3. Run Program

```bash
./bin/gitlet <command> [arguments]
```

### 4. Clean Build Files

```bash
make clean
```

## 📖 Usage Examples

### Initialize Repository

```bash
./bin/gitlet init
```

This will create the `.gitlet` directory structure in the current directory.

### Add Files to Staging Area

```bash
./bin/gitlet add file.txt
```

### Commit Changes

```bash
./bin/gitlet commit "Initial commit"
```

### View Commit History

```bash
./bin/gitlet log
```

### View Repository Status

```bash
./bin/gitlet status
```

### Restore File from HEAD

```bash
./bin/gitlet checkout -- file.txt
```

### Restore File from Specific Commit

```bash
./bin/gitlet checkout <commit-id> -- file.txt
```

### Remove File

```bash
./bin/gitlet rm file.txt
```

## 📚 Command Reference

| Command | Usage | Description |
|---------|-------|-------------|
| `init` | `gitlet init` | Initialize Gitlet repository |
| `add` | `gitlet add <file>` | Add file to staging area |
| `rm` | `gitlet rm <file>` | Remove file from staging area or mark for deletion |
| `commit` | `gitlet commit "<message>"` | Commit staged changes |
| `log` | `gitlet log` | Display commit history |
| `status` | `gitlet status` | Display repository status |
| `checkout` | `gitlet checkout -- <file>` | Restore file from HEAD |
| `checkout` | `gitlet checkout <id> -- <file>` | Restore file from specific commit |

## 🏗️ Technical Implementation

### Core Data Structures

- **Commit**: Commit object containing parent commit hash, timestamp, commit message, and file mappings
- **Blob**: File object storing SHA-1 hash of file content
- **StagingArea**: Staging area managing files to be committed
- **StringHashMap**: String key-value hash table for file mappings
- **StringDLLList**: Doubly-linked list for hash table collision handling

### Storage Mechanism

- **Object Storage**: All objects (Commit, Blob) are stored in `.gitlet/objects/` directory, named by SHA-1 hash value
- **Reference System**: Branch references are stored in `.gitlet/refs/heads/` directory
- **HEAD Pointer**: `.gitlet/HEAD` file points to current branch
- **Staging Area**: `.gitlet/index` file stores staging area state

### Serialization

The project implements binary serialization mechanism to convert in-memory data structures into byte streams for disk storage, supporting:
- Commit object serialization/deserialization
- StagingArea serialization/deserialization
- Efficient storage and retrieval

## 🔍 Internal Structure

Gitlet repository directory structure:

```
.gitlet/
├── objects/          # All objects (Blob and Commit)
├── refs/
│   └── heads/        # Branch reference files
│       └── master    # Commit hash pointed by master branch
├── HEAD              # Points to current branch (e.g., "ref: refs/heads/master")
└── index             # Staging area serialization file
```

## 🧪 Testing

The project includes test code. You can add custom test cases by modifying `tests/main.c`.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit Issues and Pull Requests.

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📝 Development Notes

### How to Extend Functionality

1. Create new data structure header files in `include/` directory
2. Create corresponding implementation files in `src/` directory
3. Add command handling logic in `tests/main.c`
4. Rebuild using `make all`

### Code Standards

- Use meaningful variable and function names
- Add necessary comments
- Ensure proper memory management (malloc/free pairing)
- Handle error cases and return appropriate error codes

## 📄 License

This project is licensed under the MIT License. See the LICENSE file for details.

## 📧 Contact

For questions or suggestions, please contact:
- GitHub Issues: <https://github.com/GTLordLL/gitlet/issues>

---

**Note**: This project is for learning and reference purposes only. Please test thoroughly before using in production environments.

</details>
