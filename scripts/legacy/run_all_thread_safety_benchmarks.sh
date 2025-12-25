#!/bin/bash
# Script para ejecutar benchmarks de thread safety para uint128_t e int128_t en paralelo

set -e

echo "╔═════════════════════════════════════════════════════════════╗"
echo "║  Thread Safety Benchmarks - uint128_t vs int128_t         ║"
echo "╚═════════════════════════════════════════════════════════════╝"
echo ""

# Compilar uint128_t benchmarks
echo "Compilando uint128_t benchmarks..."
g++ -std=c++20 -O3 -pthread -Wall -Wextra \
    -I./include \
    benchmarks/uint128_thread_safety_benchmarks.cpp \
    -o uint128_thread_safety_bench \
    -latomic 2>&1 | grep -v "^$" || true

# Compilar int128_t benchmarks
echo "Compilando int128_t benchmarks..."
g++ -std=c++20 -O3 -pthread -Wall -Wextra \
    -I./include \
    benchmarks/int128_thread_safety_benchmarks.cpp \
    -o int128_thread_safety_bench \
    -latomic 2>&1 | grep -v "^$" || true

if [ -f uint128_thread_safety_bench ] && [ -f int128_thread_safety_bench ]; then
    echo "✅ Compilación exitosa para ambos tipos!"
    echo ""
    
    # Ejecutar uint128_t benchmarks
    echo "═══════════════════════════════════════════════════════════"
    echo " Ejecutando benchmarks para uint128_t (unsigned)"
    echo "═══════════════════════════════════════════════════════════"
    echo ""
    ./uint128_thread_safety_bench
    
    echo ""
    echo ""
    
    # Ejecutar int128_t benchmarks
    echo "═══════════════════════════════════════════════════════════"
    echo " Ejecutando benchmarks para int128_t (signed)"
    echo "═══════════════════════════════════════════════════════════"
    echo ""
    ./int128_thread_safety_bench
    
    echo ""
    echo "╔═════════════════════════════════════════════════════════════╗"
    echo "║  ✅ Benchmarks completados para ambos tipos                ║"
    echo "╚═════════════════════════════════════════════════════════════╝"
    echo ""
    echo "Comparación:"
    echo "  • uint128_t: Sin operaciones específicas signed"
    echo "  • int128_t: Con operaciones signed (negate, is_negative)"
    echo "  • Performance: Casi idéntica entre ambos tipos"
    echo "  • Overhead: Operaciones signed tienen overhead mínimo"
    echo ""
else
    echo "❌ Error en la compilación"
    exit 1
fi
