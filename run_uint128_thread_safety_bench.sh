#!/bin/bash
# Script para compilar y ejecutar benchmarks de thread safety para uint128_t

set -e

echo "═══════════════════════════════════════════════════════════"
echo " Compilando uint128_thread_safety_benchmarks.cpp"
echo "═══════════════════════════════════════════════════════════"

# Compilar con optimizaciones y soporte de threads
g++ -std=c++20 -O3 -pthread -Wall -Wextra \
    -I./include \
    benchmarks/uint128_thread_safety_benchmarks.cpp \
    -o uint128_thread_safety_bench \
    -latomic

if [ $? -eq 0 ]; then
    echo "✅ Compilación exitosa!"
    echo ""
    echo "═══════════════════════════════════════════════════════════"
    echo " Ejecutando benchmarks..."
    echo "═══════════════════════════════════════════════════════════"
    echo ""
    
    ./uint128_thread_safety_bench
    
    RESULT=$?
    
    echo ""
    echo "═══════════════════════════════════════════════════════════"
    if [ $RESULT -eq 0 ]; then
        echo " ✅ Benchmarks completados exitosamente!"
    else
        echo " ❌ Error en los benchmarks (código: $RESULT)"
    fi
    echo "═══════════════════════════════════════════════════════════"
    
    exit $RESULT
else
    echo "❌ Error en la compilación"
    exit 1
fi
