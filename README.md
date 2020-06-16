## Linux下c++工程模板


### 文件安排

1. 所有的头文件位于include目录;

2. 所有的实现文件位于src目录;

3. 所有的包含main()函数的文件位于tools目录;


### 第三方库设置 (修改Makefile文件)

* 头文件目录: 在 INCLUDE 后面添加, 每一项以空格隔开;

* 库文件目录: 在 LIBRARY 后面添加, 每一项以空格隔开;

* 库文件: 在 LIBS 后面添加, 每一项以空格隔开. 注意要去掉前面的lib和后面的后缀.

```Makefile
# 第三方库设置实例:
INCLUDE := /home/chenli/Documents/tools/opencv/include
LIBRARY := /home/chenli/Documents/tools/opencv/lib
# 这里glog对应的文件为: libglog.so, opencv_core对应的文件为: libopencv_core.so
LIBS := dl m z rt glog opencv_core
```


### 修改project的名字(修改Makefile文件)

可以将project修改成任意的名字

```
PROJECT := hello_or_whatever_you_like
```


### Make 命令

```shell
make        # 编译lib文件
make lib    # 编译lib文件
make tools  # 编译可执行文件
make all    # 编译整个工程(包含lib和tools)
make clean  # 清空编译的文件
```


### 编译结果

所有编译产生的文件都位于build目录中, 所有可执行文件位于build/tools/目录中,
后缀名为.bin, 对应于tools目录中的cpp文件. 生成的lib文件位于: build/lib目录中.


### clang-format & clang_complete

clang-format 配置文件基于 clang-format v 6.0.


### compile_commands.json

生成compile_commands.json的命令

``` shell
### make
sudo apt install bear
bear make

### cmake
mkdir build && cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
```


### cmake设置

cmake设置和makefile差不多, 需要注意的是, 目前的CMakeLists.txt不支持递归搜索目录,
如果src/或者tools/中有子目录, 则需手动添加.
