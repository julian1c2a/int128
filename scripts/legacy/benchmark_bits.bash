#!/usr/bin/env bash
# Script wrapper para compilar y ejecutar benchmarks de bits

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Colores para output
COLOR_RESET="\033[0m"
COLOR_BLUE="\033[34m"

echo -e "${COLOR_BLUE}========================================${COLOR_RESET}"
echo -e "${COLOR_BLUE}  Bits Benchmarks (Build + Run)${COLOR_RESET}"
echo -e "${COLOR_BLUE}========================================${COLOR_RESET}"

# Compilar benchmarks
"${SCRIPT_DIR}/build_uint128_bits_bench.bash" "$@"
"${SCRIPT_DIR}/build_int128_bits_bench.bash" "$@"

# Ejecutar benchmarks
"${SCRIPT_DIR}/run_bits_benchmarks.bash" "$@"
