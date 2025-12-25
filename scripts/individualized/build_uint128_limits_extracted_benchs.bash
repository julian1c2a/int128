#!/usr/bin/env bash
# Build script para uint128_limits_extracted_benchs.cpp con 4 compiladores

set -e

# Detectar directorio del script y directorio raíz del proyecto
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"

# Compilador a usar (por defecto: all)
COMPILER="${1:-all}"

# Mostrar timestamp
echo "========================================="
echo " BUILD: uint128_limits_extracted_benchs"
if [ "$COMPILER" != "all" ]; then
    echo " Compilador: $COMPILER"
fi
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="

# Archivo fuente
SOURCE_FILE="$PROJECT_ROOT/benchmarks/uint128_limits_extracted_benchs.cpp"

# Verificar que existe el archivo
if [ ! -f "$SOURCE_FILE" ]; then
    echo "[FAIL] ERROR: No se encuentra el archivo $SOURCE_FILE"
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
echo "[BUILD] [1/4] Compilando con GCC..."
mkdir -p "$PROJECT_ROOT/build/uint128_limits_extracted_benchs/gcc/debug"
mkdir -p "$PROJECT_ROOT/build/uint128_limits_extracted_benchs/gcc/release"

g++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
    -o "$PROJECT_ROOT/build/uint128_limits_extracted_benchs/gcc/debug/uint128_limits_extracted_benchs"
echo "   [OK] GCC Debug: build/uint128_limits_extracted_benchs/gcc/debug/uint128_limits_extracted_benchs"

g++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O2 -DNDEBUG \
    -o "$PROJECT_ROOT/build/uint128_limits_extracted_benchs/gcc/release/uint128_limits_extracted_benchs"
echo "   [OK] GCC Release: build/uint128_limits_extracted_benchs/gcc/release/uint128_limits_extracted_benchs"
fi

# ---------------------------------------
# 2. Clang
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "clang" ]; then
echo ""
echo "[BUILD] [2/4] Compilando con Clang..."
mkdir -p "$PROJECT_ROOT/build/uint128_limits_extracted_benchs/clang/debug"
mkdir -p "$PROJECT_ROOT/build/uint128_limits_extracted_benchs/clang/release"

clang++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
    -o "$PROJECT_ROOT/build/uint128_limits_extracted_benchs/clang/debug/uint128_limits_extracted_benchs"
echo "   [OK] Clang Debug: build/uint128_limits_extracted_benchs/clang/debug/uint128_limits_extracted_benchs"

clang++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O2 -DNDEBUG \
    -o "$PROJECT_ROOT/build/uint128_limits_extracted_benchs/clang/release/uint128_limits_extracted_benchs"
echo "   [OK] Clang Release: build/uint128_limits_extracted_benchs/clang/release/uint128_limits_extracted_benchs"
fi

# ---------------------------------------
# 3. Intel ICX
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "intel" ]; then
echo ""
echo "[BUILD] [3/4] Compilando con Intel ICX..."

# Verificar si icx está disponible
if ! command -v icx &> /dev/null; then
    echo "   [WARN]  Intel ICX no disponible, omitiendo..."
else
    # Intel ICX necesita rutas sin espacios y formato Windows
    cd "$PROJECT_ROOT"
    
    mkdir -p "build/uint128_limits_extracted_benchs/intel/debug"
    mkdir -p "build/uint128_limits_extracted_benchs/intel/release"
    
    export MSYS2_ARG_CONV_EXCL="*"
    
    icx benchmarks/uint128_limits_extracted_benchs.cpp -Iinclude /std:c++20 /W4 /Zi /Od /EHsc \
        /Fe:build/uint128_limits_extracted_benchs/intel/debug/uint128_limits_extracted_benchs.exe
    echo "   [OK] Intel ICX Debug: build/uint128_limits_extracted_benchs/intel/debug/uint128_limits_extracted_benchs.exe"
    
    icx benchmarks/uint128_limits_extracted_benchs.cpp -Iinclude /std:c++20 /W4 /O3 /DNDEBUG /EHsc \
        /Fe:build/uint128_limits_extracted_benchs/intel/release/uint128_limits_extracted_benchs.exe
    echo "   [OK] Intel ICX Release: build/uint128_limits_extracted_benchs/intel/release/uint128_limits_extracted_benchs.exe"
    
    cd "$SCRIPT_DIR"
fi
fi

# ---------------------------------------
# 4. MSVC (cl.exe)
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "msvc" ]; then
echo ""
echo "[BUILD] [4/4] Compilando con MSVC..."

# Verificar si cl está disponible
if ! command -v cl &> /dev/null; then
    echo "   [WARN]  MSVC no disponible, omitiendo..."
else
    cd "$PROJECT_ROOT"
    
    mkdir -p "build/build_benchmarks/msvc/debug"
    mkdir -p "build/build_benchmarks/msvc/release"
    
    export MSYS2_ARG_CONV_EXCL="*"
    
    cl.exe benchmarks/uint128_limits_extracted_benchs.cpp /I./include /std:c++20 /W4 /Zi /Od /EHsc \
        /Fe:build/build_benchmarks/msvc/debug/uint128_limits_benchs_msvc.exe > /dev/null 2>&1
    
    result_debug=$?
    if [ $result_debug -eq 0 ]; then
        echo "   [OK] MSVC Debug OK"
        rm -f build/build_benchmarks/msvc/debug/*.obj build/build_benchmarks/msvc/debug/*.pdb
    else
        echo "   [FAIL] MSVC Debug FAILED"
    fi
    
    cl.exe benchmarks/uint128_limits_extracted_benchs.cpp /I./include /std:c++20 /W4 /O2 /DNDEBUG /EHsc \
        /Fe:build/build_benchmarks/msvc/release/uint128_limits_benchs_msvc.exe > /dev/null 2>&1
    
    result_release=$?
    if [ $result_release -eq 0 ]; then
        echo "   [OK] MSVC Release OK"
        rm -f build/build_benchmarks/msvc/release/*.obj build/build_benchmarks/msvc/release/*.pdb
    else
        echo "   [FAIL] MSVC Release FAILED"
    fi
    
    unset MSYS2_ARG_CONV_EXCL
    
    cd "$SCRIPT_DIR"
fi
fi

echo ""
echo "========================================="
echo " [OK] COMPILACIÓN COMPLETADA"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="
