## Linux下c++工程模板


### 文件安排

1. 所有的头文件位于include目录;

2. 所有的实现文件位于src目录;

3. 所有的包含main()函数的文件位于tools目录;

4. 所有单元测试文件位于unittest目录;


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
make tests  # 编译单元测试文件
make all    # 编译整个工程(包含lib和tools)
make clean  # 清空编译的文件
```


### 编译结果

所有编译产生的文件都位于build目录中, 所有可执行文件位于build/tools/目录中,
后缀名为.bin, 对应于tools目录中的cpp文件. 生成的lib文件位于: build/lib目录中.


### cmake设置

cmake设置和makefile差不多, 需要注意的是, 目前的CMakeLists.txt不支持递归搜索目录,
如果src/或者tools/中有子目录, 则需手动添加.


### 本示例中所需要的3rdparty

``` shell
sudo apt install -y libgoogle-glog-dev
sudo apt install -y libgflags-dev
sudo apt install -y libjsoncpp-dev
sudo apt install -y libboost-all-dev
sudo apt install -y libopencv-dev
sudo apt install -y libcurl4-openssl-dev
sudo apt install -y libssl-dev
```

gtest没有提供`apt install`的选项, 需要从源码安装:

``` shell
wget https://github.com/google/googletest/archive/release-1.8.1.tar.gz
tar zxvf release-1.8.1.tar.gz
cd googletest-release-1.8.1
mkdir build && cd build
cmake -DBUILD_SHARED_LIBS=on ..
make -j && sudo make install
```

date也没有提供`apt install`的选项, 需要从源码安装:

``` shell
wget https://github.com/HowardHinnant/date/archive/refs/tags/v3.0.0.tar.gz
```

### Useful tools

1. 生成`compile_commands.json`: `bear make -ir -j4 tools tests`

2. auto-formatting: `clang-format -i include/* src/* tools/* unittest/*`

3. linting: `run-clang-tidy.py`

注意: `run-clang-tidy.py`处理的是从`compile_commands.json`中的文件.


### Good Books

1. google c++ style guide

2. Effective Modern C++
