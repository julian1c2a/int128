#!/bin/bash
# Script para compilar tests y benchmarks de uint128_traits
# Uso: ./compile_uint128_traits_extracted.bash [gcc|clang|intel|msvc|all]

set -u # Error si se usan variables no definidas

PROJECT_DIR="/c/msys64/ucrt64/home/julian/CppProjects/int128"
cd "$PROJECT_DIR" || { echo "Error: No encuentro el directorio del proyecto"; exit 1; }

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Argumento
COMPILER="${1:-all}"
COMPILER=$(echo "$COMPILER" | tr '[:upper:]' '[:lower:]')

echo -e "${BLUE}============================================================${NC}"
echo -e "${BLUE}  Compilación de uint128_traits${NC}"
echo -e "${BLUE}  Modo: ${COMPILER}${NC}"
echo -e "${BLUE}============================================================${NC}"

should_compile() {
    local comp=$1
    [ "$COMPILER" = "all" ] || [ "$COMPILER" = "$comp" ]
}

# Crear dirs
mkdir -p build/build_tests/{gcc,clang,intel,msvc}/release
mkdir -p build/build_benchmarks/{gcc,clang,intel,msvc}/release

# -----------------------------------------------------------------------------
# INTEL ONEAPI (ICX)
# -----------------------------------------------------------------------------
if should_compile "intel"; then
    echo -e "\n${YELLOW}[INTEL] Compilando con Intel OneAPI...${NC}"

    if command -v icx &> /dev/null; then
        ICX_CMD="icx"
        # Flags para Intel en Windows (compatible con MSVC)
        # /EHsc: Manejo de excepciones estándar C++
        # /std:c++20: Estándar C++20
        # /O3: Optimización máxima
        # /QxHost: Optimizar para este procesador (equiv. a -march=native)
        ICX_FLAGS="-std=c++20 -O3 -QxHost -Wall /EHsc"
        
        echo -e "${GREEN}  - Compilando tests...${NC}"
        
        # Compilación explícita
        "$ICX_CMD" $ICX_FLAGS \
            -I"$PROJECT_DIR/include" \
            tests/uint128_traits_extracted_tests.cpp \
            -o build/build_tests/intel/release/uint128_traits_tests_intel.exe
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}    ✅ Tests compilados OK${NC}"
            ls -l --time-style=+%H:%M:%S build/build_tests/intel/release/uint128_traits_tests_intel.exe
        else
            echo -e "${RED}    ❌ Error compilando tests${NC}"
            exit 1
        fi

        echo -e "${GREEN}  - Compilando benchmarks...${NC}"
        "$ICX_CMD" $ICX_FLAGS \
            -I"$PROJECT_DIR/include" \
            benchmarks/uint128_traits_extracted_benchs.cpp \
            -o build/build_benchmarks/intel/release/uint128_traits_benchs_intel.exe
            
        if [ $? -eq 0 ]; then
             echo -e "${GREEN}    ✅ Benchmarks compilados OK${NC}"
        fi
    else
        echo -e "${RED}  ⚠️  Intel 'icx' no encontrado en el PATH.${NC}"
        echo -e "      Ejecuta primero: source scripts/setup_intel_combined.bash"
    fi
fi

# -----------------------------------------------------------------------------
# G++ (UCRT64) - Solo si se pide explícitamente o 'all'
# -----------------------------------------------------------------------------
if should_compile "gcc"; then
    echo -e "\n${YELLOW}[GCC] Compilando con G++...${NC}"
    # Usamos g++ del path si existe, para evitar hardcodeo que falle en otros shells
    if command -v g++ &> /dev/null; then
        g++ -std=c++20 -O3 -march=native -Wall -Wextra \
            -I"$PROJECT_DIR/include" \
            tests/uint128_traits_extracted_tests.cpp \
            -o build/build_tests/gcc/release/uint128_traits_tests_gcc.exe && \
        echo -e "${GREEN}  ✅ GCC OK${NC}" || echo -e "${RED}  ❌ GCC Falló${NC}"
    else
        echo -e "${YELLOW}  ⚠️  g++ no encontrado en PATH (¿Estás en el shell correcto?)${NC}"
    fi
fi

echo -e "\n${BLUE}============================================================${NC}"
echo -e "${BLUE}  Fin del proceso${NC}"
echo -e "${BLUE}============================================================${NC}"