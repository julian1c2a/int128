#!/usr/bin/env bash
# ============================================================================
# Script wrapper para FEATURE=interop - Run Benchmarks (int128 alias)
# Ejecuta: benchmark_int128_vs_uint128
# ============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"

COMPILER="${1:-all}"
MODE="${2:-all}"
PRINT="${3:-}"

echo "========================================="
echo " FEATURE interop - Run Benchmarks (int128)"
echo "========================================="
echo ""

# Ejecutar benchmark int128_vs_uint128
echo "[RUN] Running benchmark_int128_vs_uint128..."
bash "$SCRIPT_DIR/run_benchmark_int128_vs_uint128.bash" "$COMPILER" "$MODE" "$PRINT"

echo ""
echo "[OK] FEATURE interop - Benchmarks run complete"
