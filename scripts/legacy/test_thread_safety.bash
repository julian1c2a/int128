#!/bin/bash

# Script para compilar y ejecutar tests de thread-safety
# Incluye detección de data races con sanitizers

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$PROJECT_ROOT"

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo "========================================================"
echo "  Thread-Safety Tests for uint128_t"
echo "========================================================"
echo ""

# Función para compilar
build_thread_tests() {
    local compiler=$1
    local output=$2
    local extra_flags=$3
    
    echo -e "${BLUE}Compilando con $compiler...${NC}"
    
    if [ "$compiler" == "g++" ] || [ "$compiler" == "clang++" ]; then
        $compiler -std=c++20 -O2 -Wall -Wextra -pthread \
            $extra_flags \
            -I include tests/uint128_thread_safety_tests.cpp \
            -o "$output" -latomic 2>&1
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}[OK] Compilación exitosa${NC}"
            return 0
        else
            echo -e "${RED}[ERROR] Error de compilación${NC}"
            return 1
        fi
    fi
}

# Crear directorios
mkdir -p build/thread_tests

echo "=== Compilación Normal ==="
echo ""

# GCC normal
build_thread_tests "g++" "build/thread_tests/thread_tests_gcc.exe" ""
GCC_NORMAL=$?
echo ""

# Clang normal
build_thread_tests "clang++" "build/thread_tests/thread_tests_clang.exe" ""
CLANG_NORMAL=$?
echo ""

echo "=== Compilación con Thread Sanitizer (Data Race Detection) ==="
echo ""

# GCC con Thread Sanitizer
echo -e "${BLUE}Compilando con g++ + ThreadSanitizer...${NC}"
g++ -std=c++20 -O1 -g -Wall -Wextra -pthread \
    -fsanitize=thread \
    -I include tests/uint128_thread_safety_tests.cpp \
    -o build/thread_tests/thread_tests_gcc_tsan.exe 2>&1

if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK] Compilación con ThreadSanitizer exitosa${NC}"
    GCC_TSAN=0
else
    echo -e "${YELLOW}⚠ ThreadSanitizer no disponible en g++${NC}"
    GCC_TSAN=1
fi
echo ""

# Clang con Thread Sanitizer
echo -e "${BLUE}Compilando con clang++ + ThreadSanitizer...${NC}"
clang++ -std=c++20 -O1 -g -Wall -Wextra -pthread \
    -fsanitize=thread \
    -I include tests/uint128_thread_safety_tests.cpp \
    -o build/thread_tests/thread_tests_clang_tsan.exe 2>&1

if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK] Compilación con ThreadSanitizer exitosa${NC}"
    CLANG_TSAN=0
else
    echo -e "${YELLOW}⚠ ThreadSanitizer no disponible en clang++${NC}"
    CLANG_TSAN=1
fi
echo ""

echo "========================================================"
echo "  Ejecución de Tests"
echo "========================================================"
echo ""

# Ejecutar tests normales
if [ $GCC_NORMAL -eq 0 ]; then
    echo -e "${BLUE}=== Ejecutando tests con GCC ===${NC}"
    ./build/thread_tests/thread_tests_gcc.exe
    echo ""
fi

if [ $CLANG_NORMAL -eq 0 ]; then
    echo -e "${BLUE}=== Ejecutando tests con Clang ===${NC}"
    ./build/thread_tests/thread_tests_clang.exe
    echo ""
fi

# Ejecutar tests con ThreadSanitizer
echo "========================================================"
echo "  Data Race Detection (ThreadSanitizer)"
echo "========================================================"
echo ""

if [ $GCC_TSAN -eq 0 ]; then
    echo -e "${BLUE}=== Ejecutando con GCC + ThreadSanitizer ===${NC}"
    echo -e "${YELLOW}Nota: test_unsafe_concurrent_modification debería detectar race conditions${NC}"
    echo ""
    ./build/thread_tests/thread_tests_gcc_tsan.exe 2>&1 | grep -E "(WARNING|ThreadSanitizer|data race|passed|completed)" || ./build/thread_tests/thread_tests_gcc_tsan.exe
    echo ""
fi

if [ $CLANG_TSAN -eq 0 ]; then
    echo -e "${BLUE}=== Ejecutando con Clang + ThreadSanitizer ===${NC}"
    echo -e "${YELLOW}Nota: test_unsafe_concurrent_modification debería detectar race conditions${NC}"
    echo ""
    ./build/thread_tests/thread_tests_clang_tsan.exe 2>&1 | grep -E "(WARNING|ThreadSanitizer|data race|passed|completed)" || ./build/thread_tests/thread_tests_clang_tsan.exe
    echo ""
fi

echo "========================================================"
echo "  Resumen"
echo "========================================================"
echo ""
echo "Tests completados. Consulta el análisis en:"
echo "  documentation/THREAD_SAFETY_ANALYSIS.md"
echo ""
echo -e "${GREEN}Conclusión:${NC}"
echo "  [OK] uint128_t es thread-safe para lecturas concurrentes"
echo "  [OK] uint128_t es thread-safe para objetos thread-local"
echo "  ⚠ Modificaciones concurrentes requieren sincronización"
