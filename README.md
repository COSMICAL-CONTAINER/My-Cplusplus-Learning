# My-Cplusplus-Learning

This repository is my personal **C++ learning notes**.  
It is organized as small, topic-focused source files so I can practice one concept at a time.

### What this repo is

- A study notebook in code form
- Focused on language features + STL + `std` utilities
- File-by-file experiments instead of one large application

### Learning map by language standard

#### C++11

- `C++11.cpp` : general C++11 syntax/features practice
- `Lambda.cpp` : lambda expressions
- `auto_and_decltype.cpp` : type deduction (`auto`, `decltype`)
- `move_semantics.cpp` : move semantics and rvalue references
- `thread.cpp` : basic multithreading
- `tuple.cpp` : tuple usage

#### C++14

- `C++14.cpp` : C++14 feature practice

#### C++17

- `C++17.cpp` : C++17 feature practice

### STL and `std` focused notes

#### STL containers

- `STL_vector.cpp`
- `STL_list.cpp`
- `STL_map.cpp`
- `STL_unorderMap.cpp`
- `STL_set.cpp`
- `STL_stack.cpp`
- `STL_queue.cpp`
- `STL_priorityQueue.cpp`
- `STL_useSkill.cpp` : container usage patterns and practical STL usage tips

#### STL algorithms and iterators

- `STL_algorithm.cpp`
- `STL_iterator.cpp`

#### `std` callable utilities

- `Std_function_bind.cpp` : `std::function` and `std::bind`

### Core C++ topics in this repo

- OOP basics: `Class_and_Object.cpp`, `Constructors_Destructors.cpp`, `Polymorphism.cpp`
- Inheritance: `Inheritance.cpp`, `Inheritance-virtual_Base_Class.cpp`
- Operator overloading: `Overloaded.cpp`, `Overloaded_Operator_and_Conversation.cpp`
- Templates: `Template.cpp`, `Template2.cpp`
- Memory management: `new_delete.cpp`, `c++_memory.cpp`, `c_memory.c`
- Language fundamentals: `const_Qualifier.cpp`, `inline.cpp`, `namespace.cpp`, `referance.cpp`
- Utilities/practice: `random.cpp`, `time.cpp`, `try_catch.cpp`, `stringclass.cpp`

### How to use this notebook

1. Pick one topic/file.
2. Compile and run it independently.
3. Compare similar files (for example, language feature vs STL usage).
4. Add your own comments and tiny experiments.

### Build example (Windows + g++)

```bash
g++ -std=c++17 .\Lambda.cpp -o .\Lambda.exe
.\Lambda.exe
```

---

这个仓库是我的个人 **C++ 学习笔记**。  
我把知识点拆成一个个小文件，方便按主题练习和复习。

### 仓库定位

- 代码形式的学习笔记
- 重点放在语言特性、STL、`std` 常用工具
- 以“单文件小实验”为主，而不是一个完整业务项目

### 按标准分层学习

#### C++11

- `C++11.cpp`：C++11 综合语法/特性练习
- `Lambda.cpp`：lambda 表达式
- `auto_and_decltype.cpp`：类型推导（`auto`、`decltype`）
- `move_semantics.cpp`：移动语义与右值引用
- `thread.cpp`：基础多线程
- `tuple.cpp`：tuple 使用

#### C++14

- `C++14.cpp`：C++14 特性练习

#### C++17

- `C++17.cpp`：C++17 特性练习

### STL 与 `std` 专题

#### STL 容器

- `STL_vector.cpp`
- `STL_list.cpp`
- `STL_map.cpp`
- `STL_unorderMap.cpp`
- `STL_set.cpp`
- `STL_stack.cpp`
- `STL_queue.cpp`
- `STL_priorityQueue.cpp`
- `STL_useSkill.cpp`：容器使用模式与 STL 实用技巧

#### STL 算法与迭代器

- `STL_algorithm.cpp`
- `STL_iterator.cpp`

#### `std` 可调用对象工具

- `Std_function_bind.cpp`：`std::function` 与 `std::bind`

### 其他核心主题

- 面向对象基础：`Class_and_Object.cpp`、`Constructors_Destructors.cpp`、`Polymorphism.cpp`
- 继承体系：`Inheritance.cpp`、`Inheritance-virtual_Base_Class.cpp`
- 运算符重载：`Overloaded.cpp`、`Overloaded_Operator_and_Conversation.cpp`
- 模板：`Template.cpp`、`Template2.cpp`
- 内存管理：`new_delete.cpp`、`c++_memory.cpp`、`c_memory.c`
- 语言基础：`const_Qualifier.cpp`、`inline.cpp`、`namespace.cpp`、`referance.cpp`
- 实用练习：`random.cpp`、`time.cpp`、`try_catch.cpp`、`stringclass.cpp`

### 使用方式（学习建议）

1. 每次选一个主题文件。
2. 单独编译、单独运行。
3. 对比相近文件（例如语言特性与 STL 用法）。
4. 在原文件上继续补充你自己的注释和实验。

### 编译示例（Windows + g++）

```bash
g++ -std=c++17 .\Lambda.cpp -o .\Lambda.exe
.\Lambda.exe
```
