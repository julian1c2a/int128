#!/bin/bash
# Script para ejecutar benchmarks de int128_traits
# Uso: ./run_int128_traits_extracted_benchs.bash [gcc|clang|intel|msvc|all] [debug|release]

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
if [ "$MODE" != "debug" ] && [ "$MODE" != "release" ]; then
    echo -e "${RED}Error: Modo inválido '$MODE'. Use 'debug' o 'release'${NC}"
    exit 1
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
echo -e "${BLUE}  Ejecución de int128_traits benchmarks${NC}"
echo -e "${BLUE}  Fecha: ${TIMESTAMP}${NC}"
echo -e "${BLUE}  Modo: ${MODE}${NC}"
echo -e "${BLUE}============================================================${NC}"

# Función auxiliar
should_run() {
    local comp=$1
    [ "$COMPILER" = "all" ] || [ "$COMPILER" = "$comp" ]
}

# Crear directorio de resultados
mkdir -p benchmark_results/{gcc,clang,intel,msvc}/{debug,release}

# -----------------------------------------------------------------------------
# GCC
# -----------------------------------------------------------------------------
run_gcc() {
    echo -e "\n${YELLOW}[GCC] Ejecutando benchmarks...${NC}"
    
    local EXECUTABLE="build/build_benchmarks/gcc/${MODE}/int128_traits_benchs_gcc"
    local OUTPUT_DIR="benchmark_results/gcc/${MODE}"
    local OUTPUT_FILE="${OUTPUT_DIR}/int128_traits_benchmarks_$(date '+%Y%m%d_%H%M%S').txt"
    
    if [ ! -f "${EXECUTABLE}" ]; then
        echo -e "${RED}  ⚠️  Ejecutable no encontrado: ${EXECUTABLE}${NC}"
        echo -e "      Ejecuta primero: ./scripts/build_int128_traits_extracted_benchs.bash gcc ${MODE}"
        return 1
    fi
    
    echo -e "${CYAN}  Ejecutando: ${EXECUTABLE}${NC}"
    echo -e "${CYAN}  Guardando resultados en: ${OUTPUT_FILE}${NC}"
    echo -e "${CYAN}  ================================================${NC}"
    
    if "${EXECUTABLE}" | tee "${OUTPUT_FILE}"; then
        echo -e "${CYAN}  ================================================${NC}"
        echo -e "${GREEN}    ✅ Benchmarks completados exitosamente${NC}"
        echo -e "${GREEN}    📊 Resultados guardados en: ${OUTPUT_FILE}${NC}"
        return 0
    else
        local exit_code=$?
        echo -e "${CYAN}  ================================================${NC}"
        echo -e "${RED}    ❌ Benchmarks fallaron con código: ${exit_code}${NC}"
        return 1
    fi
}

# -----------------------------------------------------------------------------
# CLANG
# -----------------------------------------------------------------------------
run_clang() {
    echo -e "\n${YELLOW}[CLANG] Ejecutando benchmarks...${NC}"
    
    local EXECUTABLE="build/build_benchmarks/clang/${MODE}/int128_traits_benchs_clang"
    local OUTPUT_DIR="benchmark_results/clang/${MODE}"
    local OUTPUT_FILE="${OUTPUT_DIR}/int128_traits_benchmarks_$(date '+%Y%m%d_%H%M%S').txt"
    
    if [ ! -f "${EXECUTABLE}" ]; then
        echo -e "${RED}  ⚠️  Ejecutable no encontrado: ${EXECUTABLE}${NC}"
        echo -e "      Ejecuta primero: ./scripts/build_int128_traits_extracted_benchs.bash clang ${MODE}"
        return 1
    fi
    
    echo -e "${CYAN}  Ejecutando: ${EXECUTABLE}${NC}"
    echo -e "${CYAN}  Guardando resultados en: ${OUTPUT_FILE}${NC}"
    echo -e "${CYAN}  ================================================${NC}"
    
    if "${EXECUTABLE}" | tee "${OUTPUT_FILE}"; then
        echo -e "${CYAN}  ================================================${NC}"
        echo -e "${GREEN}    ✅ Benchmarks completados exitosamente${NC}"
        echo -e "${GREEN}    📊 Resultados guardados en: ${OUTPUT_FILE}${NC}"
        return 0
    else
        local exit_code=$?
        echo -e "${CYAN}  ================================================${NC}"
        echo -e "${RED}    ❌ Benchmarks fallaron con código: ${exit_code}${NC}"
        return 1
    fi
}

