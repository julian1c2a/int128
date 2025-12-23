#!/usr/bin/env bash
# Script para ejecutar los tests de int128_concepts_extracted_tests

set -u

# Detectar directorio del script y directorio raíz del proyecto
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"

# Validar argumentos
if [ $# -lt 2 ]; then
    echo "❌ ERROR: Se requieren al menos 2 argumentos"
    echo "Uso: $0 [compiler] [mode] [print]"
    echo "  compiler: intel, msvc, gcc, clang, all"
    echo "  mode: debug, release, all"
    echo "  print: opcional, cualquier valor para activar"
    exit 1
fi

# Compilador y modo
COMPILER="${1}"
MODE="${2}"
PRINT_MODE="${3:-}"

COMPILER=$(echo "$COMPILER" | tr '[:upper:]' '[:lower:]')
MODE=$(echo "$MODE" | tr '[:upper:]' '[:lower:]')

# Validar compilador
if [[ ! "$COMPILER" =~ ^(intel|msvc|gcc|clang|all)$ ]]; then
    echo "❌ ERROR: Compilador inválido: $COMPILER"
    echo "Compiladores válidos: intel, msvc, gcc, clang, all"
    exit 1
fi

# Validar modo
if [[ ! "$MODE" =~ ^(debug|release|all)$ ]]; then
    echo "❌ ERROR: Modo inválido: $MODE"
    echo "Modos válidos: debug, release, all"
    exit 1
fi

echo "========================================="
echo " EJECUTANDO: int128_concepts_extracted_tests"
echo " Compilador: $COMPILER"
echo " Modo: $MODE"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Si está activado el modo print, crear directorio para resultados
if [ -n "$PRINT_MODE" ]; then
    RESULTS_DIR="$PROJECT_ROOT/build/build_tests_results"
fi

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
        echo -e "${YELLOW}⚠️  Ejecutable no encontrado${NC}"
        return 1
    fi
    
    # Ejecutar el test
    if [ -n "$PRINT_MODE" ]; then
        mkdir -p "$RESULTS_DIR/${compiler,,}/${mode,,}"
        local timestamp=$(date '+%Y%m%d_%H%M%S')
        local output_file="$RESULTS_DIR/${compiler,,}/${mode,,}/int128_concepts_tests_${timestamp}.txt"
        
        if "$executable" | tee "$output_file"; then
            echo -e "${GREEN}✅ TESTS PASADOS${NC}"
            echo "   Salida guardada en: $output_file"
            return 0
        else
            echo -e "${RED}❌ TESTS FALLIDOS${NC}"
            echo "   Salida guardada en: $output_file"
            return 1
        fi
    else
        if "$executable"; then
            echo -e "${GREEN}✅ TESTS PASADOS${NC}"
            return 0
        else
            echo -e "${RED}❌ TESTS FALLIDOS${NC}"
            return 1
        fi
    fi
}

# Contadores
total=0
passed=0

# ---------------------------------------
# GCC
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "gcc" ]; then
    if [ "$MODE" = "all" ] || [ "$MODE" = "debug" ]; then
        ((total++))
        if run_test "GCC" "Debug" "$PROJECT_ROOT/build/build_tests/gcc/debug/int128_concepts_extracted_tests.exe"; then
            ((passed++))
        fi
    fi
    
    if [ "$MODE" = "all" ] || [ "$MODE" = "release" ]; then
        ((total++))
        if run_test "GCC" "Release" "$PROJECT_ROOT/build/build_tests/gcc/release/int128_concepts_extracted_tests.exe"; then
            ((passed++))
        fi
    fi
fi

# ---------------------------------------
# Clang
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "clang" ]; then
    if [ "$MODE" = "all" ] || [ "$MODE" = "debug" ]; then
        ((total++))
        if run_test "Clang" "Debug" "$PROJECT_ROOT/build/build_tests/clang/debug/int128_concepts_extracted_tests.exe"; then
            ((passed++))
        fi
    fi
    
    if [ "$MODE" = "all" ] || [ "$MODE" = "release" ]; then
        ((total++))
        if run_test "Clang" "Release" "$PROJECT_ROOT/build/build_tests/clang/release/int128_concepts_extracted_tests.exe"; then
            ((passed++))
        fi
    fi
fi

# ---------------------------------------
# Intel
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "intel" ]; then
    if [ "$MODE" = "all" ] || [ "$MODE" = "debug" ]; then
        if [ -f "$PROJECT_ROOT/build/build_tests/intel/debug/int128_concepts_extracted_tests.exe" ]; then
            ((total++))
            if run_test "Intel" "Debug" "$PROJECT_ROOT/build/build_tests/intel/debug/int128_concepts_extracted_tests.exe"; then
                ((passed++))
            fi
        fi
    fi
    
    if [ "$MODE" = "all" ] || [ "$MODE" = "release" ]; then
        if [ -f "$PROJECT_ROOT/build/build_tests/intel/release/int128_concepts_extracted_tests.exe" ]; then
            ((total++))
            if run_test "Intel" "Release" "$PROJECT_ROOT/build/build_tests/intel/release/int128_concepts_extracted_tests.exe"; then
                ((passed++))
            fi
        fi
    fi
fi

# ---------------------------------------
# MSVC
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "msvc" ]; then
    if [ "$MODE" = "all" ] || [ "$MODE" = "debug" ]; then
        if [ -f "$PROJECT_ROOT/build/build_tests/msvc/debug/int128_concepts_extracted_tests.exe" ]; then
            ((total++))
            if run_test "MSVC" "Debug" "$PROJECT_ROOT/build/build_tests/msvc/debug/int128_concepts_extracted_tests.exe"; then
                ((passed++))
            fi
        fi
    fi
    
    if [ "$MODE" = "all" ] || [ "$MODE" = "release" ]; then
        if [ -f "$PROJECT_ROOT/build/build_tests/msvc/release/int128_concepts_extracted_tests.exe" ]; then
            ((total++))
            if run_test "MSVC" "Release" "$PROJECT_ROOT/build/build_tests/msvc/release/int128_concepts_extracted_tests.exe"; then
                ((passed++))
            fi
        fi
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
