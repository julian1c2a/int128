#!/bin/bash

# Script para compilar uint128_extracted_tests.cpp con G++
# Equivalente a build_extracted_tests_gcc.ps1

# Obtener la ruta del directorio del script y la raíz del proyecto
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

SOURCE_FILE="$PROJECT_ROOT/tests/uint128_extracted_tests.cpp"
BUILD_DIR="$PROJECT_ROOT/build/build_tests/gcc/release"
OUTPUT_EXE="$BUILD_DIR/test_uint128_extracted.exe"

# Verificar si el archivo fuente existe
if [ ! -f "$SOURCE_FILE" ]; then
    echo "Error: No se encuentra el archivo fuente: $SOURCE_FILE" >&2
    exit 1
fi

# Crear directorio de salida si no existe
if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
    echo "Directorio creado: $BUILD_DIR"
fi

echo -e "\033[0;36mCompilando $SOURCE_FILE con g++...\033[0m"
# Nota: No usamos -DNDEBUG porque estos tests dependen de assert() para verificar la lógica.
CMD="g++ -std=c++17 -O3 -I\"$PROJECT_ROOT/include\" \"$SOURCE_FILE\" -o \"$OUTPUT_EXE\""
eval "$CMD"



if [ $? -eq 0 ]; then
    echo -e "\033[0;32m✅ Compilación exitosa.\033[0m"
    echo -e "\033[0;90mEjecutable generado en: $OUTPUT_EXE\033[0m"
else
    echo -e "\033[0;31m❌ Error de compilación.\033[0m"
    exit 1
fi