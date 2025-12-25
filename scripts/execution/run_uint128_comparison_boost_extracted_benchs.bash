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
        EXEC_PATH="$PROJECT_ROOT/build/build_benchs/gcc/debug/uint128_comparison_boost_benchs_gcc"
        if [ -f "$EXEC_PATH" ]; then
            echo "[RUN] Ejecutando GCC Debug..."
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            "$EXEC_PATH"
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            echo ""
        else
            echo "[WARN]  GCC Debug: Ejecutable no encontrado ($EXEC_PATH)"
            echo ""
        fi
    fi
    
    # Release
    if [ "$MODE_ARG" = "release" ] || [ "$MODE_ARG" = "all" ]; then
        EXEC_PATH="$PROJECT_ROOT/build/build_benchs/gcc/release/uint128_comparison_boost_benchs_gcc"
        if [ -f "$EXEC_PATH" ]; then
            echo "[RUN] Ejecutando GCC Release..."
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            "$EXEC_PATH"
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            echo ""
        else
            echo "[WARN]  GCC Release: Ejecutable no encontrado ($EXEC_PATH)"
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
        EXEC_PATH="$PROJECT_ROOT/build/build_benchs/clang/debug/uint128_comparison_boost_benchs_clang"
        if [ -f "$EXEC_PATH" ]; then
            echo "[RUN] Ejecutando Clang Debug..."
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            "$EXEC_PATH"
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            echo ""
        else
            echo "[WARN]  Clang Debug: Ejecutable no encontrado ($EXEC_PATH)"
            echo ""
        fi
    fi
    
    # Release
    if [ "$MODE_ARG" = "release" ] || [ "$MODE_ARG" = "all" ]; then
        EXEC_PATH="$PROJECT_ROOT/build/build_benchs/clang/release/uint128_comparison_boost_benchs_clang"
        if [ -f "$EXEC_PATH" ]; then
            echo "[RUN] Ejecutando Clang Release..."
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            "$EXEC_PATH"
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            echo ""
        else
            echo "[WARN]  Clang Release: Ejecutable no encontrado ($EXEC_PATH)"
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
        EXEC_PATH="$PROJECT_ROOT/build/build_benchs/intel/debug/uint128_comparison_boost_benchs_intel"
        if [ -f "$EXEC_PATH" ]; then
            echo "[RUN] Ejecutando Intel Debug..."
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            "$EXEC_PATH"
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            echo ""
        else
            echo "[WARN]  Intel Debug: Ejecutable no encontrado ($EXEC_PATH)"
            echo ""
        fi
    fi
    
    # Release
    if [ "$MODE_ARG" = "release" ] || [ "$MODE_ARG" = "all" ]; then
        EXEC_PATH="$PROJECT_ROOT/build/build_benchs/intel/release/uint128_comparison_boost_benchs_intel"
        if [ -f "$EXEC_PATH" ]; then
            echo "[RUN] Ejecutando Intel Release..."
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            "$EXEC_PATH"
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            echo ""
        else
            echo "[WARN]  Intel Release: Ejecutable no encontrado ($EXEC_PATH)"
            echo ""
        fi
    fi
fi

# =============================================================================
# NOTA: MSVC NO SOPORTA __int128
# =============================================================================
if [ "$COMPILER_ARG" = "msvc" ] || [ "$COMPILER_ARG" = "all" ]; then
    echo "[WARN]  MSVC: No soporta __int128 (este benchmark requiere GCC/Clang)"
    echo ""
fi

echo "========================================================================"
echo "                          EJECUCIÓN COMPLETA"
echo "========================================================================"
