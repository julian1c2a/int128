#!/bin/bash

# Script para compilar benchmark_comparison_boost.cpp con múltiples compiladores
# Sigue la estructura modular: build/build_benchmarks/{compiler}/{mode}/

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT" || exit 1

SOURCE_FILE="benchmarks/benchmark_comparison_boost.cpp"
INCLUDE_DIR="-Iinclude"
COMMON_FLAGS="-std=c++20 -Wall -Wextra"

# Boost.Multiprecision, GMP y tommath headers
BOOST_FLAGS=""
if command -v pkg-config &> /dev/null; then
    if pkg-config --exists gmp 2>/dev/null; then
        BOOST_FLAGS="$BOOST_FLAGS -lgmp"
    fi
    if pkg-config --exists libtommath 2>/dev/null; then
        BOOST_FLAGS="$BOOST_FLAGS -ltommath"
    fi
fi

echo "========================================================================"
echo "                  BUILD: benchmark_comparison_boost"
echo "                  Compilador: ${1:-all}  Modo: ${2:-all}"
echo "                  $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================================================"
echo ""

# Obtener tamaño del archivo fuente
if [ -f "$SOURCE_FILE" ]; then
    SIZE=$(du -h "$SOURCE_FILE" | cut -f1)
    echo "📄 Archivo fuente:"
    echo "   $(realpath --relative-to="$PROJECT_ROOT" "$SOURCE_FILE") ($SIZE)"
else
    echo "[FAIL] ERROR: No se encontró $SOURCE_FILE"
    exit 1
fi

echo ""

COMPILER_ARG="${1:-all}"
MODE_ARG="${2:-all}"

# =============================================================================
# COMPILACIÓN CON GCC
# =============================================================================
if [ "$COMPILER_ARG" = "gcc" ] || [ "$COMPILER_ARG" = "all" ]; then
    echo "[BUILD] [1/4] Compilando con GCC..."
    
    if ! command -v g++ &> /dev/null; then
        echo "   [WARN]  GCC no disponible"
    else
        # Debug
        if [ "$MODE_ARG" = "debug" ] || [ "$MODE_ARG" = "all" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_benchmarks/gcc/debug"
            g++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 $BOOST_FLAGS \
                -o "$PROJECT_ROOT/build/build_benchmarks/gcc/debug/benchmark_comparison_boost_gcc" 2>&1
            result_debug=$?
            
            if [ $result_debug -eq 0 ]; then
                echo "   [OK] GCC Debug: build/build_benchmarks/gcc/debug/benchmark_comparison_boost_gcc"
            else
                echo "   [FAIL] GCC Debug FAILED"
            fi
        fi
        
        # Release
        if [ "$MODE_ARG" = "release" ] || [ "$MODE_ARG" = "all" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_benchmarks/gcc/release"
            g++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O3 -march=native -DNDEBUG $BOOST_FLAGS \
                -o "$PROJECT_ROOT/build/build_benchmarks/gcc/release/benchmark_comparison_boost_gcc" 2>&1
            result_release=$?
            
            if [ $result_release -eq 0 ]; then
                echo "   [OK] GCC Release: build/build_benchmarks/gcc/release/benchmark_comparison_boost_gcc"
            else
                echo "   [FAIL] GCC Release FAILED"
            fi
        fi
    fi
    echo ""
fi

# =============================================================================
# COMPILACIÓN CON CLANG
# =============================================================================
if [ "$COMPILER_ARG" = "clang" ] || [ "$COMPILER_ARG" = "all" ]; then
    echo "[BUILD] [2/4] Compilando con Clang..."
    
    if ! command -v clang++ &> /dev/null; then
        echo "   [WARN]  Clang no disponible"
    else
        # Debug
        if [ "$MODE_ARG" = "debug" ] || [ "$MODE_ARG" = "all" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_benchmarks/clang/debug"
            clang++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 $BOOST_FLAGS \
                -o "$PROJECT_ROOT/build/build_benchmarks/clang/debug/benchmark_comparison_boost_clang" 2>&1
            result_debug=$?
            
            if [ $result_debug -eq 0 ]; then
                echo "   [OK] Clang Debug: build/build_benchmarks/clang/debug/benchmark_comparison_boost_clang"
            else
                echo "   [FAIL] Clang Debug FAILED"
            fi
        fi
        
        # Release
        if [ "$MODE_ARG" = "release" ] || [ "$MODE_ARG" = "all" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_benchmarks/clang/release"
            clang++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O3 -march=native -DNDEBUG $BOOST_FLAGS \
                -o "$PROJECT_ROOT/build/build_benchmarks/clang/release/benchmark_comparison_boost_clang" 2>&1
            result_release=$?
            
            if [ $result_release -eq 0 ]; then
                echo "   [OK] Clang Release: build/build_benchmarks/clang/release/benchmark_comparison_boost_clang"
            else
                echo "   [FAIL] Clang Release FAILED"
            fi
        fi
    fi
    echo ""
fi

# =============================================================================
# COMPILACIÓN CON INTEL
# =============================================================================
if [ "$COMPILER_ARG" = "intel" ] || [ "$COMPILER_ARG" = "all" ]; then
    echo "[BUILD] [3/4] Compilando con Intel..."
    
    if ! command -v icpx &> /dev/null; then
        echo "   [WARN]  Intel compiler (icpx) no disponible"
    else
        # Debug
        if [ "$MODE_ARG" = "debug" ] || [ "$MODE_ARG" = "all" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_benchmarks/intel/debug"
            icpx $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 $BOOST_FLAGS \
                -o "$PROJECT_ROOT/build/build_benchmarks/intel/debug/benchmark_comparison_boost_intel" 2>&1
            result_debug=$?
            
            if [ $result_debug -eq 0 ]; then
                echo "   [OK] Intel Debug: build/build_benchmarks/intel/debug/benchmark_comparison_boost_intel"
            else
                echo "   [FAIL] Intel Debug FAILED"
            fi
        fi
        
        # Release
        if [ "$MODE_ARG" = "release" ] || [ "$MODE_ARG" = "all" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_benchmarks/intel/release"
            icpx $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O3 -march=native -DNDEBUG $BOOST_FLAGS \
                -o "$PROJECT_ROOT/build/build_benchmarks/intel/release/benchmark_comparison_boost_intel" 2>&1
            result_release=$?
            
            if [ $result_release -eq 0 ]; then
                echo "   [OK] Intel Release: build/build_benchmarks/intel/release/benchmark_comparison_boost_intel"
            else
                echo "   [FAIL] Intel Release FAILED"
            fi
        fi
    fi
    echo ""
fi

# =============================================================================
# NOTA: MSVC NO SOPORTA __int128
# =============================================================================
if [ "$COMPILER_ARG" = "msvc" ] || [ "$COMPILER_ARG" = "all" ]; then
    echo "[BUILD] [4/4] MSVC..."
    echo "   [WARN]  MSVC no soporta __int128 (GCC/Clang builtin)"
    echo "   [WARN]  Este benchmark solo funciona con GCC/Clang"
    echo ""
fi

echo "========================================================================"
echo "                          BUILD COMPLETO"
echo "========================================================================"
