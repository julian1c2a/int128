#!/bin/bash

# Script rápido para compilar y ejecutar benchmarks de uint128_cmath

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT" || exit 1

COMPILER="${1:-gcc}"

echo "╔════════════════════════════════════════════════════════════════╗"
echo "║  Quick Benchmark: uint128_cmath.hpp con $COMPILER"
echo "╚════════════════════════════════════════════════════════════════╝"

# Compilar
echo ""
./scripts/build_uint128_cmath_bench.bash "$COMPILER"

if [ $? -ne 0 ]; then
    echo ""
    echo "[ERROR] Compilación fallida"
    exit 1
fi

# Ejecutar
echo ""
echo "╔════════════════════════════════════════════════════════════════╗"
echo "║  Ejecutando Benchmarks"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""

BENCH_PATH="build/build_benchmarks/${COMPILER}/release/uint128_cmath_benchmarks"
if [ "$COMPILER" = "msvc" ]; then
    BENCH_PATH="${BENCH_PATH}.exe"
fi

if [ -f "$BENCH_PATH" ]; then
    "$BENCH_PATH"
else
    echo "[ERROR] Ejecutable no encontrado: $BENCH_PATH"
    exit 1
fi
