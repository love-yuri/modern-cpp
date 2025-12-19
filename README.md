# modern-cpp

现代C++学习项目，研究现代化的C++特性。全面使用C++ Modules进行构建，探索C++23的新功能。

## 项目要求

### 最低版本要求

- **CMake**: >= 4.0.0
    - 推荐版本: 4.0.2 或 4.2.0 (std标志支持)
- **C++标准**: C++23
- **编译器**:
    - MSVC >= 19.38 (Visual Studio 2022 17.8+)
    - GCC >= 15.0
    - Clang >= 19.0

> ⚠️ **注意**: C++ Modules 支持需要较新的编译器版本，请确保编译器完全支持C++23模块特性。

### Windows
windows下使用clang构建请链接libc++, 使用msvc可以直接进入vs终端构建

## 目录结构

```
modern-cpp/
├── cmake/                      # CMake 配置文件
├── src/                        # 源代码目录
│   ├── reflection/             # 反射模块
│   └── utils/                  # 工具模块
├── test/                       # 测试目录
├── CMakeLists.txt              # 主CMake配置
```

## 快速开始

```bash

# 克隆项目
git clone https://github.com/love-yuri/modern-cpp.git
cd modern-cpp
mkdir build
cd build
cmake .. -G Ninja # 只能使用Ninja构建
cmake --build .   # 生成
ctest             # 启动测试，也可以单独运行`test`目录下的测试
```

## 项目特性

- ✅ 完全使用 C++23 标准
- ✅ 基于 C++ Modules (.ixx) 构建
- ✅ 编译时反射实现
- ✅ 现代化的 CMake 配置
- ✅ 模块化的项目结构
- ✅ 完整的测试支持

## API 说明
### 日志模块

提供格式化日志输出功能，支持多种日志级别。
**使用示例:**
```cpp
yuri::info("程序启动，版本: {}", "1.0.0");
yuri::error("文件未找到: {}", filename);
yuri::warn("内存使用率: {}%", usage);
```
---

### 反射模块
提供编译时结构体反射功能，支持获取成员数量和成员名称。
**使用示例:**
```cpp
struct Person {
    std::string name;
    int age;
    double salary;
};

// 获取成员数量
constexpr auto count = yuri::reflection::count_members<Person>; // 3

// 获取成员名称
constexpr auto names = yuri::reflection::get_names<Person>;
// names[0] = "name", names[1] = "age", names[2] = "salary"

// 获取单个成员名称
constexpr auto first_name = yuri::reflection::get_name<0, Person>; // "name"
```

**要求:**
- 类型 `T` 必须是聚合类型 (`std::is_aggregate_v<T>`)
- 类型 `T` 成员不能包含数组(`int a[5]`)

---

## 许可证

详见 [LICENSE](LICENSE) 文件。
