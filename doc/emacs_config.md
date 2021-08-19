# emacs中c++开发环境搭建


### clangd (c++ language server)

编译好的clang库可以从如下位置找到: https://github.com/llvm/llvm-project/releases

如果没有合适的二进制库, 也可以自己从源码编译:

``` shell
mkdir build && cd build
cmake -G "Unix Makefiles" \
      -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;libcxx;libcxxabi;libunwind;lldb;compiler-rt;lld;polly" \
      -DCMAKE_INSTALL_PREFIX=${HOME}/Documents/tools/clang \
      -DCMAKE_BUILD_TYPE=Release \
      ../llvm
make -j32 && make install

# 配置环境变量
export LD_LIBRARY_PATH=${HOME}/Documents/tools/clang/lib:${LD_LIBRARY_PATH}
export PATH=${HOME}/Documents/tools/clang/bin:${PATH}
```

### 跳转和补全

clangd需要compile_commands.json来解析源码.

生成compile_commands.json的命令

``` shell
# make
sudo apt install bear
bear make -ik all -j8

# cmake
mkdir build && cd build && cmake ..
```

不管是采用bear还是cmake方式, 每添加一个源文件, 都需要重新生成
compile_commands.json.

compile_commands.json纪录了每一个cpp文件的编译选项. 若所有的cpp文件的编译选项都
一致, 则可以用compile_flags.txt, 以下是一个示例:

``` text
-g
-xc++
-std=c++17
-Iinclude
-I3rdparty/boost/include
-I3rdparty/opencv/include
-I/home/chenli/Documents/tools/boost/include
-I/home/chenli/Documents/tools/opencv/include
 ```

其中, `-xc++`选项将所有的`.h`文件按`c++`头文件处理, 若没有这个选项, 则clangd会认
为`.h`文件为`c`的头文件, 其中使用`c++`的语法会报错.

注意:

1. `-I`和包含路径之间不能有空格, 否则clangd无法识别(不报错, 但无法正常工作).

2. 这里为了方便, 包含了多个`opencv`和`boost`的目录, 如果产生冲突, 则需要删掉多余的.


### 自动排版

自动排版用: clang-format, 配置文件位于: `.clang-format`. 原生的clang-format没有
设定单行的选项, 对此我进行了扩展, 工具位于: `doc/clang-format.py`

对整个工程进行自动排版可以用如下命令:

`./doc/clang-tools.sh format`


### linter

linter为clang-tidy, 配置文件为: `.clang-tidy`. 原生的clang-tidy没有设定文件
级别的选项, 对此我进行了扩展, 工具位于: `doc/clang-tidy.py`

clang-tidy也需要`compile_commands.json`. 对整个工程进行检查可以用如下命令:

`./doc/clang-tools.sh lint`


### .neoignore

用neotree作为emacs的侧边栏时, 可以设置`.neoignore`来决定哪些文件需要被隐藏.
