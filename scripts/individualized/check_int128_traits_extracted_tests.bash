#!/bin/bash
# Script para ejecutar tests de int128_traits
# Uso: ./check_int128_traits_extracted_tests.bash [gcc|clang|intel|msvc|all] [debug|release]

set -euo pipefail

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

# Directorios
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
PROJECT_DIR="${ROOT_DIR}"

# Validar argumentos
if [ $# -lt 1 ]; then
    echo -e "${RED}Error: Se requiere al menos un argumento${NC}"
    echo "Uso: $0 [gcc|clang|intel|msvc|all] [debug|release]"
    exit 1
fi

COMPILER="${1}"
COMPILER=$(echo "$COMPILER" | tr '[:upper:]' '[:lower:]')
MODE="${2:-release}"
MODE=$(echo "$MODE" | tr '[:upper:]' '[:lower:]')

# Validar modo
if [ "$MODE" != "debug" ] && [ "$MODE" != "release" ] && [ "$MODE" != "all" ]; then
    echo -e "${RED}Error: Modo inválido '$MODE'. Use 'debug', 'release' o 'all'${NC}"
    exit 1
fi

# Si MODE=all, ejecutar debug y release recursivamente
if [ "$MODE" = "all" ]; then
    echo -e "${BLUE}Ejecutando en modo 'all': debug + release${NC}"
    "$0" "$COMPILER" debug "${3:-}" && "$0" "$COMPILER" release "${3:-}"
    exit $?
fi

# Validar compilador
case "$COMPILER" in
    gcc|clang|intel|msvc|all)
        ;;
    *)
        echo -e "${RED}Error: Compilador inválido '$COMPILER'${NC}"
        echo "Uso: $0 [gcc|clang|intel|msvc|all] [debug|release]"
        exit 1
        ;;
esac

TIMESTAMP=$(date '+%Y-%m-%d %H:%M:%S')
echo -e "${BLUE}============================================================${NC}"
echo -e "${BLUE}  Ejecución de int128_traits tests${NC}"
echo -e "${BLUE}  Fecha: ${TIMESTAMP}${NC}"
echo -e "${BLUE}  Modo: ${MODE}${NC}"
echo -e "${BLUE}============================================================${NC}"

# Función auxiliar
should_run() {
    local comp=$1
    [ "$COMPILER" = "all" ] || [ "$COMPILER" = "$comp" ]
}

# -----------------------------------------------------------------------------
# GCC
# -----------------------------------------------------------------------------
run_gcc() {
    echo -e "\n${YELLOW}[GCC] Ejecutando tests...${NC}"
    
    local EXECUTABLE="build/build_tests/gcc/${MODE}/int128_traits_tests_gcc"
    
    if [ ! -f "${EXECUTABLE}" ]; then
        echo -e "${RED}  [WARN]  Ejecutable no encontrado: ${EXECUTABLE}${NC}"
        echo -e "      Ejecuta primero: ./scripts/build_int128_traits_extracted_tests.bash gcc ${MODE}"
        return 1
    fi
    
    echo -e "${CYAN}  Ejecutando: ${EXECUTABLE}${NC}"
    echo -e "${CYAN}  ================================================${NC}"
    
    if "${EXECUTABLE}"; then
        echo -e "${CYAN}  ================================================${NC}"
        echo -e "${GREEN}    [OK] Tests completados exitosamente${NC}"
        return 0
    else
        local exit_code=$?
        echo -e "${CYAN}  ================================================${NC}"
        echo -e "${RED}    [FAIL] Tests fallaron con código: ${exit_code}${NC}"
        return 1
    fi
}

