#!/bin/bash

# Script para ejecutar int128_algorithm_extracted_tests.cpp compilado
# Uso: ./run_int128_algorithm_tests.bash [gcc|clang|msvc|intel|all]

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Navegar al directorio raiz del proyecto
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$PROJECT_ROOT"

# Funcion para ejecutar tests GCC
run_gcc() {
    local EXE="build/build_tests/gcc/release/int128_algorithm_extracted_tests.exe"
    
    echo -e "${CYAN}Ejecutando tests GCC...${NC}"
    if [ ! -f "$EXE" ]; then
        echo -e "${RED}[FAIL] Ejecutable GCC no encontrado: $EXE${NC}"
        return 1
    fi
    
    "$EXE"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK] Tests GCC pasados${NC}"
        return 0
    else
        echo -e "${RED}[FAIL] Tests GCC fallaron${NC}"
        return 1
    fi
}

# Funcion para ejecutar tests Clang
run_clang() {
    local EXE="build/build_tests/clang/release/int128_algorithm_extracted_tests.exe"
    
    echo -e "${CYAN}Ejecutando tests Clang...${NC}"
    if [ ! -f "$EXE" ]; then
        echo -e "${RED}[FAIL] Ejecutable Clang no encontrado: $EXE${NC}"
        return 1
    fi
    
    "$EXE"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK] Tests Clang pasados${NC}"
        return 0
    else
        echo -e "${RED}[FAIL] Tests Clang fallaron${NC}"
        return 1
    fi
}

# Funcion para ejecutar tests MSVC
run_msvc() {
    local EXE="build/build_tests/msvc/release/int128_algorithm_extracted_tests.exe"
    
    echo -e "${CYAN}Ejecutando tests MSVC...${NC}"
    if [ ! -f "$EXE" ]; then
        echo -e "${RED}[FAIL] Ejecutable MSVC no encontrado: $EXE${NC}"
        return 1
    fi
    
    "$EXE"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK] Tests MSVC pasados${NC}"
        return 0
    else
        echo -e "${RED}[FAIL] Tests MSVC fallaron${NC}"
        return 1
    fi
}

# Funcion para ejecutar tests Intel
run_intel() {
    local EXE="build/build_tests/intel/release/int128_algorithm_extracted_tests.exe"
    
    echo -e "${CYAN}Ejecutando tests Intel...${NC}"
    if [ ! -f "$EXE" ]; then
        echo -e "${RED}[FAIL] Ejecutable Intel no encontrado: $EXE${NC}"
        return 1
    fi
    
    "$EXE"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK] Tests Intel pasados${NC}"
        return 0
    else
        echo -e "${RED}[FAIL] Tests Intel fallaron${NC}"
        return 1
    fi
}

# Funcion principal
main() {
    local compiler="${1:-all}"
    local exit_code=0
    
    echo "================================"
    echo "  Run int128_algorithm_extracted_tests"
    echo "================================"
    echo ""
    
    case "$compiler" in
        gcc)
            run_gcc
            exit_code=$?
            ;;
        clang)
            run_clang
            exit_code=$?
            ;;
        msvc)
            run_msvc
            exit_code=$?
            ;;
        intel)
            run_intel
            exit_code=$?
            ;;
        all)
            run_gcc
            local gcc_result=$?
            
            run_clang
            local clang_result=$?
            
            run_msvc
            local msvc_result=$?
            
            run_intel
            local intel_result=$?
            
            # Si alguno fallo, return 1
            if [ $gcc_result -ne 0 ] || [ $clang_result -ne 0 ] || [ $msvc_result -ne 0 ] || [ $intel_result -ne 0 ]; then
                exit_code=1
            fi
            ;;
        *)
            echo "Uso: $0 [gcc|clang|msvc|intel|all]"
            exit_code=1
            ;;
    esac
    
    return $exit_code
}

main "$@"
