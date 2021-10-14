# google编程规范

google编程规范着眼于大项目, 团队合作, 这样会导致写代码和读代码不是同一个人, 所以
google编程规范强调代码的可读性和注释的完整性.

本人所写的代码基本上是自己维护, 代码量也比较小, 所以可以牺牲一些可读性来保持代码
的整洁. 这里列出自己的代码规范与google编程规范不一致的地方, 并加以说明.


### Header Files

规则: In general, every .cc file should have an associated .h file. There are
some common exceptions, such as unit tests and small .cc files containing just a
main() function.

选择: 大多数情况下遵守, 除了一些接口简单但实现起来比较复杂的函数. 比如:

``` c++
cv::Mat DecodeJPEG(const std::string& content, cv::Rect* roi, int scale);
```

这个函数用turbo-jpeg解码部分图像, 实现起来比较复杂. 如果将这个函数单独放一个头文
件, 则显得比较冗余; 反之, 如果将函数的实现并入到其他的cpp中, 则该cpp文件会显得冗
长. 实际项目中, 我会将这个函数的申明放到`util.h`中, 然后将实现放到`codec.cpp`中.


### Include What You Use

规则: If a source or header file refers to a symbol defined elsewhere, the file
should directly include a header file which properly intends to provide a
declaration or definition of that symbol. It should not include header files for
any other reason.

选择: 所有情况下遵守, 除了`common.h`.

一般情况下, 我会把整个项目普遍需要的宏定义和头文件放到`common.h`中, 项目中其他文
件只需要包含`common.h`即可. 这样虽然会引入一些不必要的包含, 进而会影响编译速度,
但是减小了一些重复的代码, 也比较直观.


### Forward Declarations

规则: Try to avoid forward declarations of entities defined in another project.

选择: 所有情况下遵守. 这里将规则进一步强化: 前置申明和实现必须位于同一个文件之内.


### Inline Functions

规则: Define functions inline only when they are small, say, 10 lines or fewer.

选择: 所有情况下遵守. 但是, 对于非频繁调用的函数, 是否inline关系不大.


### Static and Global Variables

规则: Objects with static storage duration are forbidden unless they are
trivially destructible. Informally this means that the destructor does not do
anything, even taking member and base destructors into account. More formally it
means that the type has no user-defined or virtual destructor and that all bases
and non-static members are trivially destructible.

选择: 不遵守这条规则. 同时, 应尽可能减少全局变量和static变量的使用, 如果必须使用
全局变量或者static变量, 则不对用法加以限制.


### Doing Work in Constructors

规则: Avoid virtual method calls in constructors, and avoid initialization that
can fail if you can't signal an error.

选择: 不遵守这条规则. 如果一个类有可能初始化失败, 可以在该类中添加一个状态变量来
表示初始化是否成功, 并且添加一个函数返回该类的状态, 例如:

``` c++
class ObjectWithFlag {
 public:
  ObjectWithFlag() {
    if (error) { return; }
    initialized_ = true;
  }

  bool is_initialized() const { return initialized_; }

 private:
  bool initialized_{false};
};
```


### Inheritance

规则: Composition is often more appropriate than inheritance. When using
inheritance, make it public.

Limit the use of protected to those member functions that might need to be
accessed from subclasses. Note that data members should be private.

选择: 按照规则, 所有数据成员都必须为私有, 则丧失了一部分继承的意义, 也会增加程序
的复杂性. 所以, 这里数据成员可以为protected.


### Operator Overloading

规则: Overload operators judiciously. Do not use user-defined literals.

选择: 除了比较运算符之外禁止其他操作符重载. 所有情况用函数表示.


### Casting

规则: In general, do not use C-style casts. Instead, use these C++-style casts
when explicit type conversion is necessary.

选择: 不对数据类型转换方法进行限制, 我只是觉得c++的cast比较麻烦.


### Streams

规则: Use streams only when they are the best tool for the job. This is
typically the case when the I/O is ad-hoc, local, human-readable, and targeted
at other developers rather than end-users.

选择: 基本不用streams, 觉得太麻烦. 用boost::format替代. 一个类要输出信息也不重载
operator<<, 用成员函数替代, 比如: ToString(), string().


### Preprocessor Macros

规则: Avoid defining macros, especially in headers; prefer inline functions,
enums, and const variables. Name macros with a project-specific prefix. Do not
use macros to define pieces of a C++ API.

选择: 少用Macros, 而不是不用.


### Lambda Expressions

规则: Use lambda expressions where appropriate. Prefer explicit captures when
the lambda will escape the current scope.

选择: 所有情况下遵守. 注意这里的要求: 如果lambda函数在当前范围之外使用, 则需要显
式指定capture列表, 如果lambda函数在当前范围之内使用, 则不用显式指定capture列表.


### General Naming Rules

* 作用域越小变量名可以越简略, 作用域越大变量名必须越清楚.

* 可以用缩写, 但缩写必须是自己常用的. (google编程规范禁止缩写)

* 常用的缩略语保持原样, 比如: ForwardGPU (goolge编程规范为: ForwardGpu)


### Naming Rules

这里仅列出不遵守规范的命名方式.

* 文件名: my_class.h, my_class.cpp, test_my_class.cpp

* 全局常量名: kGlobalConstant

* 全局变量名: global_variable_, g_global_variable_

* 局部常量名: local_const_variable, local_constexpr_variable

* 局部static变量: local_static_variable

* 枚举量名: ENUM_NAME


### Comments

这里只考虑两种注释:

1. 辅助阅读. 其目的是使代码的逻辑结构一目了然, 也有助于快速查找和定位代码. 这种
   注释通常位于较长的函数实现代码中.

2. 在关键地方纪录实现逻辑和原因. 这样有助于自己以后阅读代码时了解当时写代码时的
   想法.
