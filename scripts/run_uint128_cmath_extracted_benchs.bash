#!/bin/bash

# Script para ejecutar benchmarks de uint128_cmath con todos los compiladores

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT" || exit 1

COMPILER="${1:-all}"
MODE="${2:-all}"
PRINT_RESULTS="${3}"

echo "========================================================================"
echo "           BENCHMARKS: uint128_cmath [${COMPILER}/${MODE}]"
echo "========================================================================"

total_executed=0
total_failed=0

# Función para ejecutar un benchmark específico
run_benchmark() {
    local compiler=$1
    local mode=$2
    local exe_path=$3
    
    if [ ! -f "$exe_path" ]; then
        echo "  ⚠️  No encontrado: $exe_path"
        return 1
    fi
    
    echo ""
    echo "▶ Ejecutando: $(basename "$exe_path")"
    
    if [ "$PRINT_RESULTS" = "print" ]; then
        OUTPUT_DIR="$PROJECT_ROOT/build/benchmark_results/${compiler}/${mode}"
        mkdir -p "$OUTPUT_DIR"
        OUTPUT_FILE="$OUTPUT_DIR/uint128_cmath_benchmarks.txt"
        
        "$exe_path" > "$OUTPUT_FILE" 2>&1
        result=$?
        
        # Mostrar resumen de los últimos resultados
        tail -30 "$OUTPUT_FILE"
    else
        "$exe_path" 2>&1
        result=$?
    fi
    
    if [ $result -eq 0 ]; then
        echo "  ✅ Benchmark completado"
        ((total_executed++))
        return 0
    else
        echo "  ❌ Benchmark falló"
        ((total_failed++))
        return 1
    fi
}

# =============================================================================
# EJECUTAR BENCHMARKS POR COMPILADOR Y MODO
# =============================================================================

if [ "$COMPILER" = "gcc" ] || [ "$COMPILER" = "all" ]; then
    if [ "$MODE" = "debug" ] || [ "$MODE" = "all" ]; then
        run_benchmark "gcc" "debug" "$PROJECT_ROOT/build/build_benchmarks/gcc/debug/uint128_cmath_benchmarks_gcc"
    fi
    if [ "$MODE" = "release" ] || [ "$MODE" = "all" ]; then
        run_benchmark "gcc" "release" "$PROJECT_ROOT/build/build_benchmarks/gcc/release/uint128_cmath_benchmarks_gcc"
    fi
fi

if [ "$COMPILER" = "clang" ] || [ "$COMPILER" = "all" ]; then
    if [ "$MODE" = "debug" ] || [ "$MODE" = "all" ]; then
        run_benchmark "clang" "debug" "$PROJECT_ROOT/build/build_benchmarks/clang/debug/uint128_cmath_benchmarks_clang"
    fi
    if [ "$MODE" = "release" ] || [ "$MODE" = "all" ]; then
        run_benchmark "clang" "release" "$PROJECT_ROOT/build/build_benchmarks/clang/release/uint128_cmath_benchmarks_clang"
    fi
fi

if [ "$COMPILER" = "intel" ] || [ "$COMPILER" = "all" ]; then
    if [ "$MODE" = "debug" ] || [ "$MODE" = "all" ]; then
        run_benchmark "intel" "debug" "$PROJECT_ROOT/build/build_benchmarks/intel/debug/uint128_cmath_benchmarks_intel"
    fi
    if [ "$MODE" = "release" ] || [ "$MODE" = "all" ]; then
        run_benchmark "intel" "release" "$PROJECT_ROOT/build/build_benchmarks/intel/release/uint128_cmath_benchmarks_intel"
    fi
fi

if [ "$COMPILER" = "msvc" ] || [ "$COMPILER" = "all" ]; then
    if [ "$MODE" = "debug" ] || [ "$MODE" = "all" ]; then
        run_benchmark "msvc" "debug" "$PROJECT_ROOT/build/build_benchmarks/msvc/debug/uint128_cmath_benchmarks_msvc.exe"
    fi
    if [ "$MODE" = "release" ] || [ "$MODE" = "all" ]; then
        run_benchmark "msvc" "release" "$PROJECT_ROOT/build/build_benchmarks/msvc/release/uint128_cmath_benchmarks_msvc.exe"
    fi
fi

# =============================================================================
# RESUMEN FINAL
# =============================================================================
echo ""
echo "========================================================================"
if [ $total_failed -eq 0 ] && [ $total_executed -gt 0 ]; then
    echo " RESUMEN: $total_executed/$total_executed benchmarks ejecutados"
    echo " ✅ TODOS LOS BENCHMARKS COMPLETADOS"
else
    echo " RESUMEN: Ejecutados: $total_executed | Fallidos: $total_failed"
    echo " ⚠️  Algunos benchmarks fallaron"
fi
echo "========================================================================"

exit $total_failed
