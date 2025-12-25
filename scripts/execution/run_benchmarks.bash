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
BENCHMARK_RESULTS_DIR="${ROOT_DIR}/bench_results"

# Ejecutables - uint128_t
BENCHMARK_EXEC_GCC_UINT128="${BUILD_DIR}/uint128_benchmarks_gcc"
BENCHMARK_EXEC_CLANG_UINT128="${BUILD_DIR}/uint128_benchmarks_clang"
BENCHMARK_EXEC_MSVC_UINT128="${BUILD_DIR}/uint128_benchmarks_msvc.exe"
BENCHMARK_EXEC_INTEL_UINT128="${BUILD_DIR}/uint128_benchmarks_intel"

# Ejecutables - int128_t
BENCHMARK_EXEC_GCC_INT128="${BUILD_DIR}/int128_benchmarks_gcc"
BENCHMARK_EXEC_CLANG_INT128="${BUILD_DIR}/int128_benchmarks_clang"
BENCHMARK_EXEC_MSVC_INT128="${BUILD_DIR}/int128_benchmarks_msvc.exe"
BENCHMARK_EXEC_INTEL_INT128="${BUILD_DIR}/int128_benchmarks_intel"

# Crear directorio de resultados si no existe
mkdir -p "${BENCHMARK_RESULTS_DIR}"

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}Running int128/uint128 Benchmarks${NC}"
echo -e "${BLUE}========================================${NC}"

# Función para ejecutar benchmark con GCC
run_gcc() {
    echo -e "\n${YELLOW}Running GCC benchmarks...${NC}"
    
    local success=0
    local failed=0
    
    # uint128_t
    if [ -f "${BENCHMARK_EXEC_GCC_UINT128}" ]; then
        echo -e "${BLUE}  Running uint128_t benchmark...${NC}"
        "${BENCHMARK_EXEC_GCC_UINT128}"
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}  [OK] uint128_t benchmark completed${NC}"
            ((success++))
        else
            echo -e "${RED}  [ERROR] uint128_t benchmark failed${NC}"
            ((failed++))
        fi
    else
        echo -e "${YELLOW}  ⚠ uint128_t benchmark not found (skipping)${NC}"
    fi
    
    # int128_t
    if [ -f "${BENCHMARK_EXEC_GCC_INT128}" ]; then
        echo -e "${BLUE}  Running int128_t benchmark...${NC}"
        "${BENCHMARK_EXEC_GCC_INT128}"
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}  [OK] int128_t benchmark completed${NC}"
            ((success++))
        else
            echo -e "${RED}  [ERROR] int128_t benchmark failed${NC}"
            ((failed++))
        fi
    else
        echo -e "${YELLOW}  ⚠ int128_t benchmark not found (skipping)${NC}"
    fi
    
    if [ $success -eq 0 ]; then
        echo -e "${RED}Error: No GCC benchmarks found${NC}"
        echo -e "${YELLOW}Hint: Run ./scripts/build_*_extracted_benchs.bash gcc first${NC}"
        return 1
    fi
    
    echo -e "${GREEN}[OK] GCC benchmarks complete: ${success} successful, ${failed} failed${NC}"
    return ${failed}
}

# Función para ejecutar benchmark con Clang
run_clang() {
    echo -e "\n${YELLOW}Running Clang benchmarks...${NC}"
    
    local success=0
    local failed=0
    
    # uint128_t
    if [ -f "${BENCHMARK_EXEC_CLANG_UINT128}" ]; then
        echo -e "${BLUE}  Running uint128_t benchmark...${NC}"
        "${BENCHMARK_EXEC_CLANG_UINT128}"
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}  [OK] uint128_t benchmark completed${NC}"
            ((success++))
        else
            echo -e "${RED}  [ERROR] uint128_t benchmark failed${NC}"
            ((failed++))
        fi
    else
        echo -e "${YELLOW}  ⚠ uint128_t benchmark not found (skipping)${NC}"
    fi
    
    # int128_t
    if [ -f "${BENCHMARK_EXEC_CLANG_INT128}" ]; then
        echo -e "${BLUE}  Running int128_t benchmark...${NC}"
        "${BENCHMARK_EXEC_CLANG_INT128}"
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}  [OK] int128_t benchmark completed${NC}"
            ((success++))
        else
            echo -e "${RED}  [ERROR] int128_t benchmark failed${NC}"
            ((failed++))
        fi
    else
        echo -e "${YELLOW}  ⚠ int128_t benchmark not found (skipping)${NC}"
    fi
    
    if [ $success -eq 0 ]; then
        echo -e "${RED}Error: No Clang benchmarks found${NC}"
        echo -e "${YELLOW}Hint: Run ./scripts/build_*_extracted_benchs.bash clang first${NC}"
        return 1
    fi
    
    echo -e "${GREEN}[OK] Clang benchmarks complete: ${success} successful, ${failed} failed${NC}"
    return ${failed}
}

