#!/bin/bash

# Script combinado para compilar y ejecutar uint128_extracted_tests
# Uso: ./test_extracted.bash [gcc|clang|msvc|intel|all]

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Navegar al directorio raíz del proyecto
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$PROJECT_ROOT"

COMPILER="${1:-all}"

echo "========================================"
echo "  Test uint128_extracted_tests"
echo "  Compilador(es): $COMPILER"
echo "========================================"
echo ""

# Paso 1: Compilar
echo -e "${BLUE}>>> PASO 1: COMPILACIÓN${NC}"
echo ""
"$SCRIPT_DIR/build_extracted_tests.bash" "$COMPILER"
BUILD_EXIT_CODE=$?

if [ $BUILD_EXIT_CODE -ne 0 ]; then
    echo ""
    echo -e "${RED}❌ La compilación falló. No se ejecutarán los tests.${NC}"
    exit $BUILD_EXIT_CODE
fi

echo ""
echo ""

# Paso 2: Ejecutar tests
echo -e "${BLUE}>>> PASO 2: EJECUCIÓN DE TESTS${NC}"
echo ""
"$SCRIPT_DIR/run_uint128_extracted_tests.bash" "$COMPILER"
RUN_EXIT_CODE=$?

echo ""
echo "========================================"
if [ $RUN_EXIT_CODE -eq 0 ]; then
    echo -e "${GREEN}✅ TODO COMPLETADO EXITOSAMENTE${NC}"
else
    echo -e "${RED}❌ ALGUNOS TESTS FALLARON${NC}"
fi
echo "========================================"

exit $RUN_EXIT_CODE
