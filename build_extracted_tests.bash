#!/bin/bash

# Script unificado para compilar uint128_extracted_tests.cpp
# Uso: ./build_extracted_tests.bash [gcc|clang|msvc|all]

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Navegar al directorio raíz del proyecto si se ejecuta desde scripts/
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

INCLUDE_DIR="include"
SRC_FILE="tests/uint128_extracted_tests.cpp"

# Función para compilar con GCC
build_gcc() {
    local BUILD_DIR="build/build_tests/gcc/release"
    local OUTPUT_EXE="$BUILD_DIR/test_uint128_extracted.exe"
    
    echo -e "${CYAN}Compilando con GCC...${NC}"
    mkdir -p "$BUILD_DIR"
    
    g++ -std=c++20 -O2 -Wall -Wextra -I "$INCLUDE_DIR" "$SRC_FILE" -o "$OUTPUT_EXE"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Compilación GCC exitosa${NC}"
        echo "  Ejecutable: $OUTPUT_EXE"
        return 0
    else
        echo -e "${RED}✗ Error en compilación GCC${NC}"
        return 1
    fi
}

# Función para compilar con Clang
build_clang() {
    local BUILD_DIR="build/build_tests/clang/release"
    local OUTPUT_EXE="$BUILD_DIR/uint128_extracted_tests.exe"
    
    echo -e "${CYAN}Compilando con Clang...${NC}"
    mkdir -p "$BUILD_DIR"
    
    clang++ -std=c++20 -O2 -Wall -Wextra -I "$INCLUDE_DIR" "$SRC_FILE" -o "$OUTPUT_EXE"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Compilación Clang exitosa${NC}"
        echo "  Ejecutable: $OUTPUT_EXE"
        return 0
    else
        echo -e "${RED}✗ Error en compilación Clang${NC}"
        return 1
    fi
}

# Función para compilar con MSVC
build_msvc() {
    local BUILD_DIR="build/build_tests/msvc/release"
    local OUTPUT_EXE="$BUILD_DIR/uint128_extracted_tests.exe"
    
    echo -e "${CYAN}Compilando con MSVC (cl.exe)...${NC}"
    mkdir -p "$BUILD_DIR"
    
    # Verificar que cl.exe esté disponible
    if ! command -v cl &> /dev/null; then
        echo -e "${YELLOW}⚠ cl.exe no encontrado en el PATH${NC}"
        echo "  Activa el entorno de Visual Studio primero:"
        echo "  source activate_msvc.bash"
        return 1
    fi
    
    cl -nologo -std:c++20 -EHsc -O2 -I "$INCLUDE_DIR" "$SRC_FILE" -Fe"$OUTPUT_EXE" -Fo"$BUILD_DIR/" 2>&1 | grep -v "Creating library"
    
    if [ ${PIPESTATUS[0]} -eq 0 ]; then
        echo -e "${GREEN}✓ Compilación MSVC exitosa${NC}"
        echo "  Ejecutable: $OUTPUT_EXE"
        return 0
    else
        echo -e "${RED}✗ Error en compilación MSVC${NC}"
        return 1
    fi
}

# Función principal
main() {
    local COMPILER="${1:-all}"
    local EXIT_CODE=0
    
    echo "================================"
    echo "  Build uint128_extracted_tests"
    echo "================================"
    echo ""
    
    case "$COMPILER" in
        gcc)
            build_gcc
            EXIT_CODE=$?
            ;;
        clang)
            build_clang
            EXIT_CODE=$?
            ;;
        msvc)
            build_msvc
            EXIT_CODE=$?
            ;;
        all)
            echo -e "${CYAN}Compilando con todos los compiladores...${NC}"
            echo ""
            
            build_gcc
            local gcc_result=$?
            echo ""
            
            build_clang
            local clang_result=$?
            echo ""
            
            build_msvc
            local msvc_result=$?
            echo ""
            
            # Resumen
            echo "================================"
            echo "  Resumen de compilaciones"
            echo "================================"
            if [ $gcc_result -eq 0 ]; then
                echo -e "${GREEN}✓ GCC${NC}"
            else
                echo -e "${RED}✗ GCC${NC}"
                EXIT_CODE=1
            fi
            
            if [ $clang_result -eq 0 ]; then
                echo -e "${GREEN}✓ Clang${NC}"
            else
                echo -e "${RED}✗ Clang${NC}"
                EXIT_CODE=1
            fi
            
            if [ $msvc_result -eq 0 ]; then
                echo -e "${GREEN}✓ MSVC${NC}"
            else
                echo -e "${RED}✗ MSVC${NC}"
                EXIT_CODE=1
            fi
            ;;
        *)
            echo -e "${RED}Error: Compilador no válido '$COMPILER'${NC}"
            echo ""
            echo "Uso: $0 [gcc|clang|msvc|all]"
            echo ""
            echo "Ejemplos:"
            echo "  $0 gcc     # Compilar solo con GCC"
            echo "  $0 clang   # Compilar solo con Clang"
            echo "  $0 msvc    # Compilar solo con MSVC"
            echo "  $0 all     # Compilar con todos (por defecto)"
            exit 1
            ;;
    esac
    
    exit $EXIT_CODE
}

main "$@"
