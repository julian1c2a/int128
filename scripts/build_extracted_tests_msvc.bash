#!/bin/bash

# Navegar al directorio raíz del proyecto si se ejecuta desde scripts/
if [ -f "build_extracted_tests_msvc.bash" ]; then
    cd ..
fi

BUILD_DIR="build/build_tests/msvc/release"
SRC_FILE="tests/uint128_extracted_tests.cpp"
INCLUDE_DIR="include"
OUTPUT_EXE="$BUILD_DIR/uint128_extracted_tests.exe"

echo "Creating output directory: $BUILD_DIR"
mkdir -p "$BUILD_DIR"

echo "Compiling with MSVC (cl.exe)..."
# Flags:
# -nologo: Suprime el banner de copyright
# -std:c++20: Estándar C++20
# -EHsc: Manejo de excepciones estándar
# -O2: Optimización máxima velocidad
# -I: Directorio de includes
# -Fe: Nombre del ejecutable de salida
# -Fo: Directorio para archivos objeto (.obj)
cl -nologo -std:c++20 -EHsc -O2 -I "$INCLUDE_DIR" "$SRC_FILE" -Fe"$OUTPUT_EXE" -Fo"$BUILD_DIR/"

if [ $? -eq 0 ]; then
    echo "Build successful: $OUTPUT_EXE"
else
    echo "Build failed."
    exit 1
fi