#!/usr/bin/env bash
# ============================================================================
# Script wrapper para FEATURE=interop - Check Tests
# Ejecuta: uint128_interop_tests_uint128_int128
# ============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"

COMPILER="${1:-all}"
MODE="${2:-all}"
PRINT="${3:-}"

echo "========================================="
echo " FEATURE interop - Check Tests (uint128)"
echo "========================================="
echo ""

# Ejecutar tests de interoperabilidad
echo "[OK] Running test_interoperability..."
bash "$SCRIPT_DIR/run_test_interoperability.bash" "$COMPILER" "$MODE" "$PRINT"

echo ""
echo "[OK] FEATURE interop - Tests run complete"
