#!/bin/bash

# Navegar al directorio raíz del proyecto si se ejecuta desde scripts/
if [ -f "build_extracted_tests_clang.bash" ]; then
    cd ..
fi

BUILD_DIR="build/build_tests/clang/release"
SRC_FILE="tests/uint128_extracted_tests.cpp"
INCLUDE_DIR="include"
OUTPUT_EXE="$BUILD_DIR/uint128_extracted_tests.exe"

echo "Creating output directory: $BUILD_DIR"
mkdir -p "$BUILD_DIR"

echo "Compiling with Clang..."
clang++ -std=c++20 -O3 -I "$INCLUDE_DIR" "$SRC_FILE" -o "$OUTPUT_EXE"

if [ $? -eq 0 ]; then
    echo "Build successful: $OUTPUT_EXE"
else
    echo "Build failed."
    exit 1
fi