# Función para ejecutar benchmark con MSVC
run_msvc() {
    echo -e "\n${YELLOW}Running MSVC benchmarks...${NC}"
    
    local success=0
    local failed=0
    
    # uint128_t
    if [ -f "${BENCHMARK_EXEC_MSVC_UINT128}" ]; then
        echo -e "${BLUE}  Running uint128_t benchmark...${NC}"
        "${BENCHMARK_EXEC_MSVC_UINT128}"
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}  [OK] uint128_t benchmark completed${NC}"
            ((success++))
        else
            echo -e "${RED}  [ERROR] uint128_t benchmark failed${NC}"
            ((failed++))
        fi
    else
        echo -e "${YELLOW}  ⚠ uint128_t benchmark not found (skipping)${NC}"
    fi
    
    # int128_t
    if [ -f "${BENCHMARK_EXEC_MSVC_INT128}" ]; then
        echo -e "${BLUE}  Running int128_t benchmark...${NC}"
        "${BENCHMARK_EXEC_MSVC_INT128}"
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}  [OK] int128_t benchmark completed${NC}"
            ((success++))
        else
            echo -e "${RED}  [ERROR] int128_t benchmark failed${NC}"
            ((failed++))
        fi
    else
        echo -e "${YELLOW}  ⚠ int128_t benchmark not found (skipping)${NC}"
    fi
    
    if [ $success -eq 0 ]; then
        echo -e "${RED}Error: No MSVC benchmarks found${NC}"
        echo -e "${YELLOW}Hint: Run ./scripts/build_*_extracted_benchs.bash msvc first${NC}"
        return 1
    fi
    
    echo -e "${GREEN}[OK] MSVC benchmarks complete: ${success} successful, ${failed} failed${NC}"
    return ${failed}
}

# Función para ejecutar con Intel
run_intel() {
    echo -e "\n${YELLOW}Running Intel benchmarks...${NC}"
    
    local success=0
    local failed=0
    
    # uint128_t
    if [ -f "${BENCHMARK_EXEC_INTEL_UINT128}" ]; then
        echo -e "${BLUE}  Running uint128_t benchmark...${NC}"
        "${BENCHMARK_EXEC_INTEL_UINT128}"
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}  [OK] uint128_t benchmark completed${NC}"
            ((success++))
        else
            echo -e "${RED}  [ERROR] uint128_t benchmark failed${NC}"
            ((failed++))
        fi
    else
        echo -e "${YELLOW}  ⚠ uint128_t benchmark not found (skipping)${NC}"
    fi
    
    # int128_t
    if [ -f "${BENCHMARK_EXEC_INTEL_INT128}" ]; then
        echo -e "${BLUE}  Running int128_t benchmark...${NC}"
        "${BENCHMARK_EXEC_INTEL_INT128}"
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}  [OK] int128_t benchmark completed${NC}"
            ((success++))
        else
            echo -e "${RED}  [ERROR] int128_t benchmark failed${NC}"
            ((failed++))
        fi
    else
        echo -e "${YELLOW}  ⚠ int128_t benchmark not found (skipping)${NC}"
    fi
    
    if [ $success -eq 0 ]; then
        echo -e "${RED}Error: No Intel benchmarks found${NC}"
        echo -e "${YELLOW}Hint: Run ./scripts/build_*_extracted_benchs.bash intel first${NC}"
        return 1
    fi
    
    echo -e "${GREEN}[OK] Intel benchmarks complete: ${success} successful, ${failed} failed${NC}"
    return ${failed}
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
    echo -e "${BLUE}Benchmark Execution Summary${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo -e "${GREEN}Compilers successful: ${success}/4${NC}"
    echo -e "${RED}Compilers failed: ${failed}/4${NC}"
    echo -e "${YELLOW}(Each compiler runs uint128_t + int128_t benchmarks)${NC}"
    
    # Si se ejecutaron todos, generar reporte consolidado
    if [ ${success} -gt 0 ]; then
        echo -e "\n${YELLOW}Generating consolidated report...${NC}"
        if [ -f "${SCRIPT_DIR}/aggregate_bench_results.py" ]; then
            python "${SCRIPT_DIR}/aggregate_bench_results.py"
        else
            echo -e "${YELLOW}Note: aggregate_bench_results.py not found${NC}"
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
