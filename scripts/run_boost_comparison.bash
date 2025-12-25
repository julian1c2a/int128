#!/bin/bash

# Script para compilar y ejecutar benchmarks comparativos con Boost.Multiprecision
# Compara uint128_t con boost::multiprecision::uint128_t (128-bit fixed)

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

echo "================================================================================================"
echo "COMPILACIÓN DE BENCHMARKS COMPARATIVOS: uint128_t vs Boost.Multiprecision"
echo "================================================================================================"
echo ""

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Verificar que Boost está instalado
echo "➜ Verificando Boost.Multiprecision..."
if ! g++ -std=c++20 -E -x c++ - <<< '#include <boost/multiprecision/cpp_int.hpp>' &>/dev/null; then
    echo -e "${RED}✗ Error: Boost.Multiprecision no encontrado${NC}"
    echo ""
    echo "Para instalar Boost en MSYS2:"
    echo "  pacman -S mingw-w64-ucrt-x86_64-boost"
    echo ""
    echo "En Ubuntu/Debian:"
    echo "  sudo apt-get install libboost-dev"
    echo ""
    exit 1
fi
echo -e "${GREEN}✓ Boost.Multiprecision encontrado${NC}"
echo ""

# Crear directorio de salida
mkdir -p "$PROJECT_ROOT/build"

# Función para compilar con un compilador
compile_benchmark() {
    local compiler=$1
    local compiler_name=$2
    local output_suffix=$3
    
    echo "➜ Compilando con $compiler_name..."
    
    local output_file="$PROJECT_ROOT/build/benchmark_comparison_boost_$output_suffix"
    
    if $compiler -std=c++20 -O3 \
        -I"$PROJECT_ROOT/include" \
        "$PROJECT_ROOT/benchmarks/benchmark_comparison_boost.cpp" \
        -o "$output_file" 2>&1; then
        echo -e "${GREEN}✓ Compilación exitosa: $output_file${NC}"
        return 0
    else
        echo -e "${RED}✗ Error en compilación con $compiler_name${NC}"
        return 1
    fi
}

# Compilar con GCC
if command -v g++ &> /dev/null; then
    compile_benchmark "g++" "GCC" "gcc"
    GCC_SUCCESS=1
else
    echo -e "${YELLOW}⊘ GCC no encontrado, saltando...${NC}"
    GCC_SUCCESS=0
fi

echo ""

# Compilar con Clang
if command -v clang++ &> /dev/null; then
    compile_benchmark "clang++" "Clang" "clang"
    CLANG_SUCCESS=1
else
    echo -e "${YELLOW}⊘ Clang no encontrado, saltando...${NC}"
    CLANG_SUCCESS=0
fi

echo ""
echo "================================================================================================"

# Ejecutar benchmarks
if [ $GCC_SUCCESS -eq 1 ]; then
    echo ""
    echo "➜ Ejecutando benchmark con GCC..."
    echo "================================================================================================"
    "$PROJECT_ROOT/build/benchmark_comparison_boost_gcc"
fi

if [ $CLANG_SUCCESS -eq 1 ]; then
    echo ""
    echo "➜ Ejecutando benchmark con Clang..."
    echo "================================================================================================"
    "$PROJECT_ROOT/build/benchmark_comparison_boost_clang"
fi

echo ""
echo "================================================================================================"
echo "BENCHMARKS COMPLETADOS"
echo "================================================================================================"
echo ""
echo "Ejecutables generados:"
[ $GCC_SUCCESS -eq 1 ] && echo "  - build/benchmark_comparison_boost_gcc"
[ $CLANG_SUCCESS -eq 1 ] && echo "  - build/benchmark_comparison_boost_clang"
echo ""
echo "Análisis detallado disponible en: BOOST_COMPARISON_ANALYSIS.md"
echo ""
