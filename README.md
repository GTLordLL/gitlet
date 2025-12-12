# p1 — C Data Structures Library

这是一个小型的 C 数据结构练习项目，包含动态数组（vector）、栈、队列、双向链表（DLList）、双端队列（deque）、哈希表、图、并查集等实现。

目的：把代码保存在 GitHub 上以便备份与协作。

快速开始（本地）

- 克隆仓库（如果还没克隆）
```
git clone https://github.com/GTLordLL/p1.git
cd p1
```

- 构建并运行测试程序
```
make
./bin/test_main
```

如何把本地更改上传到 GitHub（步骤示例）

1. 检查当前状态
```
git status
```

2. 暂存更改并提交
```
git add .
git commit -m "简要说明这次改动"
```

3. 推送到远程（首次推送时需要设置上游）
```
git push -u origin main
```

如果你还没有在 GitHub 上创建远程仓库：
1. 登录 GitHub → New Repository → 填写仓库名（例如 `p1`）→ Create。
2. 按 GitHub 提示添加远程，例如：
```
git remote add origin https://github.com/你的用户名/p1.git
git push -u origin main
```

使用 SSH（可选）
```
# 生成 SSH key（如果还没有）
ssh-keygen -t ed25519 -C "you@example.com"
# 将公钥 (~/.ssh/id_ed25519.pub) 粘贴到 GitHub 设置
git remote set-url origin git@github.com:你的用户名/p1.git
git push -u origin main
```

关于忽略构建产物（你已配置）
- 仓库包含 `.gitignore`，会忽略 `build/`、`bin/`、`.vscode/` 以及常见中间文件。
- 如果误提交了构建产物，可用：
```
git rm -r --cached build bin .vscode
git add .gitignore
git commit -m "Stop tracking build/bin/.vscode"
git push
```

常见命令速查
- 查看提交历史： `git log --oneline -n 20`
- 创建分支： `git checkout -b feature/xxx`
- 合并分支：
  ```
  git checkout main
  git merge feature/xxx
  git push
  ```

如果希望我为你添加 CI（GitHub Actions）或 `README` 里填入更详细的 API/使用示例，回复告诉我即可。

---
（已将本文件提交到仓库）
