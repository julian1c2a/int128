#!/bin/bash

# Script para ejecutar benchmark_comparison_boost con múltiples compiladores

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT" || exit 1

echo "========================================================================"
echo "                  RUN: benchmark_comparison_boost"
echo "                  Compilador: ${1:-all}  Modo: ${2:-all}"
echo "                  $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================================================"
echo ""

COMPILER_ARG="${1:-all}"
MODE_ARG="${2:-all}"

# =============================================================================
# EJECUCIÓN CON GCC
# =============================================================================
if [ "$COMPILER_ARG" = "gcc" ] || [ "$COMPILER_ARG" = "all" ]; then
    # Debug
    if [ "$MODE_ARG" = "debug" ] || [ "$MODE_ARG" = "all" ]; then
        EXEC_PATH="$PROJECT_ROOT/build/build_benchmarks/gcc/debug/benchmark_comparison_boost_gcc"
        if [ -f "$EXEC_PATH" ]; then
            echo "⚡ Ejecutando GCC Debug..."
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            "$EXEC_PATH"
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            echo ""
        else
            echo "⚠️  GCC Debug: Ejecutable no encontrado ($EXEC_PATH)"
            echo ""
        fi
    fi
    
    # Release
    if [ "$MODE_ARG" = "release" ] || [ "$MODE_ARG" = "all" ]; then
        EXEC_PATH="$PROJECT_ROOT/build/build_benchmarks/gcc/release/benchmark_comparison_boost_gcc"
        if [ -f "$EXEC_PATH" ]; then
            echo "⚡ Ejecutando GCC Release..."
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            "$EXEC_PATH"
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            echo ""
        else
            echo "⚠️  GCC Release: Ejecutable no encontrado ($EXEC_PATH)"
            echo ""
        fi
    fi
fi

# =============================================================================
# EJECUCIÓN CON CLANG
# =============================================================================
if [ "$COMPILER_ARG" = "clang" ] || [ "$COMPILER_ARG" = "all" ]; then
    # Debug
    if [ "$MODE_ARG" = "debug" ] || [ "$MODE_ARG" = "all" ]; then
        EXEC_PATH="$PROJECT_ROOT/build/build_benchmarks/clang/debug/benchmark_comparison_boost_clang"
        if [ -f "$EXEC_PATH" ]; then
            echo "⚡ Ejecutando Clang Debug..."
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            "$EXEC_PATH"
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            echo ""
        else
            echo "⚠️  Clang Debug: Ejecutable no encontrado ($EXEC_PATH)"
            echo ""
        fi
    fi
    
    # Release
    if [ "$MODE_ARG" = "release" ] || [ "$MODE_ARG" = "all" ]; then
        EXEC_PATH="$PROJECT_ROOT/build/build_benchmarks/clang/release/benchmark_comparison_boost_clang"
        if [ -f "$EXEC_PATH" ]; then
            echo "⚡ Ejecutando Clang Release..."
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            "$EXEC_PATH"
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            echo ""
        else
            echo "⚠️  Clang Release: Ejecutable no encontrado ($EXEC_PATH)"
            echo ""
        fi
    fi
fi

# =============================================================================
# EJECUCIÓN CON INTEL
# =============================================================================
if [ "$COMPILER_ARG" = "intel" ] || [ "$COMPILER_ARG" = "all" ]; then
    # Debug
    if [ "$MODE_ARG" = "debug" ] || [ "$MODE_ARG" = "all" ]; then
        EXEC_PATH="$PROJECT_ROOT/build/build_benchmarks/intel/debug/benchmark_comparison_boost_intel"
        if [ -f "$EXEC_PATH" ]; then
            echo "⚡ Ejecutando Intel Debug..."
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            "$EXEC_PATH"
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            echo ""
        else
            echo "⚠️  Intel Debug: Ejecutable no encontrado ($EXEC_PATH)"
            echo ""
        fi
    fi
    
    # Release
    if [ "$MODE_ARG" = "release" ] || [ "$MODE_ARG" = "all" ]; then
        EXEC_PATH="$PROJECT_ROOT/build/build_benchmarks/intel/release/benchmark_comparison_boost_intel"
        if [ -f "$EXEC_PATH" ]; then
            echo "⚡ Ejecutando Intel Release..."
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            "$EXEC_PATH"
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            echo ""
        else
            echo "⚠️  Intel Release: Ejecutable no encontrado ($EXEC_PATH)"
            echo ""
        fi
    fi
fi

# =============================================================================
# NOTA: MSVC NO SOPORTA __int128
# =============================================================================
if [ "$COMPILER_ARG" = "msvc" ] || [ "$COMPILER_ARG" = "all" ]; then
    echo "⚠️  MSVC: No soporta __int128 (este benchmark requiere GCC/Clang)"
    echo ""
fi

echo "========================================================================"
echo "                          EJECUCIÓN COMPLETA"
echo "========================================================================"
