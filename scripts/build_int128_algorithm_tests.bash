#!/bin/bash

# Script para compilar int128_algorithm_extracted_tests.cpp
# Uso: ./build_int128_algorithm_tests.bash [gcc|clang|msvc|intel|all]

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Navegar al directorio raiz del proyecto
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$PROJECT_ROOT"

INCLUDE_DIR="include"
SRC_FILE="tests/int128_algorithm_extracted_tests.cpp"

# Funcion para compilar con GCC
build_gcc() {
    local BUILD_DIR="build/build_tests/gcc/release"
    local OUTPUT_EXE="$BUILD_DIR/int128_algorithm_extracted_tests.exe"
    
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

# Funcion para compilar con Clang
build_clang() {
    local BUILD_DIR="build/build_tests/clang/release"
    local OUTPUT_EXE="$BUILD_DIR/int128_algorithm_extracted_tests.exe"
    
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

# Funcion para compilar con MSVC
build_msvc() {
    local BUILD_DIR="build/build_tests/msvc/release"
    local OUTPUT_EXE="$BUILD_DIR/int128_algorithm_extracted_tests.exe"
    
    echo -e "${CYAN}Compilando con MSVC (cl.exe)...${NC}"
    mkdir -p "$BUILD_DIR"
    
    # Verificar que cl.exe este disponible
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
    
    # Ejecutar cl.exe con paths Windows
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

# Funcion para compilar con Intel
build_intel() {
    local BUILD_DIR="build/build_tests/intel/release"
    local OUTPUT_EXE="$BUILD_DIR/int128_algorithm_extracted_tests.exe"
    
    echo -e "${CYAN}Compilando con Intel C++...${NC}"
    mkdir -p "$BUILD_DIR"
    
    # Verificar que icpx este disponible
    if ! command -v icpx &> /dev/null && ! command -v icx &> /dev/null; then
            echo -e "${YELLOW}[WARNING] Intel C++ compiler (icpx/icx) no encontrado.${NC}"
            echo "  Para configurar Intel oneAPI:"
            echo "  source <(python scripts/setvars_intel.py)"
            return 1
    fi
    
    # Usar icpx si esta disponible, sino icx
    local INTEL_COMPILER="icpx"
    if ! command -v icpx &> /dev/null; then
        INTEL_COMPILER="icx"
    fi
    
    $INTEL_COMPILER -std=c++20 -O2 -Wall -Wextra -I "$INCLUDE_DIR" "$SRC_FILE" -o "$OUTPUT_EXE"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK] Compilacion Intel exitosa${NC}"
        echo "  Ejecutable: $OUTPUT_EXE"
        return 0
    else
        echo -e "${RED}[FAIL] Error en compilacion Intel${NC}"
        return 1
    fi
}

# Funcion principal
main() {
    local compiler="${1:-all}"
    local exit_code=0
    
    echo "================================"
    echo "  Build int128_algorithm_extracted_tests"
    echo "================================"
    echo ""
    
    case "$compiler" in
        gcc)
            build_gcc
            exit_code=$?
            ;;
        clang)
            build_clang
            exit_code=$?
            ;;
        msvc)
            build_msvc
            exit_code=$?
            ;;
        intel)
            build_intel
            exit_code=$?
            ;;
        all)
            build_gcc
            local gcc_result=$?
            
            build_clang
            local clang_result=$?
            
            build_msvc
            local msvc_result=$?
            
            build_intel
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
