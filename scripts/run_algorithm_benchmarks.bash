#!/usr/bin/env bash
# Script para ejecutar benchmarks de algoritmos con diferentes compiladores

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build/build_benchmarks"

# Colores para output
COLOR_RESET="\033[0m"
COLOR_GREEN="\033[32m"
COLOR_RED="\033[31m"
COLOR_YELLOW="\033[33m"
COLOR_BLUE="\033[34m"
COLOR_CYAN="\033[36m"

echo -e "${COLOR_BLUE}========================================${COLOR_RESET}"
echo -e "${COLOR_BLUE}  Run Algorithm Benchmarks${COLOR_RESET}"
echo -e "${COLOR_BLUE}========================================${COLOR_RESET}"

# Función para ejecutar benchmark de uint128 con un compilador
run_uint128_benchmark() {
    local compiler="${1}"
    local mode="${2:-release}"
    local bench_exe="${BUILD_DIR}/${compiler}/${mode}/uint128_algorithm_benchmarks.exe"
    
    if [[ ! -f "${bench_exe}" ]]; then
        echo -e "${COLOR_YELLOW}[WARNING] Benchmark no encontrado: ${bench_exe}${COLOR_RESET}"
        return 1
    fi
    
    echo -e "\n${COLOR_CYAN}[RUN] uint128_algorithm_benchmarks (${compiler}, ${mode})${COLOR_RESET}"
    
    if "${bench_exe}"; then
        echo -e "${COLOR_GREEN}[OK] Benchmark completado${COLOR_RESET}"
        return 0
    else
        echo -e "${COLOR_RED}[FAIL] Benchmark fallido${COLOR_RESET}"
        return 1
    fi
}

# Función para ejecutar benchmark de int128 con un compilador
run_int128_benchmark() {
    local compiler="${1}"
    local mode="${2:-release}"
    local bench_exe="${BUILD_DIR}/${compiler}/${mode}/int128_algorithm_benchmarks.exe"
    
    if [[ ! -f "${bench_exe}" ]]; then
        echo -e "${COLOR_YELLOW}[WARNING] Benchmark no encontrado: ${bench_exe}${COLOR_RESET}"
        return 1
    fi
    
    echo -e "\n${COLOR_CYAN}[RUN] int128_algorithm_benchmarks (${compiler}, ${mode})${COLOR_RESET}"
    
    if "${bench_exe}"; then
        echo -e "${COLOR_GREEN}[OK] Benchmark completado${COLOR_RESET}"
        return 0
    else
        echo -e "${COLOR_RED}[FAIL] Benchmark fallido${COLOR_RESET}"
        return 1
    fi
}

# Función principal
main() {
    local compiler="${1:-all}"
    local mode="${2:-release}"
    
    if [[ ! "${mode}" =~ ^(release|debug)$ ]]; then
        echo -e "${COLOR_RED}[ERROR] Modo invalido: ${mode}${COLOR_RESET}"
        echo "Uso: $0 [compiler] [mode]"
        echo "  compiler: gcc, clang, msvc, intel, all (default: all)"
        echo "  mode: release, debug (default: release)"
        exit 1
    fi
    
    local failed=0
    
    case "${compiler}" in
        gcc|clang|msvc|intel)
            run_uint128_benchmark "${compiler}" "${mode}" || failed=$((failed + 1))
            run_int128_benchmark "${compiler}" "${mode}" || failed=$((failed + 1))
            ;;
        all)
            for comp in gcc clang msvc intel; do
                run_uint128_benchmark "${comp}" "${mode}" || failed=$((failed + 1))
                run_int128_benchmark "${comp}" "${mode}" || failed=$((failed + 1))
            done
            ;;
        *)
            echo -e "${COLOR_RED}[ERROR] Compilador desconocido: ${compiler}${COLOR_RESET}"
            echo "Uso: $0 [compiler] [mode]"
            echo "  compiler: gcc, clang, msvc, intel, all (default: all)"
            echo "  mode: release, debug (default: release)"
            exit 1
            ;;
    esac
    
    echo -e "\n${COLOR_BLUE}========================================${COLOR_RESET}"
    if [[ ${failed} -eq 0 ]]; then
        echo -e "${COLOR_GREEN}[OK] Todos los benchmarks completados${COLOR_RESET}"
    else
        echo -e "${COLOR_RED}[FAIL] ${failed} benchmark(s) fallido(s)${COLOR_RESET}"
    fi
    echo -e "${COLOR_BLUE}========================================${COLOR_RESET}"
    
    exit ${failed}
}

main "$@"
