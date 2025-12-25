#!/bin/bash

# Script para compilar uint128_cmath_benchmarks.cpp con múltiples compiladores

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT" || exit 1

SOURCE_FILE="benchmarks/uint128_cmath_benchmarks.cpp"
OUTPUT_DIR="build/build_benchmarks"
mkdir -p "$OUTPUT_DIR/gcc/release"
mkdir -p "$OUTPUT_DIR/clang/release"
mkdir -p "$OUTPUT_DIR/msvc/release"
mkdir -p "$OUTPUT_DIR/intel/release"

echo "========================================"
echo "  Build uint128_cmath_benchmarks"
echo "========================================"

# Función para compilar
compile_with() {
    local COMPILER=$1
    local CXX_CMD=$2
    local OUTPUT_PATH=$3
    local EXTRA_FLAGS=$4
    
    echo ""
    echo "[INFO] Compilando con $COMPILER (release)..."
    
    if ! command -v "$CXX_CMD" &> /dev/null; then
        echo "[SKIP] $COMPILER no disponible"
        return 1
    fi
    
    if [ "$COMPILER" = "msvc" ]; then
        # MSVC usa sintaxis diferente
        if cl.exe /std:c++20 /O2 /EHsc /I"include" "$SOURCE_FILE" /Fe:"$OUTPUT_PATH" /link 2>&1; then
            echo "[OK] $COMPILER compilacion exitosa"
            echo "     Output: $OUTPUT_PATH"
            return 0
        fi
    else
        # GCC, Clang, Intel
        if $CXX_CMD -std=c++20 -O3 -march=native -Wall -Wextra $EXTRA_FLAGS \
            -I"include" "$SOURCE_FILE" -o "$OUTPUT_PATH" 2>&1; then
            echo "[OK] $COMPILER compilacion exitosa"
            echo "     Output: $OUTPUT_PATH"
            return 0
        fi
    fi
    
    echo "[ERROR] $COMPILER compilacion fallida"
    return 1
}

SUCCESS=0

if [ "$1" = "gcc" ] || [ -z "$1" ]; then
    if compile_with "gcc" "g++" \
        "$OUTPUT_DIR/gcc/release/uint128_cmath_benchmarks" ""; then
        ((SUCCESS++))
    fi
fi

if [ "$1" = "clang" ] || [ -z "$1" ]; then
    if compile_with "clang" "clang++" \
        "$OUTPUT_DIR/clang/release/uint128_cmath_benchmarks" ""; then
        ((SUCCESS++))
    fi
fi

if [ "$1" = "msvc" ] || [ -z "$1" ]; then
    if compile_with "msvc" "cl.exe" \
        "$OUTPUT_DIR/msvc/release/uint128_cmath_benchmarks.exe" ""; then
        ((SUCCESS++))
    fi
fi

if [ "$1" = "intel" ] || [ -z "$1" ]; then
    if compile_with "intel" "icpx" \
        "$OUTPUT_DIR/intel/release/uint128_cmath_benchmarks" ""; then
        ((SUCCESS++))
    fi
fi

echo ""
echo "========================================"
if [ $SUCCESS -gt 0 ]; then
    echo "[OK] Compilacion completada exitosamente"
else
    echo "[ERROR] Todas las compilaciones fallaron"
fi
echo "========================================"

exit $((SUCCESS > 0 ? 0 : 1))
