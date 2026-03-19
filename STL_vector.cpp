#include <iostream>
#include <memory>
#include <vector>
using namespace std;

// vector is a dynamic array that can resize itself automatically when elements are added or removed.
// vector 是一个动态数组，当元素被添加或删除时，它能够自动调整大小。

// test Myobject for demonstrating push_back and emplace_back
// 用于演示 push_back 和 emplace_back 的测试对象
struct MyObject 
{
    MyObject(int a, double b) { std::cout << "构造" << std::endl; }
    MyObject(const MyObject& w) { std::cout << "拷贝构造" << std::endl; }
    MyObject(MyObject&& w) noexcept { std::cout << "移动构造" << std::endl; }
};

class FileHandle 
{
public:
    // explicit shows that this constructor can't be used for implicit conversion
    // explicit表面这里不是随便一个int都能当FileHandle用，也就是不能被隐式转换
    explicit FileHandle(int fd) : fd_(fd) {}
    ~FileHandle() { if (fd_ >= 0) /*close(fd_);*/ return;}
private:
    int fd_;
};

int main()
{
    cout << "=== Vector Demo ===" << endl;
    cout << "=== Vector 详细示例 ===" << endl;
// 1. create and initialize
// 1. 创建和初始化
    vector<int> vec1 = {1, 2, 3, 4, 5};

    // five elements with value 10
    // 5个10
    vector<int> vec2(5, 10);

    // use vec1's elements to initialize
    // 用vec1初始化
    vector<int> vec3(vec1.begin(), vec1.end());

    // void vector;
    // 空vector
    vector<int> vec4;

// 2. capacity and size
// 2. 容量管理
    cout << "vec1.size(): " << vec1.size() << ", vec1.capacity(): " << vec1.capacity() << endl;
    // preallocate space, reduce memory allocation times
    // 预分配空间, 减少内存分配次数
    vec1.reserve(20);

    cout << "after reserve capacity: " << vec1.capacity() << endl;

// 3. element access
// 3. 元素访问
    // use index or at(), use front() and back() to get the first and last element
    // when use at(), it will do boundary check
    // 用下标或at()访问, 用front()和back()获取第一个和最后一个元素
    // 其中用at()访问会做越界检查
    cout << "the first element: "<< vec1.front() << endl;
    cout << "the last element: " << vec1.back() << endl;
    cout << "the 3rd element: " << vec1[2] << endl;
    cout << "the 3rd element(safe): " << vec1.at(2) << endl;

// 4. insert operation
// 4. 插入操作
    vec1.push_back(6);
    vec1.insert(vec1.begin() + 2, 100);
    // insert 3 elements at the beginning
    // 在开头插入3个0
    vec1.insert(vec1.begin(), 3, 0);

    cout << "after insert: ";
    for (const auto &elem : vec1)
        cout << elem << " ";
    cout << endl;

// 5. delete operation
// 5. 删除操作
    vec1.pop_back();
    vec1.erase(vec1.begin() + 2);
    // delete the first 3 elements
    // 删除前3个元素
    vec1.erase(vec1.begin(), vec1.begin() + 3);

    cout << "after erase: ";
    for (const auto &elem : vec1)
        cout << elem << " ";
    cout << endl;

// 6. resize operation
// 6. 调整大小
    // resize to 10 elements, new elements are filled with 999
    // 调整到10个元素，新元素用999填充
    vec1.resize(10, 999);
    cout << "after resize: ";
    for (const auto &elem : vec1)
        cout << elem << " ";
    cout << endl;

// 7. clear and swap
// 7. 清空和交换
    vector<int> temp = {100, 200, 300};
    vec1.swap(temp);
    cout << "after swap: ";
    for (const auto &elem : vec1)
        cout << elem << " ";
    cout << endl;

// note 1: push_back and emplace_back difference, push_back will first construct a temporary object then move/copy it into the vector, while emplace_back constructs the object directly in the vector's memory, avoiding unnecessary move/copy.
// 注意 1:push_back和emplace_back的区别，前者会先构造一个临时对象再移动/拷贝到vector中，后者直接在vector内存上构造对象，避免了不必要的移动/拷贝。

// push_back have two overloads, one takes const T& and the other takes T&&. No matter which one is called, the passed-in argument is already a fully constructed object, and internally it just copies or moves it to the end of the container.
// emplace_back only has one overload, it takes a bunch of parameters, and internally it
// directly calls the T constructor to construct the object and put it at the end of the container.
// push_back有两个重载，一个接const T&，一个接T&&。不管走哪个，传进来的已经是一个造好的对象了，内部只是把它拷贝或移动到容器末尾。
// emplace_back只有一个重载，传进来的是一堆参数，内部直接调用T的构造函数构造对象，然后放到容器末尾。
// 简单来所push_back是"你造好给我，我搬进去"; emplace_back是"你告诉我怎么造，我直接在目的地造"。

#if __cplusplus >= 201102L
    std::cout << "Compiler mode is C++11 or later.\n";
    std::cout << "---------------------\n";

// (1) emplace_back base use
// (1) emplace_back 基本用法
    std::vector<MyObject> vec;
    // reserve space to avoid reallocation
    // 预留空间，排除扩容干扰
    vec.reserve(10);

    std::cout << "push_back demo:\n";
    // push_back path: constructs a temporary Widget, then moves it into the vector
    // output: construct → move construct
    // push_back的路径：先在栈上构造临时Widget，再移动到容器末尾
    // 输出：构造 → 移动构造
    vec.push_back(MyObject(1, 3.14));

    std::cout << "emplace_back demo:\n";
    // emplace_back path: constructs the Widget directly in place at the end of the vector's memory, no temporary object is created
    // output: construct
    // emplace_back的路径：直接在容器末尾的内存上调用Widget(1, 3.14)，没有临时对象
    // 输出：构造
    vec.emplace_back(1, 3.14);
    std::vector<FileHandle> handles;

    std::cout << "---------------------\n";
// (2) notice: emplace_back will bypass explicit, the compiler will no longer help you check
// (2) 注意: emplace_back会绕过explicit，编译器不再帮你把关
    // error: no implicit conversion from int to FileHandle, push_back requires an object of type FileHandle or something that can be implicitly converted to it, but 42 is just an int, so this will cause a compilation error.
    // 错误：没有从int到FileHandle的隐式转换，push_back
    // handles.push_back(42);

    // conpiles fine, but it will create a temporary FileHandle object from 42, then move it into the vector, which is less efficient than emplace_back that constructs the FileHandle directly in place.
    // 显式构造，编译通过
    handles.push_back(FileHandle(42));

    // conpiles fine, and it don't have any warning!
    // 编译通过，没有任何警告！
    handles.emplace_back(42); 

// (3) naked pointer + emplace_back = memory leak time bomb
// (3) 裸指针 + emplace_back = 内存泄漏定时炸弹
    std::vector<std::shared_ptr<MyObject>> ptrVec;

    // 写法A：安全
    ptrVec.push_back(std::make_shared<MyObject>(1, 3.14));

    // 写法B：有泄漏风险
    // 原因：参数会先求值，new MyObject() 先返回裸指针；
    // 若随后在扩容或shared_ptr控制块分配阶段抛异常，
    // shared_ptr还没来得及接管该裸指针，就会泄漏。
    // 结论：这里应优先使用 make_shared/make_unique + push_back。
    // vec.emplace_back(new MyObject());

// (4) emplace_back with initializer list
// (4) 花括号初始化列表，emplace_back接不住

    // error: no matching function for call to 'emplace_back', emplace_back doesn't have an overload that takes an initializer list, so this will cause a compilation error.
    // 错误：没有匹配的函数调用，emplace_back没有接initializer list的重载，所以编译错误。
    // vec.emplace_back({1, 3.14});

    // but push_back can take an initializer list
    // 但是push_back可以接initializer list
    vec.push_back({1, 3.14});

    // we also can use emplace_back with initializer list, but we need to explicitly construct an initializer_list object, which is a bit verbose and not very elegant.
    // 也可以用emplace_back接initializer list，但需要显式构造一个
    std::vector<std::vector<int>> matrix;

    // Method 1：explicitly construct an initializer_list
    // 方法1：显式构造一个initializer_list对象
    matrix.emplace_back(std::initializer_list<int>{1, 2, 3});

    // Method 2：construct a temporary vector, then move it into the container, which is less efficient than directly constructing in place, 
    // 方法2：先构造一个临时vector，再移动到容器中，比直接构造在容器中效率低，但比显式构造initializer_list简洁。
    matrix.emplace_back(std::vector<int>{1, 2, 3});
    // but it is stupid, because it becomes push_back, so why not just use push_back?
    // 但这很蠢，因为它变成了push_back, 那为什么不直接用push_back呢？

    // ------------------------------------------------------------
    // push_back vs emplace_back Quick Reference
    // +------------------------------------------------------+---------------------------------+--------------------------------------------------------+
    // | Scenario                                             | Recommended                     | Why                                                    |
    // +------------------------------------------------------+---------------------------------+--------------------------------------------------------+
    // | Passing constructor args (no explicit constraint)    | emplace_back                    | In-place construction; often avoids one temp/move      |
    // | Passing an already-constructed object                | Either                          | Usually equivalent in effect; choose by readability    |
    // | Type has explicit constructor (keep type boundary)   | push_back                       | Preserves explicit-construction constraints            |
    // | Involves new/raw pointer                             | push_back + make_shared/unique  | Exception-safe; avoids leaks before ownership transfer |
    // | Brace-init / initializer_list                        | push_back                       | Simpler overload resolution; emplace often won't deduce|
    // +------------------------------------------------------+---------------------------------+--------------------------------------------------------+

    // ------------------------------------------------------------
    // push_back vs emplace_back 速查表
    // +----------------------------------------------+---------------------------------+--------------------------------------------+
    // | 场景                                         | 推荐                            | 原因                                        |
    // +----------------------------------------------+---------------------------------+--------------------------------------------+
    // | 传构造参数（不涉及 explicit 限制）             | emplace_back                    | 原地构造，通常可省一次临时对象构造/移动       |
    // | 传已构造好的对象                              | 都行                            | 语义和效果基本等价，按可读性选择              |
    // | 类有 explicit 构造函数（要保留类型边界）       | push_back                       | 保留显式构造约束，避免参数直通构造            |
    // | 涉及 new / 裸指针                            | push_back + make_shared/unique  | 异常安全，避免接管前泄漏                     |
    // | 花括号初始化列表（initializer_list）          | push_back                       | 重载匹配更直接，emplace_back 常接不住 { ... }|
    // +----------------------------------------------+---------------------------------+--------------------------------------------+
#endif

    return 0;
}