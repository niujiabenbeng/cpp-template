################################################################################
### Makefile template, based on the following blog:
###  http://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure
################################################################################

################################################################################
### Makefile中的文件依赖, 参考:
###   https://stackoverflow.com/questions/2394609/makefile-header-dependencies
###
### 编译选项'-MMD'自动生成.d文件, 与.o文件(目标文件)位于同一目录. 比如:
###    源文件:             src/hello.cpp
###    它依赖的头文件:     include/hello.h
###    编译的目标文件:     build/src/hello.o
###
### 编译命令:
###   $(CC) $(CFLAGS) -MMD -c -o build/src/hello.o src/hello.cpp $(INCLUDE)
###
### 则其生成头文件依赖:    build/src/hello.d
### 其内容为:              build/src/hello.o: src/hello.cpp include/hello.h
### 这实际上是一个编译目标. 可以通过include指令导入.
###
### 这里有一个问题: 同一个编译目标被指定了两次: 一个在本Makefile中, 一个在.d文件
### 中, 用include指令导入. 实际上, 如果有多个依赖指向同一个编译目标, 这些依赖将
### 会被合并.
################################################################################

CC := g++
CFLAGS := -g -Wall -fpic -O3 -std=c++14

INCDIR := include
SRCDIR := src
TOOLSDIR := tools
BUILDDIR := build
PROJECT := hello

### 这里添加第三方库
INCLUDE := ./3rdparty/boost/include
LIBRARY := ./3rdparty/boost/lib
LIBS := dl m z rt glog

INCLUDE := $(foreach INC, $(INCLUDE), -isystem $(INC))
LIBRARY := $(foreach LIB, $(LIBRARY), -L $(LIB))
LIBS := $(foreach LIB, $(LIBS), -l$(LIB))
INCLUDE += -I $(INCDIR)

### all header files
HEADERS := $(shell find $(INCDIR) -type f -name *.h)

### $(SRCDIR)包含所有的库cpp
SRC_SRC := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJ_SRC := $(addprefix $(BUILDDIR)/, ${SRC_SRC:.cpp=.o})
DEP_SRC := $(addprefix $(BUILDDIR)/, ${SRC_SRC:.cpp=.d})
TGT_SRC := $(BUILDDIR)/lib/lib$(PROJECT).so

### ${TOOLSDIR}包含所有的有main函数的cpp
SRC_TOOLS := $(shell find $(TOOLSDIR) -type f -name *.cpp)
OBJ_TOOLS := $(addprefix $(BUILDDIR)/, ${SRC_TOOLS:.cpp=.o})
DEP_TOOLS := $(addprefix $(BUILDDIR)/, ${SRC_TOOLS:.cpp=.d})
TGT_TOOLS := $(addprefix $(BUILDDIR)/, ${SRC_TOOLS:.cpp=.bin})

### 提前建好所有与build相关的目录
ALL_BUILD_DIRS := $(sort $(dir $(OBJ_SRC) $(TGT_SRC) $(TGT_TOOLS)))
ALL_BUILD_DIRS := $(shell mkdir -p $(ALL_BUILD_DIRS))

lib: $(TGT_SRC)

tools: $(TGT_TOOLS)

all: $(TGT_SRC) $(TGT_TOOLS)

$(TGT_SRC): $(OBJ_SRC)
	$(CC) -shared -o $@ $^ $(LIBRARY) $(LIBS)

$(TGT_TOOLS): %.bin : %.o $(OBJ_SRC)
	$(CC) -o $@ $^ $(LIBRARY) $(LIBS)

$(OBJ_SRC): $(BUILDDIR)/%.o : %.cpp
	$(CC) $(CFLAGS) -MMD -c -o $@ $< $(INCLUDE)

$(OBJ_TOOLS): $(BUILDDIR)/%.o : %.cpp
	$(CC) $(CFLAGS) -MMD -c -o $@ $< $(INCLUDE)

ifneq ($(filter clean, $(MAKECMDGOALS)), clean)
    ### 这里'-'表示忽略错误.
    ### 缺省状态下只编译lib, 这里把tools的头文件依赖也包含进去虽然略显多余,
    ### 但是由于.d文件只包含依赖, 并没有实际的编译指令, 所以并不耗费时间.
    -include $(DEP_SRC)
    -include $(DEP_TOOLS)
endif

clean:
	rm -rf $(BUILDDIR)

.PHONY: clean lib tools all
