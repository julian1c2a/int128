#!/bin/bash

# Script para compilar test_uint128_cmath.cpp con múltiples compiladores

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT" || exit 1

SOURCE_FILE="tests/test_uint128_cmath.cpp"
OUTPUT_DIR="build/tests"
mkdir -p "$OUTPUT_DIR"

echo "========================================"
echo "  Compilando test_uint128_cmath.cpp"
echo "========================================"

# Función para compilar con un compilador específico
compile_with() {
    local COMPILER=$1
    local CXX_CMD=$2
    local OUTPUT_NAME="${OUTPUT_DIR}/test_uint128_cmath_${COMPILER}"
    
    echo ""
    echo "[INFO] Compilando con $COMPILER..."
    
    if ! command -v "$CXX_CMD" &> /dev/null; then
        echo "[SKIP] $COMPILER no disponible"
        return 1
    fi
    
    if [ "$COMPILER" = "msvc" ]; then
        # MSVC usa sintaxis diferente
        if cl.exe /std:c++20 /O2 /EHsc /W4 /I"include" "$SOURCE_FILE" /Fe:"$OUTPUT_NAME" 2>&1; then
            echo "[OK] $COMPILER compilación exitosa"
            echo "     Output: $OUTPUT_NAME"
            return 0
        fi
    elif $CXX_CMD -std=c++20 -O3 -Wall -Wextra -I"include" "$SOURCE_FILE" -o "$OUTPUT_NAME" 2>&1; then
        echo "[OK] $COMPILER compilación exitosa"
        echo "     Output: $OUTPUT_NAME"
        return 0
    else
        echo "[ERROR] $COMPILER compilación falló"
        return 1
    fi
}

# Compilar con cada compilador disponible
SUCCESS=0

if [ "$1" = "gcc" ] || [ -z "$1" ]; then
    if compile_with "gcc" "g++"; then
        ((SUCCESS++))
    fi
fi

if [ "$1" = "clang" ] || [ -z "$1" ]; then
    if compile_with "clang" "clang++"; then
        ((SUCCESS++))
    fi
fi

if [ "$1" = "msvc" ] || [ -z "$1" ]; then
    if compile_with "msvc" "cl.exe"; then
        ((SUCCESS++))
    fi
fi

if [ "$1" = "intel" ] || [ -z "$1" ]; then
    if compile_with "intel" "icpx"; then
        ((SUCCESS++))
    fi
fi

echo ""
echo "========================================"
if [ $SUCCESS -gt 0 ]; then
    echo "[OK] Compilación completada: $SUCCESS compilador(es)"
    echo "========================================"
    exit 0
else
    echo "[ERROR] Ninguna compilación exitosa"
    echo "========================================"
    exit 1
fi
