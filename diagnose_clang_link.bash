#!/usr/bin/env bash
# Script temporal para diagnosticar errores de linkado con Clang

cd /c/msys64/ucrt64/home/julian/CppProjects/int128

echo "========================================="
echo "Compilando uint128_t_extracted_tests.cpp"
echo "con Clang (Debug) - SIN FILTROS"
echo "========================================="
echo ""

SOURCE_FILE="tests/uint128_t_extracted_tests.cpp"
INCLUDE_DIR="-Iinclude"
COMMON_FLAGS="-std=c++20 -Wall -Wextra"
OUTPUT="build/build_tests/clang/debug/uint128_t_extracted_tests_diagnostic.exe"

mkdir -p build/build_tests/clang/debug

echo "Ejecutando:"
echo "clang++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 -o $OUTPUT"
echo ""

clang++ "$SOURCE_FILE" $INCLUDE_DIR $COMMON_FLAGS -g -O0 -o "$OUTPUT"

EXIT_CODE=$?

echo ""
echo "========================================="
if [ $EXIT_CODE -eq 0 ]; then
    echo "✅ Compilación exitosa"
    ls -lh "$OUTPUT"
else
    echo "❌ Compilación fallida (exit code: $EXIT_CODE)"
fi
echo "========================================="

exit $EXIT_CODE
