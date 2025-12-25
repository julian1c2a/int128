#!/usr/bin/env bash
# ==============================================================================
# run_generic.bash - Script genérico de ejecución de benchmarks
# ==============================================================================
# Ejecuta benchmarks compilados con todos los compiladores y modos
# 
# Uso:
#   run_generic.bash <type> <feature> [compiler] [mode]
#
# Argumentos:
#   type     : uint128 | int128
#   feature  : bits | numeric | algorithm | iostreams | cmath | etc.
#   compiler : gcc | clang | intel | msvc | all (default: all)
#   mode     : debug | release | all (default: all)
#
# Ejemplos:
#   run_generic.bash uint128 bits
#   run_generic.bash int128 numeric gcc release
#   run_generic.bash uint128 algorithm all all
# ==============================================================================

set -euo pipefail

# ========================= Parse Arguments =========================

if [[ $# -lt 2 ]]; then
    echo "Error: Se requieren al menos 2 argumentos"
    echo "Uso: $0 <type> <feature> [compiler] [mode]"
    echo ""
    echo "Ejemplos:"
    echo "  $0 uint128 bits"
    echo "  $0 int128 numeric gcc release"
    echo "  $0 uint128 algorithm all all"
    exit 1
fi

TYPE="$1"
FEATURE="$2"
COMPILER="${3:-all}"
MODE="${4:-all}"

# ========================= Validation =========================

if [[ "$TYPE" != "uint128" && "$TYPE" != "int128" ]]; then
    echo "Error: TYPE debe ser 'uint128' o 'int128'"
    exit 1
fi

VALID_COMPILERS=("gcc" "clang" "intel" "msvc" "all")
if [[ ! " ${VALID_COMPILERS[*]} " =~ " ${COMPILER} " ]]; then
    echo "Error: COMPILER debe ser uno de: ${VALID_COMPILERS[*]}"
    exit 1
fi

VALID_MODES=("debug" "release" "all")
if [[ ! " ${VALID_MODES[*]} " =~ " ${MODE} " ]]; then
    echo "Error: MODE debe ser uno de: ${VALID_MODES[*]}"
    exit 1
fi

# ========================= Configuration =========================

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_ROOT"

BUILD_DIR="$PROJECT_ROOT/build/build_benchs"

# ========================= Color Output =========================

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo_success() { echo -e "${GREEN}[OK] $1${NC}"; }
echo_error() { echo -e "${RED}[ERROR] $1${NC}"; }
echo_info() { echo -e "${YELLOW}-> $1${NC}"; }
echo_header() { echo -e "${BLUE}$1${NC}"; }

# ========================= Execution Function =========================

run_benchmark() {
    local comp="$1"
    local mod="$2"
    
    local exe_name="${TYPE}_${FEATURE}_benchs_${comp}"
    
    if [[ "$comp" == "msvc" || "$comp" == "intel" ]]; then
        exe_name="${exe_name}.exe"
    fi
    
    local exe_path="$BUILD_DIR/$comp/$mod/$exe_name"
    
    if [[ ! -f "$exe_path" ]]; then
        echo_info "Skipping: $comp [$mod] (not found)"
        return 1
    fi
    
    echo_header "--------------------------------------------------------------------"
    echo_info "Running: $comp [$mod]"
    echo_header "--------------------------------------------------------------------"
    
    if "$exe_path"; then
        echo_success "  $comp [$mod]: completed"
        return 0
    else
        echo_error "  $comp [$mod]: failed"
        return 1
    fi
}

# ========================= Execute Benchmarks =========================

echo_header "========================================================================"
echo_header "  ${TYPE^^} ${FEATURE^^} BENCHMARKS - Execution"
echo_header "========================================================================"
echo ""

executed=0
failed=0
skipped=0

# Define compilers to test
compilers_to_test=()
if [[ "$COMPILER" == "all" ]]; then
    compilers_to_test=("gcc" "clang" "intel" "msvc")
else
    compilers_to_test=("$COMPILER")
fi

# Define modes to test
modes_to_test=()
if [[ "$MODE" == "all" ]]; then
    modes_to_test=("debug" "release")
else
    modes_to_test=("$MODE")
fi

# Execute benchmarks
for comp in "${compilers_to_test[@]}"; do
    for mod in "${modes_to_test[@]}"; do
        if run_benchmark "$comp" "$mod"; then
            ((executed++))
        else
            if [[ -f "$BUILD_DIR/$comp/$mod/${TYPE}_${FEATURE}_benchs_${comp}"* ]]; then
                ((failed++))
            else
                ((skipped++))
            fi
        fi
        echo ""
    done
done

# ========================= Summary =========================

echo_header "========================================================================"
echo_header "  BENCHMARK EXECUTION SUMMARY"
echo_header "========================================================================"
echo ""
echo "Executed: $executed"
echo "Failed: $failed"
echo "Skipped: $skipped"
echo ""

if [[ $failed -gt 0 ]]; then
    exit 1
fi
