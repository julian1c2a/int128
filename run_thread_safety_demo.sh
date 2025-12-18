#!/bin/bash
# Script para compilar y ejecutar la demo de int128_t thread safety

set -e

echo "═══════════════════════════════════════════════════════════"
echo " Compilando demo_int128_thread_safety.cpp"
echo "═══════════════════════════════════════════════════════════"

# Compilar (con -latomic para soporte de std::atomic<int128_t>)
g++ -std=c++20 -O2 -pthread -Wall -Wextra \
    -I./include \
    demos/demo_int128_thread_safety.cpp \
    -o demo_int128_thread_safety \
    -latomic

if [ $? -eq 0 ]; then
    echo "✅ Compilación exitosa!"
    echo ""
    echo "═══════════════════════════════════════════════════════════"
    echo " Ejecutando demo..."
    echo "═══════════════════════════════════════════════════════════"
    echo ""
    
    ./demo_int128_thread_safety
    
    echo ""
    echo "═══════════════════════════════════════════════════════════"
    echo " Demo completada!"
    echo "═══════════════════════════════════════════════════════════"
else
    echo "❌ Error en la compilación"
    exit 1
fi
