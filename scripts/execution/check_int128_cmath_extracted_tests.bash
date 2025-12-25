#!/bin/bash

# Script para ejecutar tests de int128_cmath con todos los compiladores

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT" || exit 1

COMPILER="${1:-all}"
MODE="${2:-all}"
PRINT_RESULTS="${3}"

echo "========================================================================"
echo "              TESTS: int128_cmath [${COMPILER}/${MODE}]"
echo "========================================================================"

declare -A test_results
total_passed=0
total_failed=0
total_configs=0

# Función para ejecutar un test específico
run_test() {
    local compiler=$1
    local mode=$2
    local exe_path=$3
    
    if [ ! -f "$exe_path" ]; then
        echo "  [WARN]  No encontrado: $exe_path"
        return 1
    fi
    
    echo ""
    echo "▶ Ejecutando: $(basename "$exe_path")"
    
    if [ "$PRINT_RESULTS" = "print" ]; then
        OUTPUT_DIR="$PROJECT_ROOT/build/test_results/${compiler}/${mode}"
        mkdir -p "$OUTPUT_DIR"
        OUTPUT_FILE="$OUTPUT_DIR/int128_cmath_tests.txt"
        
        "$exe_path" > "$OUTPUT_FILE" 2>&1
        result=$?
        
        # Mostrar resumen
        if grep -q "tests pasados" "$OUTPUT_FILE" 2>/dev/null; then
            grep "tests pasados" "$OUTPUT_FILE"
        fi
    else
        "$exe_path" 2>&1 | tail -20
        result=$?
    fi
    
    if [ $result -eq 0 ]; then
        echo "  [OK] PASS"
        ((total_passed++))
        test_results["${compiler}_${mode}"]="[OK]"
        return 0
    else
        echo "  [FAIL] FAIL"
        ((total_failed++))
        test_results["${compiler}_${mode}"]="[FAIL]"
        return 1
    fi
}

# =============================================================================
# EJECUTAR TESTS POR COMPILADOR Y MODO
# =============================================================================

if [ "$COMPILER" = "gcc" ] || [ "$COMPILER" = "all" ]; then
    if [ "$MODE" = "debug" ] || [ "$MODE" = "all" ]; then
        run_test "gcc" "debug" "$PROJECT_ROOT/build/build_tests/gcc/debug/int128_cmath_tests_gcc"
        ((total_configs++))
    fi
    if [ "$MODE" = "release" ] || [ "$MODE" = "all" ]; then
        run_test "gcc" "release" "$PROJECT_ROOT/build/build_tests/gcc/release/int128_cmath_tests_gcc"
        ((total_configs++))
    fi
fi

if [ "$COMPILER" = "clang" ] || [ "$COMPILER" = "all" ]; then
    if [ "$MODE" = "debug" ] || [ "$MODE" = "all" ]; then
        run_test "clang" "debug" "$PROJECT_ROOT/build/build_tests/clang/debug/int128_cmath_tests_clang"
        ((total_configs++))
    fi
    if [ "$MODE" = "release" ] || [ "$MODE" = "all" ]; then
        run_test "clang" "release" "$PROJECT_ROOT/build/build_tests/clang/release/int128_cmath_tests_clang"
        ((total_configs++))
    fi
fi

if [ "$COMPILER" = "intel" ] || [ "$COMPILER" = "all" ]; then
    if [ "$MODE" = "debug" ] || [ "$MODE" = "all" ]; then
        run_test "intel" "debug" "$PROJECT_ROOT/build/build_tests/intel/debug/int128_cmath_tests_intel"
        ((total_configs++))
    fi
    if [ "$MODE" = "release" ] || [ "$MODE" = "all" ]; then
        run_test "intel" "release" "$PROJECT_ROOT/build/build_tests/intel/release/int128_cmath_tests_intel"
        ((total_configs++))
    fi
fi

if [ "$COMPILER" = "msvc" ] || [ "$COMPILER" = "all" ]; then
    if [ "$MODE" = "debug" ] || [ "$MODE" = "all" ]; then
        run_test "msvc" "debug" "$PROJECT_ROOT/build/build_tests/msvc/debug/int128_cmath_tests_msvc.exe"
        ((total_configs++))
    fi
    if [ "$MODE" = "release" ] || [ "$MODE" = "all" ]; then
        run_test "msvc" "release" "$PROJECT_ROOT/build/build_tests/msvc/release/int128_cmath_tests_msvc.exe"
        ((total_configs++))
    fi
fi

# =============================================================================
# RESUMEN FINAL
# =============================================================================
echo ""
echo "========================================================================"
echo "                       RESUMEN DE TESTS"
echo "========================================================================"

# Mostrar matriz de resultados
if [ "$COMPILER" = "all" ] && [ "$MODE" = "all" ]; then
    echo ""
    echo "Resultados por configuración:"
    echo "  GCC Debug:    ${test_results[gcc_debug]:-[WARN] }"
    echo "  GCC Release:  ${test_results[gcc_release]:-[WARN] }"
    echo "  Clang Debug:  ${test_results[clang_debug]:-[WARN] }"
    echo "  Clang Release:${test_results[clang_release]:-[WARN] }"
    echo "  Intel Debug:  ${test_results[intel_debug]:-[WARN] }"
    echo "  Intel Release:${test_results[intel_release]:-[WARN] }"
    echo "  MSVC Debug:   ${test_results[msvc_debug]:-[WARN] }"
    echo "  MSVC Release: ${test_results[msvc_release]:-[WARN] }"
fi

echo ""
if [ $total_failed -eq 0 ] && [ $total_passed -gt 0 ]; then
    echo " RESUMEN: $total_configs/$total_configs configuraciones OK"
    echo " [OK] TODOS LOS TESTS PASARON"
else
    echo " RESUMEN: $total_passed/$total_configs configuraciones OK"
    echo " [WARN]  Passed: $total_passed | Failed: $total_failed"
fi
echo "========================================================================"

exit $total_failed
