#!/bin/bash

# Script para ejecutar benchmark_int128_vs_uint128 con múltiples compiladores

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT" || exit 1

echo "========================================================================"
echo "              RUN: benchmark_int128_vs_uint128"
echo "              Compilador: ${1:-all}  Modo: ${2:-all}"
echo "              $(date '+%Y-%m-%d %H:%M:%S')"
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
        EXEC_PATH="$PROJECT_ROOT/build/build_benchs/gcc/debug/uint128_interop_benchs_gcc"
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
        EXEC_PATH="$PROJECT_ROOT/build/build_benchs/gcc/release/uint128_interop_benchs_gcc"
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
        EXEC_PATH="$PROJECT_ROOT/build/build_benchs/clang/debug/uint128_interop_benchs_clang"
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
        EXEC_PATH="$PROJECT_ROOT/build/build_benchs/clang/release/uint128_interop_benchs_clang"
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
        EXEC_PATH="$PROJECT_ROOT/build/build_benchs/intel/debug/uint128_interop_benchs_intel"
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
        EXEC_PATH="$PROJECT_ROOT/build/build_benchs/intel/release/uint128_interop_benchs_intel"
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
# EJECUCIÓN CON MSVC
# =============================================================================
if [ "$COMPILER_ARG" = "msvc" ] || [ "$COMPILER_ARG" = "all" ]; then
    # Debug
    if [ "$MODE_ARG" = "debug" ] || [ "$MODE_ARG" = "all" ]; then
        EXEC_PATH="$PROJECT_ROOT/build/build_benchs/msvc/debug/uint128_interop_benchs_msvc.exe"
        if [ -f "$EXEC_PATH" ]; then
            echo "[RUN] Ejecutando MSVC Debug..."
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            "$EXEC_PATH"
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            echo ""
        else
            echo "[WARN]  MSVC Debug: Ejecutable no encontrado ($EXEC_PATH)"
            echo ""
        fi
    fi
    
    # Release
    if [ "$MODE_ARG" = "release" ] || [ "$MODE_ARG" = "all" ]; then
        EXEC_PATH="$PROJECT_ROOT/build/build_benchs/msvc/release/uint128_interop_benchs_msvc.exe"
        if [ -f "$EXEC_PATH" ]; then
            echo "[RUN] Ejecutando MSVC Release..."
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            "$EXEC_PATH"
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            echo ""
        else
            echo "[WARN]  MSVC Release: Ejecutable no encontrado ($EXEC_PATH)"
            echo ""
        fi
    fi
fi

echo "========================================================================"
echo "                          EJECUCIÓN COMPLETA"
echo "========================================================================"
