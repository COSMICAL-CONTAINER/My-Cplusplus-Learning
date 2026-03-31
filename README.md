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
- `lambda.cpp` : lambda expressions
- `auto_and_decltype.cpp` : type deduction (`auto`, `decltype`)
- `move_semantics.cpp` : move semantics and rvalue references
- `thread.cpp` : basic multithreading
- `tuple.cpp` : tuple usage

#### C++14

- `C++14.cpp` : C++14 feature practice

#### C++17

- `C++17.cpp` : C++17 feature practice

#### C++20

- `C++20.cpp` : C++20 feature practice

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

- `std_function_bind.cpp` : `std::function` and `std::bind`

### Core C++ topics in this repo

- OOP basics: `class_and_object.cpp`, `constructors_destructors.cpp`, `polymorphism.cpp`
- Inheritance: `inheritance.cpp`, `inheritance_virtual_base_class.cpp`
- Operator overloading: `overloaded.cpp`, `overloaded_operator_and_conversation.cpp`
- Templates: `template.cpp`, `template2.cpp`
- Memory management: `new_delete.cpp`, `memory_in_C++.cpp`, `memory_in_C.c`
- Language fundamentals: `const_qualifier.cpp`, `inline.cpp`, `namespace.cpp`, `referance.cpp`
- Utilities/practice: `random.cpp`, `time.cpp`, `try_catch.cpp`, `stringclass.cpp`

### Design Patterns roadmap

Status legend: `✅ done & uploaded` · `⬜ planned`

#### Creational patterns

| Pattern | Status | File |
|---|---|---|
| Factory Method | ✅ | `designPattern_factoryMethod.cpp` |
| Abstract Factory | ✅ | `designPattern_abstractFactoryMethod.cpp` |
| Builder | ✅ | `designPattern_builderMethod.cpp` |
| Prototype | ✅ | `designPattern_prototypeMethod.cpp` |
| Singleton | ✅ | `designPattern_SingletonMethod.cpp` |

#### Structural patterns

| Pattern | Status | File |
|---|---|---|
| Adapter | ✅ | `designPattern_adapterMethod.cpp` |
| Bridge | ✅ | `designPattern_bridgeMethod.cpp` |
| Composite | ✅ | `designPattern_compositeMethod.cpp` |
| Decorator | ✅ | `designPattern_decoratorMethod.cpp` |
| Facade | ✅ | `designPattern_facadeMethod.cpp` |
| Flyweight | ✅ | `designPattern_flyweightMethod.cpp` |
| Proxy | ✅ | `designPattern_proxyMethod.cpp` |

#### Behavioral patterns

| Pattern | Status | File |
|---|---|---|
| Chain of Responsibility | ✅ | `designPattern_chainOfResponsibilityMethod.cpp` |
| Command | ✅ | `designPattern_commandMethod.cpp` |
| Iterator | ✅ | `designPattern_iteratorMethod.cpp` |
| Mediator | ✅ | `designPattern_MediatorMethod.cpp` |
| Memento | ✅ | `designPattern_MementoMethod.cpp` |
| Observer | ✅ | `designPattern_observerMethod.cpp` |
| State | ✅ | `designPattern_stateMethod.cpp` |
| Strategy | ⬜ | - |
| Template Method | ⬜ | - |
| Visitor | ⬜ | - |

### How to use this notebook

1. Pick one topic/file.
2. Compile and run it independently.
3. Compare similar files (for example, language feature vs STL usage).
4. Add your own comments and tiny experiments.

### Build example (Windows + g++)

```bash
g++ -std=c++17 .\lambda.cpp -o .\lambda.exe
.\lambda.exe
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
- `lambda.cpp`：lambda 表达式
- `auto_and_decltype.cpp`：类型推导（`auto`、`decltype`）
- `move_semantics.cpp`：移动语义与右值引用
- `thread.cpp`：基础多线程
- `tuple.cpp`：tuple 使用

#### C++14

- `C++14.cpp`：C++14 特性练习

#### C++17

- `C++17.cpp`：C++17 特性练习

#### C++20

- `C++20.cpp`：C++20 特性练习

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

- `std_function_bind.cpp`：`std::function` 与 `std::bind`

### 其他核心主题

- 面向对象基础：`class_and_object.cpp`、`constructors_destructors.cpp`、`polymorphism.cpp`
- 继承体系：`inheritance.cpp`、`inheritance_virtual_base_class.cpp`
- 运算符重载：`overloaded.cpp`、`overloaded_operator_and_conversation.cpp`
- 模板：`template.cpp`、`template2.cpp`
- 内存管理：`new_delete.cpp`、`memory_in_C++.cpp`、`memory_in_C.c`
- 语言基础：`const_qualifier.cpp`、`inline.cpp`、`namespace.cpp`、`referance.cpp`
- 实用练习：`random.cpp`、`time.cpp`、`try_catch.cpp`、`stringclass.cpp`

### 设计模式目录

状态说明：`✅ 已完成并上传` · `⬜ 计划中`

#### 创建型模式

| 模式 | 状态 | 文件 |
|---|---|---|
| 工厂方法（Factory Method） | ✅ | `designPattern_factoryMethod.cpp` |
| 抽象工厂（Abstract Factory） | ✅ | `designPattern_abstractFactoryMethod.cpp` |
| 生成器（Builder） | ✅ | `designPattern_builderMethod.cpp` |
| 原型（Prototype） | ✅ | `designPattern_prototypeMethod.cpp` |
| 单例（Singleton） | ✅ | `designPattern_SingletonMethod.cpp` |

#### 结构型模式

| 模式 | 状态 | 文件 |
|---|---|---|
| 适配器（Adapter） | ✅ | `designPattern_adapterMethod.cpp` |
| 桥接（Bridge） | ✅ | `designPattern_bridgeMethod.cpp` |
| 组合（Composite） | ✅ | `designPattern_compositeMethod.cpp` |
| 装饰（Decorator） | ✅ | `designPattern_decoratorMethod.cpp` |
| 外观（Facade） | ✅ | `designPattern_facadeMethod.cpp` |
| 享元（Flyweight） | ✅ | `designPattern_flyweightMethod.cpp` |
| 代理（Proxy） | ✅ | `designPattern_proxyMethod.cpp` |

#### 行为型模式

| 模式 | 状态 | 文件 |
|---|---|---|
| 责任链（Chain of Responsibility） | ✅ | `designPattern_chainOfResponsibilityMethod.cpp` |
| 命令（Command） | ✅ | `designPattern_commandMethod.cpp` |
| 迭代器（Iterator） | ✅ | `designPattern_iteratorMethod.cpp` |
| 中介者（Mediator） | ✅ | `designPattern_MediatorMethod.cpp` |
| 备忘录（Memento） | ✅ | `designPattern_MementoMethod.cpp` |
| 观察者（Observer） | ✅ | `designPattern_observerMethod.cpp` |
| 状态（State） | ✅ | `designPattern_stateMethod.cpp` |
| 策略（Strategy） | ⬜ | - |
| 模板方法（Template Method） | ⬜ | - |
| 访问者（Visitor） | ⬜ | - |

### 使用方式（学习建议）

1. 每次选一个主题文件。
2. 单独编译、单独运行。
3. 对比相近文件（例如语言特性与 STL 用法）。
4. 在原文件上继续补充你自己的注释和实验。

### 编译示例（Windows + g++）

```bash
g++ -std=c++17 .\lambda.cpp -o .\lambda.exe
.\lambda.exe
```
