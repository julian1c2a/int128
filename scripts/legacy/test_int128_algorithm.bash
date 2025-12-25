#!/bin/bash

# Script combinado para compilar y ejecutar int128_algorithm tests
# Uso: ./test_int128_algorithm.bash [gcc|clang|msvc|intel|all]

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Navegar al directorio raiz del proyecto
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$PROJECT_ROOT"

compiler="${1:-all}"

echo "========================================"
echo "  Test Suite: int128_algorithm"
echo "========================================"
echo ""

# Compilar
./scripts/build_int128_algorithm_tests.bash "$compiler"

if [ $? -ne 0 ]; then
    echo -e "${RED}[FAIL] Compilacion fallida${NC}"
    exit 1
fi

echo ""
echo "========================================"
echo ""

# Ejecutar tests
./scripts/run_int128_algorithm_tests.bash "$compiler"

if [ $? -eq 0 ]; then
    echo ""
    echo "========================================"
    echo -e "${GREEN}[OK] Suite completa exitosa${NC}"
    echo "========================================"
    exit 0
else
    echo ""
    echo "========================================"
    echo -e "${RED}[FAIL] Algunos tests fallaron${NC}"
    echo "========================================"
    exit 1
fi