# -----------------------------------------------------------------------------
# INTEL
# -----------------------------------------------------------------------------
run_intel() {
    echo -e "\n${YELLOW}[INTEL] Ejecutando benchmarks...${NC}"
    
    local EXECUTABLE="build/build_benchmarks/intel/${MODE}/int128_traits_benchs_intel.exe"
    local OUTPUT_DIR="benchmark_results/intel/${MODE}"
    local OUTPUT_FILE="${OUTPUT_DIR}/int128_traits_benchmarks_$(date '+%Y%m%d_%H%M%S').txt"
    
    if [ ! -f "${EXECUTABLE}" ]; then
        echo -e "${RED}  ⚠️  Ejecutable no encontrado: ${EXECUTABLE}${NC}"
        echo -e "      Ejecuta primero: ./scripts/build_int128_traits_extracted_benchs.bash intel ${MODE}"
        return 1
    fi
    
    echo -e "${CYAN}  Ejecutando: ${EXECUTABLE}${NC}"
    echo -e "${CYAN}  Guardando resultados en: ${OUTPUT_FILE}${NC}"
    echo -e "${CYAN}  ================================================${NC}"
    
    if "${EXECUTABLE}" | tee "${OUTPUT_FILE}"; then
        echo -e "${CYAN}  ================================================${NC}"
        echo -e "${GREEN}    ✅ Benchmarks completados exitosamente${NC}"
        echo -e "${GREEN}    📊 Resultados guardados en: ${OUTPUT_FILE}${NC}"
        return 0
    else
        local exit_code=$?
        echo -e "${CYAN}  ================================================${NC}"
        echo -e "${RED}    ❌ Benchmarks fallaron con código: ${exit_code}${NC}"
        return 1
    fi
}

# -----------------------------------------------------------------------------
# MSVC
# -----------------------------------------------------------------------------
run_msvc() {
    echo -e "\n${YELLOW}[MSVC] Ejecutando benchmarks...${NC}"
    
    local EXECUTABLE="build/build_benchmarks/msvc/${MODE}/int128_traits_benchs_msvc.exe"
    local OUTPUT_DIR="benchmark_results/msvc/${MODE}"
    local OUTPUT_FILE="${OUTPUT_DIR}/int128_traits_benchmarks_$(date '+%Y%m%d_%H%M%S').txt"
    
    if [ ! -f "${EXECUTABLE}" ]; then
        echo -e "${RED}  ⚠️  Ejecutable no encontrado: ${EXECUTABLE}${NC}"
        echo -e "      Ejecuta primero: ./scripts/build_int128_traits_extracted_benchs.bash msvc ${MODE}"
        return 1
    fi
    
    echo -e "${CYAN}  Ejecutando: ${EXECUTABLE}${NC}"
    echo -e "${CYAN}  Guardando resultados en: ${OUTPUT_FILE}${NC}"
    echo -e "${CYAN}  ================================================${NC}"
    
    if "${EXECUTABLE}" | tee "${OUTPUT_FILE}"; then
        echo -e "${CYAN}  ================================================${NC}"
        echo -e "${GREEN}    ✅ Benchmarks completados exitosamente${NC}"
        echo -e "${GREEN}    📊 Resultados guardados en: ${OUTPUT_FILE}${NC}"
        return 0
    else
        local exit_code=$?
        echo -e "${CYAN}  ================================================${NC}"
        echo -e "${RED}    ❌ Benchmarks fallaron con código: ${exit_code}${NC}"
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
            ((success++))
        else
            ((failed++))
        fi
    fi
    
    if should_run "clang"; then
        if run_clang; then
            ((success++))
        else
            ((failed++))
        fi
    fi
    
    if should_run "intel"; then
        if run_intel; then
            ((success++))
        else
            ((failed++))
        fi
    fi
    
    if should_run "msvc"; then
        if run_msvc; then
            ((success++))
        else
            ((failed++))
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

