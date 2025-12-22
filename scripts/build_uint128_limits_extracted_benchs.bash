#!/usr/bin/env bash
# Build script para uint128_limits_extracted_benchs.cpp con 4 compiladores

set -e

# Mostrar timestamp
echo "========================================="
echo " BUILD: uint128_limits_extracted_benchs"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="

# Archivo fuente
SOURCE_FILE="../benchmarks/uint128_limits_extracted_benchs.cpp"

# Verificar que existe el archivo
if [ ! -f "$SOURCE_FILE" ]; then
    echo "❌ ERROR: No se encuentra el archivo $SOURCE_FILE"
    exit 1
fi

# Mostrar info del archivo
echo ""
echo "📄 Archivo fuente:"
ls -lh "$SOURCE_FILE" | awk '{print "   " $9 " (" $5 ")"}'
echo ""

# Flags comunes
COMMON_FLAGS="-std=c++20 -Wall -Wextra"
INCLUDE_DIR="-I../include"

# ---------------------------------------
# 1. GCC
# ---------------------------------------
echo "🔨 [1/4] Compilando con GCC..."
mkdir -p ../build/uint128_limits_extracted_benchs/gcc/debug
mkdir -p ../build/uint128_limits_extracted_benchs/gcc/release

g++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
    -o ../build/uint128_limits_extracted_benchs/gcc/debug/uint128_limits_extracted_benchs
echo "   ✅ GCC Debug: build/uint128_limits_extracted_benchs/gcc/debug/uint128_limits_extracted_benchs"

g++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O3 -DNDEBUG \
    -o ../build/uint128_limits_extracted_benchs/gcc/release/uint128_limits_extracted_benchs
echo "   ✅ GCC Release: build/uint128_limits_extracted_benchs/gcc/release/uint128_limits_extracted_benchs"

# ---------------------------------------
# 2. Clang
# ---------------------------------------
echo ""
echo "🔨 [2/4] Compilando con Clang..."
mkdir -p ../build/uint128_limits_extracted_benchs/clang/debug
mkdir -p ../build/uint128_limits_extracted_benchs/clang/release

clang++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
    -o ../build/uint128_limits_extracted_benchs/clang/debug/uint128_limits_extracted_benchs
echo "   ✅ Clang Debug: build/uint128_limits_extracted_benchs/clang/debug/uint128_limits_extracted_benchs"

clang++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O3 -DNDEBUG \
    -o ../build/uint128_limits_extracted_benchs/clang/release/uint128_limits_extracted_benchs
echo "   ✅ Clang Release: build/uint128_limits_extracted_benchs/clang/release/uint128_limits_extracted_benchs"

# ---------------------------------------
# 3. Intel ICX
# ---------------------------------------
echo ""
echo "🔨 [3/4] Compilando con Intel ICX..."

# Verificar si icx está disponible
if ! command -v icx &> /dev/null; then
    echo "   ⚠️  Intel ICX no disponible, omitiendo..."
else
    # Intel ICX necesita path con \ en Windows
    # Usamos paths relativos sin comillas
    INTEL_SOURCE=../benchmarks/uint128_limits_extracted_benchs.cpp
    INTEL_INCLUDE=-I../include
    
    mkdir -p ../build/uint128_limits_extracted_benchs/intel/debug
    mkdir -p ../build/uint128_limits_extracted_benchs/intel/release
    
    # Evitar conversión de paths de MSYS2
    export MSYS2_ARG_CONV_EXCL="*"
    
    icx $INTEL_SOURCE $INTEL_INCLUDE /std:c++20 /W4 /Zi /Od /EHsc \
        /Fe:../build/uint128_limits_extracted_benchs/intel/debug/uint128_limits_extracted_benchs.exe
    echo "   ✅ Intel ICX Debug: build/uint128_limits_extracted_benchs/intel/debug/uint128_limits_extracted_benchs.exe"
    
    icx $INTEL_SOURCE $INTEL_INCLUDE /std:c++20 /W4 /O3 /DNDEBUG /EHsc \
        /Fe:../build/uint128_limits_extracted_benchs/intel/release/uint128_limits_extracted_benchs.exe
    echo "   ✅ Intel ICX Release: build/uint128_limits_extracted_benchs/intel/release/uint128_limits_extracted_benchs.exe"
fi

# ---------------------------------------
# 4. MSVC (cl.exe)
# ---------------------------------------
echo ""
echo "🔨 [4/4] Compilando con MSVC..."

# Verificar si cl está disponible
if ! command -v cl &> /dev/null; then
    echo "   ⚠️  MSVC no disponible, omitiendo..."
else
    MSVC_SOURCE=../benchmarks/uint128_limits_extracted_benchs.cpp
    MSVC_INCLUDE=/I../include
    
    mkdir -p ../build/uint128_limits_extracted_benchs/msvc/debug
    mkdir -p ../build/uint128_limits_extracted_benchs/msvc/release
    
    # Evitar conversión de paths
    export MSYS2_ARG_CONV_EXCL="*"
    
    cl $MSVC_SOURCE $MSVC_INCLUDE /std:c++20 /W4 /Zi /Od /EHsc \
        /Fe:../build/uint128_limits_extracted_benchs/msvc/debug/uint128_limits_extracted_benchs.exe
    echo "   ✅ MSVC Debug: build/uint128_limits_extracted_benchs/msvc/debug/uint128_limits_extracted_benchs.exe"
    
    cl $MSVC_SOURCE $MSVC_INCLUDE /std:c++20 /W4 /O2 /DNDEBUG /EHsc \
        /Fe:../build/uint128_limits_extracted_benchs/msvc/release/uint128_limits_extracted_benchs.exe
    echo "   ✅ MSVC Release: build/uint128_limits_extracted_benchs/msvc/release/uint128_limits_extracted_benchs.exe"
fi

echo ""
echo "========================================="
echo " ✅ COMPILACIÓN COMPLETADA"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="
