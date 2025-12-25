#!/usr/bin/env bash
# ============================================================================
# Script wrapper para FEATURE=interop - Benchmarks (int128 alias)
# Ejecuta: benchmark_int128_vs_uint128.cpp
# ============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"

COMPILER="${1:-all}"
MODE="${2:-all}"
PRINT="${3:-}"

echo "========================================="
echo " FEATURE interop - Benchmarks (int128)"
echo "========================================="
echo ""

# Ejecutar benchmark int128_vs_uint128
echo "📊 Building benchmark_int128_vs_uint128..."
bash "$SCRIPT_DIR/build_benchmark_int128_vs_uint128.bash" "$COMPILER" "$MODE" "$PRINT"

echo ""
echo "✅ FEATURE interop - Benchmarks build complete"
