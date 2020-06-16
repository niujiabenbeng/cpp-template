################################################################################
### CMakeLists.txt template
################################################################################

cmake_minimum_required(VERSION 3.0.2)

### 这里修改project的名字
project(hello)

### 这里选择用clang还是gcc
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-g -Wall -fpic -O3 -std=c++14")

set(INCDIR   ${PROJECT_SOURCE_DIR}/include)
set(SRCDIR   ${PROJECT_SOURCE_DIR}/src)
set(TOOLSDIR ${PROJECT_SOURCE_DIR}/tools)

### 这里添加第三方库
set(INCLUDE /home/ainfinit/Documents/tools/boost/include)
set(LIBRARY /home/ainfinit/Documents/tools/boost/lib)
set(LIBS dl m z rt glog)
include_directories(SYSTEM ${INCLUDE})
include_directories(${INCDIR})
link_directories(${LIBRARY})
link_libraries(${LIBS})

### cmake递归查找命令(GLOB_RECURSE)有bug, 这里采用非递归方式. 如果有${SRCDIR}或
### 者${TOOLSDIR}中有子目录, 手动在这里添加.
file(GLOB SRC_SRC ${SRCDIR}/*.cpp)
set(SRC_FILES ${SRC_SRC})
file(GLOB SRC_TOOLS ${TOOLSDIR}/*.cpp)
set(TOOLS_FILES ${SRC_TOOLS})

### 生成动态库, 位于: lib/lib${PROJECT_NAME}.so
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
set_target_properties(${PROJECT_NAME}
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY "lib")

### 生成可执行文件
foreach(TOOL ${TOOLS_FILES})
  file(RELATIVE_PATH RELPATH ${PROJECT_SOURCE_DIR} ${TOOL})
  get_filename_component(BASENAME ${RELPATH} NAME)
  get_filename_component(DIRNAME  ${RELPATH} DIRECTORY)
  string(REGEX REPLACE "cpp$" "bin" BINNAME ${BASENAME})
  add_executable(${BINNAME} ${SRC_FILES} ${TOOL})
  set_target_properties(${BINNAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${DIRNAME})
endforeach(TOOL)
