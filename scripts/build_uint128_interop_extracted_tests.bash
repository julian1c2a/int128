#!/usr/bin/env bash
# ============================================================================
# Script wrapper para FEATURE=interop - Tests
# Ejecuta: test_interoperability_uint128_int128.cpp
# ============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"

COMPILER="${1:-all}"
MODE="${2:-all}"
PRINT="${3:-}"

echo "========================================="
echo " FEATURE interop - Tests (uint128)"
echo "========================================="
echo ""

# Ejecutar tests de interoperabilidad
echo "[TEST] Building test_interoperability..."
bash "$SCRIPT_DIR/build_test_interoperability.bash" "$COMPILER" "$MODE" "$PRINT"

echo ""
echo "[OK] FEATURE interop - Tests build complete"
