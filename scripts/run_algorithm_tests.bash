#!/bin/bash

# Script para ejecutar algorithm_extracted_tests
# Uso: ./run_algorithm_tests.bash [gcc|clang|msvc|intel|all]

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

# Función para ejecutar con GCC
run_gcc() {
    local EXE="build/build_tests/gcc/release/algorithm_extracted_tests.exe"
    
    if [ ! -f "$EXE" ]; then
        echo -e "${RED}[FAIL] Ejecutable GCC no encontrado: $EXE${NC}"
        echo "  Ejecute primero: ./scripts/build_algorithm_tests.bash gcc"
        return 1
    fi
    
    echo -e "${CYAN}Ejecutando tests (GCC)...${NC}"
    "$EXE"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK] Tests GCC pasados${NC}"
        return 0
    else
        echo -e "${RED}[FAIL] Tests GCC fallaron${NC}"
        return 1
    fi
}

# Función para ejecutar con Clang
run_clang() {
    local EXE="build/build_tests/clang/release/algorithm_extracted_tests.exe"
    
    if [ ! -f "$EXE" ]; then
        echo -e "${RED}[FAIL] Ejecutable Clang no encontrado: $EXE${NC}"
        echo "  Ejecute primero: ./scripts/build_algorithm_tests.bash clang"
        return 1
    fi
    
    echo -e "${CYAN}Ejecutando tests (Clang)...${NC}"
    "$EXE"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK] Tests Clang pasados${NC}"
        return 0
    else
        echo -e "${RED}[FAIL] Tests Clang fallaron${NC}"
        return 1
    fi
}

# Función para ejecutar con MSVC
run_msvc() {
    local EXE="build/build_tests/msvc/release/algorithm_extracted_tests.exe"
    
    if [ ! -f "$EXE" ]; then
        echo -e "${RED}[FAIL] Ejecutable MSVC no encontrado: $EXE${NC}"
        echo "  Ejecute primero: ./scripts/build_algorithm_tests.bash msvc"
        return 1
    fi
    
    echo -e "${CYAN}Ejecutando tests (MSVC)...${NC}"
    "$EXE"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK] Tests MSVC pasados${NC}"
        return 0
    else
        echo -e "${RED}[FAIL] Tests MSVC fallaron${NC}"
        return 1
    fi
}

# Función para ejecutar con Intel
run_intel() {
    local EXE="build/build_tests/intel/release/algorithm_extracted_tests.exe"
    
    if [ ! -f "$EXE" ]; then
        echo -e "${RED}[FAIL] Ejecutable Intel no encontrado: $EXE${NC}"
        echo "  Ejecute primero: ./scripts/build_algorithm_tests.bash intel"
        return 1
    fi
    
    echo -e "${CYAN}Ejecutando tests (Intel)...${NC}"
    "$EXE"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK] Tests Intel pasados${NC}"
        return 0
    else
        echo -e "${RED}[FAIL] Tests Intel fallaron${NC}"
        return 1
    fi
}

# Main
echo "================================"
echo "  Run algorithm_extracted_tests"
echo "================================"
echo ""

COMPILER="${1:-all}"

case "$COMPILER" in
    gcc)
        run_gcc
        ;;
    clang)
        run_clang
        ;;
    msvc)
        run_msvc
        ;;
    intel)
        run_intel
        ;;
    all)
        echo "Ejecutando con todos los compiladores..."
        echo ""
        
        SUCCESS=0
        FAILED=0
        
        run_gcc && ((SUCCESS++)) || ((FAILED++))
        echo ""
        
        run_clang && ((SUCCESS++)) || ((FAILED++))
        echo ""
        
        run_msvc && ((SUCCESS++)) || ((FAILED++))
        echo ""
        
        run_intel && ((SUCCESS++)) || ((FAILED++))
        echo ""
        
        echo "================================"
        echo -e "${GREEN}Exitosos: $SUCCESS${NC} | ${RED}Fallidos: $FAILED${NC}"
        echo "================================"
        
        [ $FAILED -eq 0 ]
        ;;
    *)
        echo -e "${RED}Error: Compilador desconocido '$COMPILER'${NC}"
        echo "Uso: $0 [gcc|clang|msvc|intel|all]"
        exit 1
        ;;
esac
