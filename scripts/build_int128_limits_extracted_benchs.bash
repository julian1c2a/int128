#!/usr/bin/env bash
# Build script para int128_limits_extracted_benchs.cpp con 4 compiladores

set -e

# Detectar directorio del script y directorio raíz del proyecto
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"

# Compilador a usar (por defecto: all)
COMPILER="${1:-all}"

# Mostrar timestamp
echo "========================================="
echo " BUILD: int128_limits_extracted_benchs"
if [ "$COMPILER" != "all" ]; then
    echo " Compilador: $COMPILER"
fi
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="

# Archivo fuente
SOURCE_FILE="$PROJECT_ROOT/benchmarks/int128_limits_extracted_benchs.cpp"

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
INCLUDE_DIR="-I$PROJECT_ROOT/include"

# ---------------------------------------
# 1. GCC
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "gcc" ]; then
echo "🔨 [1/4] Compilando con GCC..."
mkdir -p "$PROJECT_ROOT/build/int128_limits_extracted_benchs/gcc/debug"
mkdir -p "$PROJECT_ROOT/build/int128_limits_extracted_benchs/gcc/release"

g++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
    -o "$PROJECT_ROOT/build/int128_limits_extracted_benchs/gcc/debug/int128_limits_extracted_benchs"
echo "   ✅ GCC Debug: build/int128_limits_extracted_benchs/gcc/debug/int128_limits_extracted_benchs"

g++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O3 -DNDEBUG \
    -o "$PROJECT_ROOT/build/int128_limits_extracted_benchs/gcc/release/int128_limits_extracted_benchs"
echo "   ✅ GCC Release: build/int128_limits_extracted_benchs/gcc/release/int128_limits_extracted_benchs"
fi

# ---------------------------------------
# 2. Clang
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "clang" ]; then
echo ""
echo "🔨 [2/4] Compilando con Clang..."
mkdir -p "$PROJECT_ROOT/build/int128_limits_extracted_benchs/clang/debug"
mkdir -p "$PROJECT_ROOT/build/int128_limits_extracted_benchs/clang/release"

clang++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
    -o "$PROJECT_ROOT/build/int128_limits_extracted_benchs/clang/debug/int128_limits_extracted_benchs"
echo "   ✅ Clang Debug: build/int128_limits_extracted_benchs/clang/debug/int128_limits_extracted_benchs"

clang++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O3 -DNDEBUG \
    -o "$PROJECT_ROOT/build/int128_limits_extracted_benchs/clang/release/int128_limits_extracted_benchs"
echo "   ✅ Clang Release: build/int128_limits_extracted_benchs/clang/release/int128_limits_extracted_benchs"
fi

# ---------------------------------------
# 3. Intel ICX
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "intel" ]; then
echo ""
echo "🔨 [3/4] Compilando con Intel ICX..."

if ! command -v icx &> /dev/null; then
    echo "   ⚠️  Intel ICX no disponible, omitiendo..."
else
    cd "$PROJECT_ROOT"
    
    mkdir -p "build/int128_limits_extracted_benchs/intel/debug"
    mkdir -p "build/int128_limits_extracted_benchs/intel/release"
    
    export MSYS2_ARG_CONV_EXCL="*"
    
    icx benchmarks/int128_limits_extracted_benchs.cpp -Iinclude /std:c++20 /W4 /Zi /Od /EHsc \
        /Fe:build/int128_limits_extracted_benchs/intel/debug/int128_limits_extracted_benchs.exe
    echo "   ✅ Intel ICX Debug: build/int128_limits_extracted_benchs/intel/debug/int128_limits_extracted_benchs.exe"
    
    icx benchmarks/int128_limits_extracted_benchs.cpp -Iinclude /std:c++20 /W4 /O3 /DNDEBUG /EHsc \
        /Fe:build/int128_limits_extracted_benchs/intel/release/int128_limits_extracted_benchs.exe
    echo "   ✅ Intel ICX Release: build/int128_limits_extracted_benchs/intel/release/int128_limits_extracted_benchs.exe"
    
    cd "$SCRIPT_DIR"
fi
fi

# ---------------------------------------
# 4. MSVC (cl.exe)
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "msvc" ]; then
echo ""
echo "🔨 [4/4] Compilando con MSVC..."

if ! command -v cl &> /dev/null; then
    echo "   ⚠️  MSVC no disponible, omitiendo..."
else
    cd "$PROJECT_ROOT"
    
    mkdir -p "build/build_benchmarks/msvc/debug"
    mkdir -p "build/build_benchmarks/msvc/release"
    
    export MSYS2_ARG_CONV_EXCL="*"
    
    cl.exe benchmarks/int128_limits_extracted_benchs.cpp /I./include /std:c++20 /W4 /Zi /Od /EHsc \
        /Fe:build/build_benchmarks/msvc/debug/int128_limits_benchs_msvc.exe > /dev/null 2>&1
    
    result_debug=$?
    if [ $result_debug -eq 0 ]; then
        echo "   ✅ MSVC Debug OK"
        rm -f build/build_benchmarks/msvc/debug/*.obj build/build_benchmarks/msvc/debug/*.pdb
    else
        echo "   ❌ MSVC Debug FAILED"
    fi
    
    cl.exe benchmarks/int128_limits_extracted_benchs.cpp /I./include /std:c++20 /W4 /O2 /DNDEBUG /EHsc \
        /Fe:build/build_benchmarks/msvc/release/int128_limits_benchs_msvc.exe > /dev/null 2>&1
    
    result_release=$?
    if [ $result_release -eq 0 ]; then
        echo "   ✅ MSVC Release OK"
        rm -f build/build_benchmarks/msvc/release/*.obj build/build_benchmarks/msvc/release/*.pdb
    else
        echo "   ❌ MSVC Release FAILED"
    fi
    
    unset MSYS2_ARG_CONV_EXCL
    
    cd "$SCRIPT_DIR"
fi
fi

echo ""
echo "========================================="
echo " ✅ COMPILACIÓN COMPLETADA"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="
