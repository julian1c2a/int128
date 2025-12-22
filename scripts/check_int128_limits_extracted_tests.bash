#!/usr/bin/env bash
# Script para ejecutar los tests de int128_limits_extracted_tests

echo "========================================="
echo " EJECUTANDO: int128_limits_extracted_tests"
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
    echo "───────────────────────────────────────"
    echo "🧪 $compiler ($mode)"
    echo "───────────────────────────────────────"
    
    if [ ! -f "$executable" ]; then
        echo -e "${YELLOW}⚠️  Ejecutable no encontrado: $executable${NC}"
        return 1
    fi
    
    # Ejecutar el test
    if "$executable"; then
        echo -e "${GREEN}✅ TESTS PASADOS${NC}"
        return 0
    else
        echo -e "${RED}❌ TESTS FALLIDOS${NC}"
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
if run_test "GCC" "Debug" "../build/int128_limits_extracted_tests/gcc/debug/int128_limits_extracted_tests"; then
    ((passed++))
fi

((total++))
if run_test "GCC" "Release" "../build/int128_limits_extracted_tests/gcc/release/int128_limits_extracted_tests"; then
    ((passed++))
fi

# ---------------------------------------
# 2. Clang
# ---------------------------------------
((total++))
if run_test "Clang" "Debug" "../build/int128_limits_extracted_tests/clang/debug/int128_limits_extracted_tests"; then
    ((passed++))
fi

((total++))
if run_test "Clang" "Release" "../build/int128_limits_extracted_tests/clang/release/int128_limits_extracted_tests"; then
    ((passed++))
fi

# ---------------------------------------
# 3. Intel ICX
# ---------------------------------------
if [ -f "../build/int128_limits_extracted_tests/intel/debug/int128_limits_extracted_tests.exe" ]; then
    ((total++))
    if run_test "Intel ICX" "Debug" "../build/int128_limits_extracted_tests/intel/debug/int128_limits_extracted_tests.exe"; then
        ((passed++))
    fi
    
    ((total++))
    if run_test "Intel ICX" "Release" "../build/int128_limits_extracted_tests/intel/release/int128_limits_extracted_tests.exe"; then
        ((passed++))
    fi
fi

# ---------------------------------------
# 4. MSVC
# ---------------------------------------
if [ -f "../build/int128_limits_extracted_tests/msvc/debug/int128_limits_extracted_tests.exe" ]; then
    ((total++))
    if run_test "MSVC" "Debug" "../build/int128_limits_extracted_tests/msvc/debug/int128_limits_extracted_tests.exe"; then
        ((passed++))
    fi
    
    ((total++))
    if run_test "MSVC" "Release" "../build/int128_limits_extracted_tests/msvc/release/int128_limits_extracted_tests.exe"; then
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
    echo -e "${GREEN}✅ TODOS LOS TESTS PASARON${NC}"
    exit 0
else
    echo -e "${RED}❌ ALGUNOS TESTS FALLARON${NC}"
    exit 1
fi