# -----------------------------------------------------------------------------
# CLANG
# -----------------------------------------------------------------------------
run_clang() {
    echo -e "\n${YELLOW}[CLANG] Ejecutando tests...${NC}"
    
    local EXECUTABLE="build/build_tests/clang/${MODE}/int128_traits_tests_clang"
    
    if [ ! -f "${EXECUTABLE}" ]; then
        echo -e "${RED}  [WARN]  Ejecutable no encontrado: ${EXECUTABLE}${NC}"
        echo -e "      Ejecuta primero: ./scripts/build_int128_traits_extracted_tests.bash clang ${MODE}"
        return 1
    fi
    
    echo -e "${CYAN}  Ejecutando: ${EXECUTABLE}${NC}"
    echo -e "${CYAN}  ================================================${NC}"
    
    if "${EXECUTABLE}"; then
        echo -e "${CYAN}  ================================================${NC}"
        echo -e "${GREEN}    [OK] Tests completados exitosamente${NC}"
        return 0
    else
        local exit_code=$?
        echo -e "${CYAN}  ================================================${NC}"
        echo -e "${RED}    [FAIL] Tests fallaron con código: ${exit_code}${NC}"
        return 1
    fi
}

# -----------------------------------------------------------------------------
# INTEL
# -----------------------------------------------------------------------------
run_intel() {
    echo -e "\n${YELLOW}[INTEL] Ejecutando tests...${NC}"
    
    local EXECUTABLE="build/build_tests/intel/${MODE}/int128_traits_tests_intel.exe"
    
    if [ ! -f "${EXECUTABLE}" ]; then
        echo -e "${RED}  [WARN]  Ejecutable no encontrado: ${EXECUTABLE}${NC}"
        echo -e "      Ejecuta primero: ./scripts/build_int128_traits_extracted_tests.bash intel ${MODE}"
        return 1
    fi
    
    echo -e "${CYAN}  Ejecutando: ${EXECUTABLE}${NC}"
    echo -e "${CYAN}  ================================================${NC}"
    
    if "${EXECUTABLE}"; then
        echo -e "${CYAN}  ================================================${NC}"
        echo -e "${GREEN}    [OK] Tests completados exitosamente${NC}"
        return 0
    else
        local exit_code=$?
        echo -e "${CYAN}  ================================================${NC}"
        echo -e "${RED}    [FAIL] Tests fallaron con código: ${exit_code}${NC}"
        return 1
    fi
}

# -----------------------------------------------------------------------------
# MSVC
# -----------------------------------------------------------------------------
run_msvc() {
    echo -e "\n${YELLOW}[MSVC] Ejecutando tests...${NC}"
    
    local EXECUTABLE="build/build_tests/msvc/${MODE}/int128_traits_tests_msvc.exe"
    
    if [ ! -f "${EXECUTABLE}" ]; then
        echo -e "${RED}  [WARN]  Ejecutable no encontrado: ${EXECUTABLE}${NC}"
        echo -e "      Ejecuta primero: ./scripts/build_int128_traits_extracted_tests.bash msvc ${MODE}"
        return 1
    fi
    
    echo -e "${CYAN}  Ejecutando: ${EXECUTABLE}${NC}"
    echo -e "${CYAN}  ================================================${NC}"
    
    if "${EXECUTABLE}"; then
        echo -e "${CYAN}  ================================================${NC}"
        echo -e "${GREEN}    [OK] Tests completados exitosamente${NC}"
        return 0
    else
        local exit_code=$?
        echo -e "${CYAN}  ================================================${NC}"
        echo -e "${RED}    [FAIL] Tests fallaron con código: ${exit_code}${NC}"
        return 1
    fi
}

# -----------------------------------------------------------------------------
# MAIN
# -----------------------------------------------------------------------------
main() {
    local success=0
    local failed=0
    
    if should_run "gcc"; then
        if run_gcc; then
            ((success++)) || true
        else
            ((failed++)) || true
        fi
    fi
    
    if should_run "clang"; then
        if run_clang; then
            ((success++)) || true
        else
            ((failed++)) || true
        fi
    fi
    
    if should_run "intel"; then
        if run_intel; then
            ((success++)) || true
        else
            ((failed++)) || true
        fi
    fi
    
    if should_run "msvc"; then
        if run_msvc; then
            ((success++)) || true
        else
            ((failed++)) || true
        fi
    fi
    
    echo -e "\n${BLUE}============================================================${NC}"
    echo -e "${BLUE}  Resumen de ejecución${NC}"
    echo -e "${BLUE}============================================================${NC}"
    echo -e "${GREEN}  Exitosos: ${success}${NC}"
    echo -e "${RED}  Fallidos: ${failed}${NC}"
    echo -e "${BLUE}============================================================${NC}"
    
    return ${failed}
}

main

