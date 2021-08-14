#! /usr/bin/env python
# coding: utf-8
#
# pylint: disable=all
#
"""clang-format加强版.

clang-format本身不支持单行(单语句)设置, 这给实际使用带来了一些麻烦. 比如
只想忽略某一行的时候, 也需要添加clang-format保护对:

``` cpp
// clang-format off
int no_format_this_line = 0;
// clang-format on
```

这里我们给出一个更方便的解决方案: NOFORMAT(s1:e1, s2:e2, ...).
表示忽略行数为s1~e1的代码, 忽略s2~e2的代码, ..., 以此类推.
其中, s1, e1, s2, e2等等都是相对于当前行. 这里的范围包括边界, 也就是说,
s1, e1, s2, e2本身也会被忽略.

``` cpp
// 单纯的NOFORMAT表示忽略当前行.
int no_format_this_line = 0;  // NOFORMAT
// NOFORMAT带一个数字表示忽略该数字指定的行, 所以下面语句也表示忽略上面一行
// NOFORMAT(-2)

// 忽略下面这个代码块
if (true) {
  int hello = 1;
} // NOFORMAT(-2:)

// NOFORMAT(1:3), 这里也是忽略下面的代码块
while (true) {
  int world = 0;
}
```
"""

import re
import os
import argparse


def _parse_format_string(line_id, line_content):
    line_ranges = []
    for item in line_content.split(","):
        if ":" not in item:
            line_ranges.append((int(item), int(item)))
        elif item.startswith(":"):
            line_ranges.append((0, int(item[1:])))
        elif item.endswith(":"):
            line_ranges.append((int(item[:-1]), 0))
        else:
            start, end = item.split(":")
            line_ranges.append((int(start), int(end)))

    normed_ranges = []
    for start, end in line_ranges:
        assert start <= end, f"Wrong format: {line_content}"
        normed_ranges.append((line_id + start, line_id + end))
    return normed_ranges


def _get_no_format_ranges(lines, flag):
    # 这里的range从0开始, 且包含两个端点
    no_format_ranges = []
    pattern = re.compile(r"//{}\((.+)\)".format(flag))
    for i, line in enumerate(lines):
        line = line.strip().replace(" ", "")
        match = pattern.search(line)
        if match:
            ranges = _parse_format_string(i, match.group(1))
            no_format_ranges.extend(ranges)
        elif f"//{flag}" in line:
            no_format_ranges.append((i, i))
    # 不允许每一个range之间有overlap
    no_format_ranges.sort(key=lambda x: x[0])
    for i in range(len(no_format_ranges) - 1):
        assert no_format_ranges[i][1] < no_format_ranges[i + 1][0]
    return no_format_ranges


def _generate_modified_code(lines, no_format_ranges, save_file):
    curr_loc = 0
    new_lines = []
    for start, end in no_format_ranges:
        new_lines.extend(lines[curr_loc:start])
        head = " " * (len(lines[start]) - len(lines[start].lstrip()))
        new_lines.append(f"{head}// clang-format off\n")
        new_lines.extend(lines[start:end + 1])
        head = " " * (len(lines[end]) - len(lines[end].lstrip()))
        new_lines.append(f"{head}// clang-format on\n")
        curr_loc = end + 1
    new_lines.extend(lines[curr_loc:])
    with open(save_file, "w") as dstfile:
        dstfile.writelines(new_lines)


def _format_cpp_file(file_path, file_length, no_format_ranges):
    curr_loc = 0
    line_ranges = []
    for start, end in no_format_ranges:
        if curr_loc < start:
            # 注意: clang-format的range从1开始
            line_ranges.append(f"--lines {curr_loc+1}:{start}")
        curr_loc = end + 1
    if curr_loc < file_length - 1:
        line_ranges.append(f"--lines {curr_loc+1}:{file_length}")
    command = "clang-format -i {} --sort-includes --style=file {}"
    command = command.format(" ".join(line_ranges), file_path)
    print(command)
    os.system(command)


def run_clang_format(args):
    for file in args.file:
        assert os.path.exists(file), f"File not found: {file}"
        # 注意: 这里的lines带了前后的空格字符, 结尾带了换行符
        with open(file, "r") as srcfile: lines = [l for l in srcfile]
        ranges = _get_no_format_ranges(lines, args.flag)
        dirname, filename = os.path.split(file)
        # 4cf的意思: for clang-format
        path = os.path.join(dirname, f".{filename}.4cf")
        _generate_modified_code(lines, ranges, path)
        _format_cpp_file(file, len(lines), ranges)


def main():
    parser = argparse.ArgumentParser(
        description="Customized clang-format to auto-format c++ files.")

    parser.add_argument(
        "file",
        type=str,
        nargs="+",
        help="Path of c++ file to be auto-formatted.",
    )
    parser.add_argument(
        "--flag",
        type=str,
        default="NOFORMAT",
        help="Flag to disable clang-format.",
    )
    args = parser.parse_args()
    run_clang_format(args)
    print("Done!")


if __name__ == "__main__":
    main()
