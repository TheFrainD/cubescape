#!/bin/bash

# Check if an argument is provided
if [ -z "$1" ]; then
    echo "Error: No directory specified."
    echo "Usage: $0 <directory>"
    exit 1
fi

# Set the directory
DIR=$1

# Find all C, C++, and Objective-C source files and format them
find "$DIR" \( \
        -name "*.c" -o \
        -name "*.h" -o \
        -name "*.cpp" -o \
        -name "*.hpp" -o \
        -name "*.cc" -o \
        -name "*.hh" -o \
        -name "*.m" -o \
        -name "*.mm" \
\) -exec clang-format -i {} +

echo "clang-format applied recursively in $DIR"
