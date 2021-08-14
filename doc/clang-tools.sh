#! /bin/bash

curr_dir=$(dirname $0)
proj_dir=${curr_dir}/..
format_tool=${curr_dir}/clang-format.py
lint_tool=${curr_dir}/clang-tidy.py

source_files="
include/blocking_queue.h
include/common.h
include/thread_pool.h
include/timer.h
include/util.h
src/util.cpp
tools/main.cpp
unittests/unittest.cpp
"

if [ $# != 1 ]; then
    echo "usage: clang-tools.sh format|lint|clean" && exit 0
fi

if [ $1 == "format" ]; then
    cd ${proj_dir} && ${format_tool} ${source_files}
elif [ $1 == "lint" ]; then
    cd ${proj_dir} && ${lint_tool} ${source_files}
elif [ $1 == "clean" ]; then
    find ${proj_dir} -type f -name '*.4cf' -print -exec rm {} \;
    find ${proj_dir} -type f -name '*.4ct' -print -exec rm {} \;
else
    echo "Unknown task $1"
fi
