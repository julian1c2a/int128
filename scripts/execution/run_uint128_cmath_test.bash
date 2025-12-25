#!/bin/bash

# Script rápido para compilar y ejecutar tests de uint128_cmath

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT" || exit 1

COMPILER="${1:-gcc}"

echo "╔================================================================╗"
echo "║  Quick Test: uint128_cmath.hpp con $COMPILER"
echo "╚================================================================╝"

# Compilar
echo ""
./scripts/build_uint128_cmath_test.bash "$COMPILER"

if [ $? -ne 0 ]; then
    echo ""
    echo "[ERROR] Compilación fallida"
    exit 1
fi

# Ejecutar
echo ""
echo "╔================================================================╗"
echo "║  Ejecutando Tests"
echo "╚================================================================╝"
echo ""

if [ -f "build/tests/test_uint128_cmath_${COMPILER}" ]; then
    ./build/tests/test_uint128_cmath_${COMPILER}
elif [ -f "build/tests/test_uint128_cmath_${COMPILER}.exe" ]; then
    ./build/tests/test_uint128_cmath_${COMPILER}.exe
else
    echo "[ERROR] Ejecutable no encontrado"
    exit 1
fi
