#! /usr/bin/env python
# coding: utf-8

# pylint: disable=all

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
        with open(file, "r") as srcfile:
            lines = [l for l in srcfile]
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
