#!/usr/bin/env bash
# Script para ejecutar los tests de uint128_limits_extracted_tests

# Detectar directorio del script y directorio raíz del proyecto
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"

echo "========================================="
echo " EJECUTANDO: uint128_limits_extracted_tests"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Función para ejecutar un test
run_test() {
    local compiler=$1
    local mode=$2
    local executable=$3
    
    echo ""
    echo "---------------------------------------"
    echo "[TEST] $compiler ($mode)"
    echo "---------------------------------------"
    
    if [ ! -f "$executable" ]; then
        echo -e "${YELLOW}[WARN]  Ejecutable no encontrado: $executable${NC}"
        return 1
    fi
    
    # Ejecutar el test
    if "$executable"; then
        echo -e "${GREEN}[OK] TESTS PASADOS${NC}"
        return 0
    else
        echo -e "${RED}[FAIL] TESTS FALLIDOS${NC}"
        return 1
    fi
}

# Contadores
total=0
passed=0

# ---------------------------------------
# 1. GCC
# ---------------------------------------
((total++))
if run_test "GCC" "Debug" "$PROJECT_ROOT/build/uint128_limits_extracted_tests/gcc/debug/uint128_limits_extracted_tests"; then
    ((passed++))
fi

((total++))
if run_test "GCC" "Release" "$PROJECT_ROOT/build/uint128_limits_extracted_tests/gcc/release/uint128_limits_extracted_tests"; then
    ((passed++))
fi

# ---------------------------------------
# 2. Clang
# ---------------------------------------
((total++))
if run_test "Clang" "Debug" "$PROJECT_ROOT/build/uint128_limits_extracted_tests/clang/debug/uint128_limits_extracted_tests"; then
    ((passed++))
fi

((total++))
if run_test "Clang" "Release" "$PROJECT_ROOT/build/uint128_limits_extracted_tests/clang/release/uint128_limits_extracted_tests"; then
    ((passed++))
fi

# ---------------------------------------
# 3. Intel ICX
# ---------------------------------------
if [ -f "../build/uint128_limits_extracted_tests/intel/debug/uint128_limits_extracted_tests.exe" ]; then
    ((total++))
    if run_test "Intel ICX" "Debug" "../build/uint128_limits_extracted_tests/intel/debug/uint128_limits_extracted_tests.exe"; then
        ((passed++))
    fi
    
    ((total++))
    if run_test "Intel ICX" "Release" "../build/uint128_limits_extracted_tests/intel/release/uint128_limits_extracted_tests.exe"; then
        ((passed++))
    fi
fi

# ---------------------------------------
# 4. MSVC
# ---------------------------------------
if [ -f "../build/build_tests/msvc/debug/uint128_limits_tests_msvc.exe" ]; then
    ((total++))
    if run_test "MSVC" "Debug" "../build/build_tests/msvc/debug/uint128_limits_tests_msvc.exe"; then
        ((passed++))
    fi
    
    ((total++))
    if run_test "MSVC" "Release" "../build/build_tests/msvc/release/uint128_limits_tests_msvc.exe"; then
        ((passed++))
    fi
fi

# ---------------------------------------
# Resumen final
# ---------------------------------------
echo ""
echo "========================================="
echo " RESUMEN: $passed/$total configuraciones OK"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="

if [ $passed -eq $total ]; then
    echo -e "${GREEN}[OK] TODOS LOS TESTS PASARON${NC}"
    exit 0
else
    echo -e "${RED}[FAIL] ALGUNOS TESTS FALLARON${NC}"
    exit 1
fi
