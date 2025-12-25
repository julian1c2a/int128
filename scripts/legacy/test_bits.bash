#!/usr/bin/env bash
# Script completo para compilar y ejecutar tests de bits

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Colores para output
COLOR_RESET="\033[0m"
COLOR_BLUE="\033[34m"

echo -e "${COLOR_BLUE}========================================${COLOR_RESET}"
echo -e "${COLOR_BLUE}  Bits Tests (Build + Run)${COLOR_RESET}"
echo -e "${COLOR_BLUE}========================================${COLOR_RESET}"

# Compilar uint128_bits tests
"${SCRIPT_DIR}/build_uint128_bits_tests.bash" "$@" || exit 1

# Compilar int128_bits tests
"${SCRIPT_DIR}/build_int128_bits_tests.bash" "$@" || exit 1

# Ejecutar tests
"${SCRIPT_DIR}/run_bits_tests.bash" "$@" || exit 1

exit 0
