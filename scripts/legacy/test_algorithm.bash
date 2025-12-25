#!/bin/bash

# Script combinado para compilar y ejecutar algorithm_extracted_tests
# Uso: ./test_algorithm.bash [gcc|clang|msvc|intel|all]

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
NC='\033[0m'

COMPILER="${1:-all}"

echo "========================================"
echo "  Test Suite: algorithm_extracted_tests"
echo "========================================"
echo ""

# Compilar
echo -e "${CYAN}[1/2] Compilando...${NC}"
"$SCRIPT_DIR/build_algorithm_tests.bash" "$COMPILER"

if [ $? -ne 0 ]; then
    echo -e "${RED}[FAIL] Compilacion fallida${NC}"
    exit 1
fi

echo ""
echo -e "${CYAN}[2/2] Ejecutando tests...${NC}"

# Ejecutar
"$SCRIPT_DIR/run_algorithm_tests.bash" "$COMPILER"

if [ $? -eq 0 ]; then
    echo ""
    echo -e "${GREEN}[OK] Suite completa exitosa${NC}"
    exit 0
else
    echo ""
    echo -e "${RED}[FAIL] Algunos tests fallaron${NC}"
    exit 1
fi
