#!/bin/bash

# Simple build script template for small projects

echo "Starting compilation..."

# @@@@@@@@@@@@@@@@@@@@@@@@@@@@ Edit the following to your liking @@@@@@@@@@@@@@@@@@@@@@@@@@@@

# Define compiler
COMPILER=gcc

# Output file name (Linux executables usually have no extension)
OUTPUT_FILE_NAME=main

# Define resource files to include
RES_FILES=""

# Define the source files to include
SRC_FILES="main.c"

# Define additional compiler flags to use
CFLAGS=""

# Define additional linker flags to use (static linking)
LDFLAGS="-L../tinygl/internal/lib/linux/x64 -l:libfreeglut_static.a -l:libGL.a -l:libGLU.a -lm -lpthread -ldl"

# Set the C standard
CSTD="-std=c99"

# Release build optimization flags
RELEASEFLAGS="-O3 -Wall -s -fno-strict-aliasing -fomit-frame-pointer"

# Release build (comment/uncomment)
$COMPILER -c $SRC_FILES $CSTD $CFLAGS $RELEASEFLAGS -o main.o

# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Edit the above to your liking @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

# Linkage
echo "Starting linking..."
$COMPILER -o $OUTPUT_FILE_NAME main.o $LDFLAGS

if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

echo "Compilation succeeded."

# Check if the output file exists before attempting to run it
if [ -f "$OUTPUT_FILE_NAME" ]; then
    echo "Running $OUTPUT_FILE_NAME..."
    ./$OUTPUT_FILE_NAME
else
    echo "$OUTPUT_FILE_NAME not found."
fi
