################################################################################
### Makefile template, based on the following blog:
###  http://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure
################################################################################

CC := g++
CFLAGS := -g -Wall -fpic -O3 -std=c++14

INCDIR := include
SRCDIR := src
TOOLSDIR := tools
BUILDDIR := build
PROJECT := hello

INCLUDE := /home/ainfinit/Documents/tools/boost/include
LIBRARY := /home/ainfinit/Documents/tools/boost/lib
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
TGT_SRC := $(BUILDDIR)/lib/lib$(PROJECT).so

### ${TOOLSDIR}包含所有的有main函数的cpp
SRC_TOOLS := $(shell find $(TOOLSDIR) -type f -name *.cpp)
OBJ_TOOLS := $(addprefix $(BUILDDIR)/, ${SRC_TOOLS:.cpp=.o})
TGT_TOOLS := $(addprefix $(BUILDDIR)/, ${SRC_TOOLS:.cpp=.bin})

### 所有与build相关的目录
ALL_BUILD_DIRS := $(sort $(dir $(OBJ_SRC) $(TGT_SRC) $(TGT_TOOLS)))

lib: $(TGT_SRC)

tools: $(TGT_TOOLS)

all: $(TGT_SRC) $(TGT_TOOLS)

$(TGT_SRC): $(OBJ_SRC)
	$(CC) -shared -o $@ $^ $(LIBRARY) $(LIBS)

$(TGT_TOOLS): %.bin : %.o $(OBJ_SRC)
	$(CC) -o $@ $^ $(LIBRARY) $(LIBS)

$(OBJ_SRC): $(BUILDDIR)/%.o : %.cpp $(HEADERS) | $(ALL_BUILD_DIRS)
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDE)

$(OBJ_TOOLS): $(BUILDDIR)/%.o : %.cpp $(HEADERS) | $(ALL_BUILD_DIRS)
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDE)

$(ALL_BUILD_DIRS):
	@ mkdir -p $@

clean:
	rm -rf $(BUILDDIR)

.PHONY: clean lib tools all
