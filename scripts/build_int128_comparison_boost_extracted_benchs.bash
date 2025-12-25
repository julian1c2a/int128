#!/bin/bash

# Wrapper: int128 comparison_boost (redirige a uint128)
# El benchmark está implementado para uint128_t, pero incluye comparación con __int128 signed

echo "ℹ️  Nota: comparison_boost está implementado para uint128_t"
echo "   Redirigiendo a: build_uint128_comparison_boost_extracted_benchs.bash"
echo ""

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
bash "$SCRIPT_DIR/build_uint128_comparison_boost_extracted_benchs.bash" "$@"
