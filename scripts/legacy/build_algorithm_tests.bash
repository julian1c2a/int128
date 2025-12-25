#!/bin/bash

# Script para compilar algorithm_extracted_tests.cpp
# Uso: ./build_algorithm_tests.bash [gcc|clang|msvc|intel|all]

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Navegar al directorio raíz del proyecto
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$PROJECT_ROOT"

INCLUDE_DIR="include"
SRC_FILE="tests/algorithm_extracted_tests.cpp"

# Función para compilar con GCC
build_gcc() {
    local BUILD_DIR="build/build_tests/gcc/release"
    local OUTPUT_EXE="$BUILD_DIR/algorithm_extracted_tests.exe"
    
    echo -e "${CYAN}Compilando con GCC...${NC}"
    mkdir -p "$BUILD_DIR"
    
    g++ -std=c++20 -O2 -Wall -Wextra -I "$INCLUDE_DIR" "$SRC_FILE" -o "$OUTPUT_EXE"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK] Compilacion GCC exitosa${NC}"
        echo "  Ejecutable: $OUTPUT_EXE"
        return 0
    else
        echo -e "${RED}[FAIL] Error en compilacion GCC${NC}"
        return 1
    fi
}

# Función para compilar con Clang
build_clang() {
    local BUILD_DIR="build/build_tests/clang/release"
    local OUTPUT_EXE="$BUILD_DIR/algorithm_extracted_tests.exe"
    
    echo -e "${CYAN}Compilando con Clang...${NC}"
    mkdir -p "$BUILD_DIR"
    
    clang++ -std=c++20 -O2 -Wall -Wextra -I "$INCLUDE_DIR" "$SRC_FILE" -o "$OUTPUT_EXE"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK] Compilacion Clang exitosa${NC}"
        echo "  Ejecutable: $OUTPUT_EXE"
        return 0
    else
        echo -e "${RED}[FAIL] Error en compilacion Clang${NC}"
        return 1
    fi
}

# Función para compilar con MSVC
build_msvc() {
    local BUILD_DIR="build/build_tests/msvc/release"
    local OUTPUT_EXE="$BUILD_DIR/algorithm_extracted_tests.exe"
    
    echo -e "${CYAN}Compilando con MSVC (cl.exe)...${NC}"
    mkdir -p "$BUILD_DIR"
    
    # Verificar que cl.exe esté disponible
    if ! command -v cl &> /dev/null; then
        echo -e "${YELLOW}[WARNING] cl.exe no encontrado. Necesita configurar el entorno de MSVC.${NC}"
        echo "  Ejecute: source <(python scripts/vcvarsall.py)"
        return 1
    fi
    
    # Convertir paths a formato Windows
    local WIN_INCLUDE_DIR=$(cygpath -m "$INCLUDE_DIR")
    local WIN_SRC_FILE=$(cygpath -m "$SRC_FILE")
    local WIN_OUTPUT_EXE=$(cygpath -m "$OUTPUT_EXE")
    local WIN_BUILD_DIR=$(cygpath -m "$BUILD_DIR")
    
    # Ejecutar cl.exe con paths Windows (usando -m para forward slashes que MSVC acepta)
    MSYS_NO_PATHCONV=1 MSYS2_ARG_CONV_EXCL='*' \
        cl.exe /EHsc /std:c++20 /O2 /W4 \
        /I"$WIN_INCLUDE_DIR" \
        "$WIN_SRC_FILE" \
        /Fe:"$WIN_OUTPUT_EXE" \
        /Fo:"$WIN_BUILD_DIR/"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK] Compilacion MSVC exitosa${NC}"
        echo "  Ejecutable: $OUTPUT_EXE"
        return 0
    else
        echo -e "${RED}[FAIL] Error en compilacion MSVC${NC}"
        return 1
    fi
}

# Función para compilar con Intel
build_intel() {
    local BUILD_DIR="build/build_tests/intel/release"
    local OUTPUT_EXE="$BUILD_DIR/algorithm_extracted_tests.exe"
    
    echo -e "${CYAN}Compilando con Intel C++...${NC}"
    
    # Verificar que icpx esté disponible
    if ! command -v icpx &> /dev/null; then
        if ! command -v icx &> /dev/null; then
            echo -e "${YELLOW}[WARNING] Intel C++ compiler (icpx/icx) no encontrado.${NC}"
            echo "  Ejecute: source <(python scripts/setvarsall_intel.py)"
            return 1
        fi
        INTEL_CXX="icx"
    else
        INTEL_CXX="icpx"
    fi
    
    echo "  Usando compilador: $INTEL_CXX"
    mkdir -p "$BUILD_DIR"
    
    $INTEL_CXX -std=c++20 -O2 -Wall -Wextra -I "$INCLUDE_DIR" "$SRC_FILE" -o "$OUTPUT_EXE"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK] Compilacion Intel exitosa${NC}"
        echo "  Ejecutable: $OUTPUT_EXE"
        return 0
    else
        echo -e "${RED}[FAIL] Error en compilacion Intel${NC}"
        return 1
    fi
}

# Main
echo "================================"
echo "  Build algorithm_extracted_tests"
echo "================================"
echo ""

COMPILER="${1:-all}"

case "$COMPILER" in
    gcc)
        build_gcc
        ;;
    clang)
        build_clang
        ;;
    msvc)
        build_msvc
        ;;
    intel)
        build_intel
        ;;
    all)
        echo "Compilando con todos los compiladores..."
        echo ""
        
        SUCCESS=0
        FAILED=0
        
        build_gcc && ((SUCCESS++)) || ((FAILED++))
        echo ""
        
        build_clang && ((SUCCESS++)) || ((FAILED++))
        echo ""
        
        build_msvc && ((SUCCESS++)) || ((FAILED++))
        echo ""
        
        build_intel && ((SUCCESS++)) || ((FAILED++))
        echo ""
        
        echo "================================"
        echo -e "${GREEN}Exitosas: $SUCCESS${NC} | ${RED}Fallidas: $FAILED${NC}"
        echo "================================"
        
        [ $FAILED -eq 0 ]
        ;;
    *)
        echo -e "${RED}Error: Compilador desconocido '$COMPILER'${NC}"
        echo "Uso: $0 [gcc|clang|msvc|intel|all]"
        exit 1
        ;;
esac
