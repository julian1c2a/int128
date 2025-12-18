#!/usr/bin/env bash
#
# run_benchmarks.bash
# Script para ejecutar los benchmarks compilados con GCC, Clang, MSVC e Intel
#
# Uso:
#   ./scripts/run_benchmarks.bash [gcc|clang|msvc|intel|all]
#

set -euo pipefail

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Directorios
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${ROOT_DIR}/build"
BENCHMARK_RESULTS_DIR="${ROOT_DIR}/benchmark_results"

# Ejecutables
BENCHMARK_EXEC_GCC="${BUILD_DIR}/uint128_benchmarks_gcc"
BENCHMARK_EXEC_CLANG="${BUILD_DIR}/uint128_benchmarks_clang"
BENCHMARK_EXEC_MSVC="${BUILD_DIR}/uint128_benchmarks_msvc.exe"
BENCHMARK_EXEC_INTEL="${BUILD_DIR}/uint128_benchmarks_intel"

# Crear directorio de resultados si no existe
mkdir -p "${BENCHMARK_RESULTS_DIR}"

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}Running uint128 Benchmarks${NC}"
echo -e "${BLUE}========================================${NC}"

# Función para ejecutar benchmark con GCC
run_gcc() {
    echo -e "\n${YELLOW}Running GCC benchmark...${NC}"
    
    if [ ! -f "${BENCHMARK_EXEC_GCC}" ]; then
        echo -e "${RED}Error: GCC benchmark not found${NC}"
        echo -e "${YELLOW}Hint: Run ./scripts/build_benchmarks.bash gcc first${NC}"
        return 1
    fi
    
    "${BENCHMARK_EXEC_GCC}"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ GCC benchmark completed${NC}"
        return 0
    else
        echo -e "${RED}✗ GCC benchmark failed${NC}"
        return 1
    fi
}

# Función para ejecutar benchmark con Clang
run_clang() {
    echo -e "\n${YELLOW}Running Clang benchmark...${NC}"
    
    if [ ! -f "${BENCHMARK_EXEC_CLANG}" ]; then
        echo -e "${RED}Error: Clang benchmark not found${NC}"
        echo -e "${YELLOW}Hint: Run ./scripts/build_benchmarks.bash clang first${NC}"
        return 1
    fi
    
    "${BENCHMARK_EXEC_CLANG}"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Clang benchmark completed${NC}"
        return 0
    else
        echo -e "${RED}✗ Clang benchmark failed${NC}"
        return 1
    fi
}

# Función para ejecutar benchmark con MSVC
run_msvc() {
    echo -e "\n${YELLOW}Running MSVC benchmark...${NC}"
    
    if [ ! -f "${BENCHMARK_EXEC_MSVC}" ]; then
        echo -e "${RED}Error: MSVC benchmark not found${NC}"
        echo -e "${YELLOW}Hint: Run ./scripts/build_benchmarks.bash msvc first${NC}"
        return 1
    fi
    
    "${BENCHMARK_EXEC_MSVC}"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ MSVC benchmark completed${NC}"
        return 0
    else
        echo -e "${RED}✗ MSVC benchmark failed${NC}"
        return 1
    fi
}

# Función para ejecutar con Intel
run_intel() {
    echo -e "${BLUE}Running Intel benchmark...${NC}"
    
    if [ ! -f "${BENCHMARK_EXEC_INTEL}" ]; then
        echo -e "${RED}Intel benchmark executable not found: ${BENCHMARK_EXEC_INTEL}${NC}"
        echo -e "${YELLOW}Run build_benchmarks.bash intel first${NC}"
        return 1
    fi
    
    "${BENCHMARK_EXEC_INTEL}"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Intel benchmark completed${NC}"
        return 0
    else
        echo -e "${RED}✗ Intel benchmark failed${NC}"
        return 1
    fi
}

# Función para ejecutar todos
run_all() {
    local success=0
    local failed=0
    
    if run_gcc; then
        ((success++))
    else
        ((failed++))
    fi
    
    if run_clang; then
        ((success++))
    else
        ((failed++))
    fi
    
    if run_msvc; then
        ((success++))
    else
        ((failed++))
    fi
    
    if run_intel; then
        ((success++))
    else
        ((failed++))
    fi
    
    echo -e "\n${BLUE}========================================${NC}"
    echo -e "${BLUE}Benchmark Summary${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo -e "${GREEN}Successful: ${success}${NC}"
    echo -e "${RED}Failed: ${failed}${NC}"
    
    # Si se ejecutaron todos, generar reporte consolidado
    if [ ${success} -gt 0 ]; then
        echo -e "\n${YELLOW}Generating consolidated report...${NC}"
        if [ -f "${SCRIPT_DIR}/aggregate_benchmark_results.py" ]; then
            python "${SCRIPT_DIR}/aggregate_benchmark_results.py"
        else
            echo -e "${YELLOW}Note: aggregate_benchmark_results.py not found${NC}"
        fi
    fi
    
    return ${failed}
}

# Main
main() {
    # Determinar qué ejecutar
    local target="${1:-all}"
    
    case "$target" in
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
            run_all
            ;;
        *)
            echo -e "${RED}Error: Unknown target '${target}'${NC}"
            echo -e "Usage: $0 [gcc|clang|msvc|intel|all]"
            exit 1
            ;;
    esac
}

main "$@"
