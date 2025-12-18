#!/bin/bash
# Script para compilar y ejecutar tests de thread safety para int128_t

set -e

echo "═══════════════════════════════════════════════════════════"
echo " Compilando int128_thread_safety_tests.cpp"
echo "═══════════════════════════════════════════════════════════"

# Compilar con soporte de threads y atomic
g++ -std=c++20 -O2 -pthread -Wall -Wextra \
    -I./include \
    tests/int128_thread_safety_tests.cpp \
    -o int128_thread_safety_tests \
    -latomic

if [ $? -eq 0 ]; then
    echo "✅ Compilación exitosa!"
    echo ""
    echo "═══════════════════════════════════════════════════════════"
    echo " Ejecutando tests..."
    echo "═══════════════════════════════════════════════════════════"
    echo ""
    
    ./int128_thread_safety_tests
    
    RESULT=$?
    
    echo ""
    echo "═══════════════════════════════════════════════════════════"
    if [ $RESULT -eq 0 ]; then
        echo " ✅ Todos los tests pasaron exitosamente!"
    else
        echo " ❌ Algunos tests fallaron (código: $RESULT)"
    fi
    echo "═══════════════════════════════════════════════════════════"
    
    exit $RESULT
else
    echo "❌ Error en la compilación"
    exit 1
fi
