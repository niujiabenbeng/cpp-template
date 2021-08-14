#! /bin/bash

# 删除clang-format和clang-tidy生成的临时文件.

for path in $@; do
    find ${path} -type f -name '*.4cf' -print -exec rm {} \;
    find ${path} -type f -name '*.4ct' -print -exec rm {} \;
done
