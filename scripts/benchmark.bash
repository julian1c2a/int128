#!/usr/bin/env bash
#
# benchmark.bash
# Script wrapper para compilar y ejecutar todos los benchmarks
#
# Uso:
#   ./scripts/benchmark.bash [build|run|all|clean]
#

set -euo pipefail

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Directorios
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

# Scripts
BUILD_SCRIPT="${SCRIPT_DIR}/build_benchmarks.bash"
RUN_SCRIPT="${SCRIPT_DIR}/run_benchmarks.bash"
AGGREGATE_SCRIPT="${SCRIPT_DIR}/aggregate_benchmark_results.py"

echo -e "${CYAN}========================================${NC}"
echo -e "${CYAN}uint128_t Benchmark Suite${NC}"
echo -e "${CYAN}========================================${NC}"

# Función para compilar
do_build() {
    echo -e "\n${BLUE}Building benchmarks...${NC}"
    if [ -f "${BUILD_SCRIPT}" ]; then
        bash "${BUILD_SCRIPT}" all
        return $?
    else
        echo -e "${RED}Error: Build script not found${NC}"
        return 1
    fi
}

# Función para ejecutar
do_run() {
    echo -e "\n${BLUE}Running benchmarks...${NC}"
    if [ -f "${RUN_SCRIPT}" ]; then
        bash "${RUN_SCRIPT}" all
        return $?
    else
        echo -e "${RED}Error: Run script not found${NC}"
        return 1
    fi
}

# Función para limpiar
do_clean() {
    echo -e "\n${YELLOW}Cleaning benchmark executables...${NC}"
    rm -f "${ROOT_DIR}/build/uint128_benchmarks_gcc"
    rm -f "${ROOT_DIR}/build/uint128_benchmarks_clang"
    rm -f "${ROOT_DIR}/build/uint128_benchmarks_msvc.exe"
    echo -e "${GREEN}✓ Cleaned${NC}"
}

# Función para hacer todo
do_all() {
    local success=true
    
    if ! do_build; then
        echo -e "${RED}Build failed, stopping${NC}"
        return 1
    fi
    
    if ! do_run; then
        echo -e "${RED}Run failed${NC}"
        success=false
    fi
    
    if $success; then
        echo -e "\n${GREEN}========================================${NC}"
        echo -e "${GREEN}All benchmarks completed successfully!${NC}"
        echo -e "${GREEN}========================================${NC}"
        return 0
    else
        return 1
    fi
}

# Main
main() {
    local command="${1:-all}"
    
    case "$command" in
        build)
            do_build
            ;;
        run)
            do_run
            ;;
        clean)
            do_clean
            ;;
        all)
            do_all
            ;;
        *)
            echo -e "${RED}Error: Unknown command '${command}'${NC}"
            echo -e "Usage: $0 [build|run|all|clean]"
            exit 1
            ;;
    esac
}

main "$@"
