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


### cmake设置

cmake设置和makefile差不多, 需要注意的是, 目前的CMakeLists.txt不支持递归搜索目录,
如果src/或者tools/中有子目录, 则需手动添加.


--------------------------------------------------------------------------------
--------------------------- 以下属于个人编程环境配置 ---------------------------
--------------------------------------------------------------------------------

### clangd (c++ language server)

``` shell
## get srouce code: https://github.com/llvm/llvm-project/releases
mkdir build && cd build
cmake -G "Unix Makefiles" \
      -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;libcxx;libcxxabi;libunwind;lldb;compiler-rt;lld;polly" \
      -DCMAKE_INSTALL_PREFIX=${HOME}/Documents/tools/clang \
      -DCMAKE_BUILD_TYPE=Release \
      ../llvm
make -j32 && make install

## 配置环境变量
export LD_LIBRARY_PATH=${HOME}/Documents/tools/clang/lib:${LD_LIBRARY_PATH}
export PATH=${HOME}/Documents/tools/clang/bin:${PATH}
```

##### 跳转和补全

clangd需要compile_commands.json来解析源码.

生成compile_commands.json的命令

``` shell
### make
sudo apt install bear
bear make all

### cmake
mkdir build && cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
```

不管是采用bear还是cmake方式, 每添加一个源文件, 都需要重新生成
compile_commands.json.

compile_commands.json纪录了每一个cpp文件的编译选项. 若所有的cpp文件的编译选项都
一致, 则可以用compile_flags.txt, 以下是一个示例:

``` text
-g
-xc++
-std=c++14
-Iinclude
-I3rdparty/boost/include
-I3rdparty/opencv/include
-I~/Documents/tools/boost/include
-I~/Documents/tools/opencv/include
```

其中, `-xc++`选项将所有的`.h`文件按`c++`头文件处理, 若没有这个选项, 则clangd会认
为`.h`文件为`c`的头文件, 其中使用`c++`的语法会报错.

注意:

1. `-I`和包含路径之间不能有空格, 否则clangd无法识别(不报错, 但无法正常工作).

2. 这里为了方便, 包含了多个`opencv`和`boost`的目录, 如果产生冲突, 则需要删掉多余的.

3. helm-lsp中, 默认开启了模糊匹配, 为了避免补全选项过多过杂, 需要关闭模糊匹配:
   `:fuzzy-match nil`


##### clang-format

clang-format 配置文件基于 clang-format v10.0.


### .neoignore

用neotree作为emacs的侧边栏时, 可以设置`.neoignore`来决定哪些文件需要被隐藏.
