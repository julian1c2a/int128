#!/bin/bash

# Script unificado para ejecutar uint128_extracted_tests
# Uso: ./run_uint128_extracted_tests.bash [gcc|clang|msvc|all]

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Navegar al directorio raíz del proyecto
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Función para ejecutar tests con GCC
run_gcc() {
    local BUILD_DIR="build/build_tests/gcc/release"
    local OUTPUT_EXE="$BUILD_DIR/test_uint128_extracted.exe"
    
    echo -e "${CYAN}Ejecutando tests compilados con GCC...${NC}"
    echo "================================"
    
    if [ ! -f "$OUTPUT_EXE" ]; then
        echo -e "${YELLOW}⚠ Ejecutable no encontrado: $OUTPUT_EXE${NC}"
        echo "  Compílalo primero con: ./build_extracted_tests.bash gcc"
        return 1
    fi
    
    "$OUTPUT_EXE"
    local result=$?
    
    echo ""
    if [ $result -eq 0 ]; then
        echo -e "${GREEN}✓ Tests GCC: PASSED${NC}"
    else
        echo -e "${RED}✗ Tests GCC: FAILED (código de salida: $result)${NC}"
    fi
    
    return $result
}

# Función para ejecutar tests con Clang
run_clang() {
    local BUILD_DIR="build/build_tests/clang/release"
    local OUTPUT_EXE="$BUILD_DIR/uint128_extracted_tests.exe"
    
    echo -e "${CYAN}Ejecutando tests compilados con Clang...${NC}"
    echo "================================"
    
    if [ ! -f "$OUTPUT_EXE" ]; then
        echo -e "${YELLOW}⚠ Ejecutable no encontrado: $OUTPUT_EXE${NC}"
        echo "  Compílalo primero con: ./build_extracted_tests.bash clang"
        return 1
    fi
    
    "$OUTPUT_EXE"
    local result=$?
    
    echo ""
    if [ $result -eq 0 ]; then
        echo -e "${GREEN}✓ Tests Clang: PASSED${NC}"
    else
        echo -e "${RED}✗ Tests Clang: FAILED (código de salida: $result)${NC}"
    fi
    
    return $result
}

# Función para ejecutar tests con MSVC
run_msvc() {
    local BUILD_DIR="build/build_tests/msvc/release"
    local OUTPUT_EXE="$BUILD_DIR/uint128_extracted_tests.exe"
    
    echo -e "${CYAN}Ejecutando tests compilados con MSVC...${NC}"
    echo "================================"
    
    if [ ! -f "$OUTPUT_EXE" ]; then
        echo -e "${YELLOW}⚠ Ejecutable no encontrado: $OUTPUT_EXE${NC}"
        echo "  Compílalo primero con: ./build_extracted_tests.bash msvc"
        return 1
    fi
    
    "$OUTPUT_EXE"
    local result=$?
    
    echo ""
    if [ $result -eq 0 ]; then
        echo -e "${GREEN}✓ Tests MSVC: PASSED${NC}"
    else
        echo -e "${RED}✗ Tests MSVC: FAILED (código de salida: $result)${NC}"
    fi
    
    return $result
}

# Función principal
main() {
    local COMPILER="${1:-all}"
    local EXIT_CODE=0
    
    echo "========================================"
    echo "  Run uint128_extracted_tests"
    echo "========================================"
    echo ""
    
    case "$COMPILER" in
        gcc)
            run_gcc
            EXIT_CODE=$?
            ;;
        clang)
            run_clang
            EXIT_CODE=$?
            ;;
        msvc)
            run_msvc
            EXIT_CODE=$?
            ;;
        all)
            echo -e "${CYAN}Ejecutando tests con todos los compiladores...${NC}"
            echo ""
            
            run_gcc
            local gcc_result=$?
            echo ""
            echo ""
            
            run_clang
            local clang_result=$?
            echo ""
            echo ""
            
            run_msvc
            local msvc_result=$?
            echo ""
            
            # Resumen
            echo "========================================"
            echo "  Resumen de tests"
            echo "========================================"
            if [ $gcc_result -eq 0 ]; then
                echo -e "${GREEN}✓ GCC: PASSED${NC}"
            else
                echo -e "${RED}✗ GCC: FAILED${NC}"
                EXIT_CODE=1
            fi
            
            if [ $clang_result -eq 0 ]; then
                echo -e "${GREEN}✓ Clang: PASSED${NC}"
            else
                echo -e "${RED}✗ Clang: FAILED${NC}"
                EXIT_CODE=1
            fi
            
            if [ $msvc_result -eq 0 ]; then
                echo -e "${GREEN}✓ MSVC: PASSED${NC}"
            else
                echo -e "${RED}✗ MSVC: FAILED${NC}"
                EXIT_CODE=1
            fi
            
            echo ""
            if [ $EXIT_CODE -eq 0 ]; then
                echo -e "${GREEN}🎉 Todos los tests pasaron exitosamente${NC}"
            else
                echo -e "${RED}❌ Algunos tests fallaron${NC}"
            fi
            ;;
        *)
            echo -e "${RED}Error: Compilador no válido '$COMPILER'${NC}"
            echo ""
            echo "Uso: $0 [gcc|clang|msvc|all]"
            echo ""
            echo "Ejemplos:"
            echo "  $0 gcc     # Ejecutar tests compilados con GCC"
            echo "  $0 clang   # Ejecutar tests compilados con Clang"
            echo "  $0 msvc    # Ejecutar tests compilados con MSVC"
            echo "  $0 all     # Ejecutar todos los tests (por defecto)"
            exit 1
            ;;
    esac
    
    exit $EXIT_CODE
}

main "$@"
