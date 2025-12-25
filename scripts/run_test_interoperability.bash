#!/bin/bash

# Script para ejecutar test_interoperability_uint128_int128 con múltiples compiladores

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT" || exit 1

echo "========================================================================"
echo "          RUN: test_interoperability_uint128_int128"
echo "          Compilador: ${1:-all}  Modo: ${2:-all}"
echo "          $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================================================"
echo ""

COMPILER_ARG="${1:-all}"
MODE_ARG="${2:-all}"

run_test() {
    local exec_path=$1
    local compiler_name=$2
    
    if [ -f "$exec_path" ]; then
        echo "⚡ Ejecutando $compiler_name..."
        echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
        "$exec_path"
        local exit_code=$?
        echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
        echo ""
        return $exit_code
    else
        echo "⚠️  $compiler_name: Ejecutable no encontrado ($exec_path)"
        echo ""
        return 1
    fi
}

total_runs=0
successful_runs=0

# =============================================================================
# EJECUCIÓN CON GCC
# =============================================================================
if [ "$COMPILER_ARG" = "gcc" ] || [ "$COMPILER_ARG" = "all" ]; then
    if [ "$MODE_ARG" = "debug" ] || [ "$MODE_ARG" = "all" ]; then
        ((total_runs++))
        run_test "$PROJECT_ROOT/build/build_tests/gcc/debug/test_interoperability_gcc" "GCC Debug"
        [ $? -eq 0 ] && ((successful_runs++))
    fi
    
    if [ "$MODE_ARG" = "release" ] || [ "$MODE_ARG" = "all" ]; then
        ((total_runs++))
        run_test "$PROJECT_ROOT/build/build_tests/gcc/release/test_interoperability_gcc" "GCC Release"
        [ $? -eq 0 ] && ((successful_runs++))
    fi
fi

# =============================================================================
# EJECUCIÓN CON CLANG
# =============================================================================
if [ "$COMPILER_ARG" = "clang" ] || [ "$COMPILER_ARG" = "all" ]; then
    if [ "$MODE_ARG" = "debug" ] || [ "$MODE_ARG" = "all" ]; then
        ((total_runs++))
        run_test "$PROJECT_ROOT/build/build_tests/clang/debug/test_interoperability_clang" "Clang Debug"
        [ $? -eq 0 ] && ((successful_runs++))
    fi
    
    if [ "$MODE_ARG" = "release" ] || [ "$MODE_ARG" = "all" ]; then
        ((total_runs++))
        run_test "$PROJECT_ROOT/build/build_tests/clang/release/test_interoperability_clang" "Clang Release"
        [ $? -eq 0 ] && ((successful_runs++))
    fi
fi

# =============================================================================
# EJECUCIÓN CON INTEL
# =============================================================================
if [ "$COMPILER_ARG" = "intel" ] || [ "$COMPILER_ARG" = "all" ]; then
    if [ "$MODE_ARG" = "debug" ] || [ "$MODE_ARG" = "all" ]; then
        ((total_runs++))
        run_test "$PROJECT_ROOT/build/build_tests/intel/debug/test_interoperability_intel" "Intel Debug"
        [ $? -eq 0 ] && ((successful_runs++))
    fi
    
    if [ "$MODE_ARG" = "release" ] || [ "$MODE_ARG" = "all" ]; then
        ((total_runs++))
        run_test "$PROJECT_ROOT/build/build_tests/intel/release/test_interoperability_intel" "Intel Release"
        [ $? -eq 0 ] && ((successful_runs++))
    fi
fi

# =============================================================================
# EJECUCIÓN CON MSVC
# =============================================================================
if [ "$COMPILER_ARG" = "msvc" ] || [ "$COMPILER_ARG" = "all" ]; then
    if [ "$MODE_ARG" = "debug" ] || [ "$MODE_ARG" = "all" ]; then
        ((total_runs++))
        run_test "$PROJECT_ROOT/build/build_tests/msvc/debug/test_interoperability_msvc.exe" "MSVC Debug"
        [ $? -eq 0 ] && ((successful_runs++))
    fi
    
    if [ "$MODE_ARG" = "release" ] || [ "$MODE_ARG" = "all" ]; then
        ((total_runs++))
        run_test "$PROJECT_ROOT/build/build_tests/msvc/release/test_interoperability_msvc.exe" "MSVC Release"
        [ $? -eq 0 ] && ((successful_runs++))
    fi
fi

# Resumen final
echo "========================================================================"
echo "                     RESUMEN DE EJECUCIÓN"
echo "========================================================================"
echo "Tests ejecutados: $total_runs"
echo "Exitosos: $successful_runs"
echo "Fallidos: $((total_runs - successful_runs))"

if [ $successful_runs -eq $total_runs ]; then
    echo ""
    echo "✅ TODOS LOS TESTS DE INTEROPERABILIDAD PASARON"
    exit 0
else
    echo ""
    echo "❌ ALGUNOS TESTS FALLARON"
    exit 1
fi
