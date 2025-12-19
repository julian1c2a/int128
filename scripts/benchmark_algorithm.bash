#!/usr/bin/env bash
# Script completo para compilar y ejecutar benchmarks de algoritmos

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Colores para output
COLOR_RESET="\033[0m"
COLOR_BLUE="\033[34m"

echo -e "${COLOR_BLUE}========================================${COLOR_RESET}"
echo -e "${COLOR_BLUE}  Algorithm Benchmarks (Build + Run)${COLOR_RESET}"
echo -e "${COLOR_BLUE}========================================${COLOR_RESET}"

# Compilar uint128 benchmarks
"${SCRIPT_DIR}/build_uint128_algo_bench.bash" "$@" || exit 1

# Compilar int128 benchmarks
"${SCRIPT_DIR}/build_int128_algo_bench.bash" "$@" || exit 1

# Ejecutar benchmarks
"${SCRIPT_DIR}/run_algorithm_benchmarks.bash" "$@" || exit 1

exit 0
