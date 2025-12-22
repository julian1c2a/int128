#!/usr/bin/env bash
# Build script para int128_limits_extracted_benchs.cpp con 4 compiladores

set -e

# Mostrar timestamp
echo "========================================="
echo " BUILD: int128_limits_extracted_benchs"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="

# Archivo fuente
SOURCE_FILE="../benchmarks/int128_limits_extracted_benchs.cpp"

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
mkdir -p ../build/int128_limits_extracted_benchs/gcc/debug
mkdir -p ../build/int128_limits_extracted_benchs/gcc/release

g++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
    -o ../build/int128_limits_extracted_benchs/gcc/debug/int128_limits_extracted_benchs
echo "   ✅ GCC Debug: build/int128_limits_extracted_benchs/gcc/debug/int128_limits_extracted_benchs"

g++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O3 -DNDEBUG \
    -o ../build/int128_limits_extracted_benchs/gcc/release/int128_limits_extracted_benchs
echo "   ✅ GCC Release: build/int128_limits_extracted_benchs/gcc/release/int128_limits_extracted_benchs"

# ---------------------------------------
# 2. Clang
# ---------------------------------------
echo ""
echo "🔨 [2/4] Compilando con Clang..."
mkdir -p ../build/int128_limits_extracted_benchs/clang/debug
mkdir -p ../build/int128_limits_extracted_benchs/clang/release

clang++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
    -o ../build/int128_limits_extracted_benchs/clang/debug/int128_limits_extracted_benchs
echo "   ✅ Clang Debug: build/int128_limits_extracted_benchs/clang/debug/int128_limits_extracted_benchs"

clang++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O3 -DNDEBUG \
    -o ../build/int128_limits_extracted_benchs/clang/release/int128_limits_extracted_benchs
echo "   ✅ Clang Release: build/int128_limits_extracted_benchs/clang/release/int128_limits_extracted_benchs"

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
    INTEL_SOURCE=../benchmarks/int128_limits_extracted_benchs.cpp
    INTEL_INCLUDE=-I../include
    
    mkdir -p ../build/int128_limits_extracted_benchs/intel/debug
    mkdir -p ../build/int128_limits_extracted_benchs/intel/release
    
    # Evitar conversión de paths de MSYS2
    export MSYS2_ARG_CONV_EXCL="*"
    
    icx $INTEL_SOURCE $INTEL_INCLUDE /std:c++20 /W4 /Zi /Od /EHsc \
        /Fe:../build/int128_limits_extracted_benchs/intel/debug/int128_limits_extracted_benchs.exe
    echo "   ✅ Intel ICX Debug: build/int128_limits_extracted_benchs/intel/debug/int128_limits_extracted_benchs.exe"
    
    icx $INTEL_SOURCE $INTEL_INCLUDE /std:c++20 /W4 /O3 /DNDEBUG /EHsc \
        /Fe:../build/int128_limits_extracted_benchs/intel/release/int128_limits_extracted_benchs.exe
    echo "   ✅ Intel ICX Release: build/int128_limits_extracted_benchs/intel/release/int128_limits_extracted_benchs.exe"
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
    MSVC_SOURCE=../benchmarks/int128_limits_extracted_benchs.cpp
    MSVC_INCLUDE=/I../include
    
    mkdir -p ../build/int128_limits_extracted_benchs/msvc/debug
    mkdir -p ../build/int128_limits_extracted_benchs/msvc/release
    
    # Evitar conversión de paths
    export MSYS2_ARG_CONV_EXCL="*"
    
    cl $MSVC_SOURCE $MSVC_INCLUDE /std:c++20 /W4 /Zi /Od /EHsc \
        /Fe:../build/int128_limits_extracted_benchs/msvc/debug/int128_limits_extracted_benchs.exe
    echo "   ✅ MSVC Debug: build/int128_limits_extracted_benchs/msvc/debug/int128_limits_extracted_benchs.exe"
    
    cl $MSVC_SOURCE $MSVC_INCLUDE /std:c++20 /W4 /O2 /DNDEBUG /EHsc \
        /Fe:../build/int128_limits_extracted_benchs/msvc/release/int128_limits_extracted_benchs.exe
    echo "   ✅ MSVC Release: build/int128_limits_extracted_benchs/msvc/release/int128_limits_extracted_benchs.exe"
fi

echo ""
echo "========================================="
echo " ✅ COMPILACIÓN COMPLETADA"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="
