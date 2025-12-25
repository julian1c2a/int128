#!/bin/bash

# Script para compilar test_interoperability_uint128_int128.cpp con múltiples compiladores

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT" || exit 1

SOURCE_FILE="tests/test_interoperability_uint128_int128.cpp"
INCLUDE_DIR="-Iinclude"
COMMON_FLAGS="-std=c++20 -Wall -Wextra"

echo "========================================================================"
echo "          BUILD: test_interoperability_uint128_int128"
echo "          Compilador: ${1:-all}  Modo: ${2:-all}"
echo "          $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================================================"
echo ""

if [ -f "$SOURCE_FILE" ]; then
    SIZE=$(du -h "$SOURCE_FILE" | cut -f1)
    echo "📄 Archivo fuente: $SOURCE_FILE ($SIZE)"
else
    echo "❌ ERROR: No se encontró $SOURCE_FILE"
    exit 1
fi

echo ""

COMPILER_ARG="${1:-all}"
MODE_ARG="${2:-all}"

# =============================================================================
# COMPILACIÓN CON GCC
# =============================================================================
if [ "$COMPILER_ARG" = "gcc" ] || [ "$COMPILER_ARG" = "all" ]; then
    echo "🔨 [1/4] Compilando con GCC..."
    
    if ! command -v g++ &> /dev/null; then
        echo "   ⚠️  GCC no disponible"
    else
        if [ "$MODE_ARG" = "debug" ] || [ "$MODE_ARG" = "all" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_tests/gcc/debug"
            g++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
                -o "$PROJECT_ROOT/build/build_tests/gcc/debug/test_interoperability_gcc" 2>&1
            [ $? -eq 0 ] && echo "   ✅ GCC Debug: build/build_tests/gcc/debug/test_interoperability_gcc" || echo "   ❌ GCC Debug FAILED"
        fi
        
        if [ "$MODE_ARG" = "release" ] || [ "$MODE_ARG" = "all" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_tests/gcc/release"
            g++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O2 -DNDEBUG \
                -o "$PROJECT_ROOT/build/build_tests/gcc/release/test_interoperability_gcc" 2>&1
            [ $? -eq 0 ] && echo "   ✅ GCC Release: build/build_tests/gcc/release/test_interoperability_gcc" || echo "   ❌ GCC Release FAILED"
        fi
    fi
    echo ""
fi

# =============================================================================
# COMPILACIÓN CON CLANG
# =============================================================================
if [ "$COMPILER_ARG" = "clang" ] || [ "$COMPILER_ARG" = "all" ]; then
    echo "🔨 [2/4] Compilando con Clang..."
    
    if ! command -v clang++ &> /dev/null; then
        echo "   ⚠️  Clang no disponible"
    else
        if [ "$MODE_ARG" = "debug" ] || [ "$MODE_ARG" = "all" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_tests/clang/debug"
            clang++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
                -o "$PROJECT_ROOT/build/build_tests/clang/debug/test_interoperability_clang" 2>&1
            [ $? -eq 0 ] && echo "   ✅ Clang Debug: build/build_tests/clang/debug/test_interoperability_clang" || echo "   ❌ Clang Debug FAILED"
        fi
        
        if [ "$MODE_ARG" = "release" ] || [ "$MODE_ARG" = "all" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_tests/clang/release"
            clang++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O2 -DNDEBUG \
                -o "$PROJECT_ROOT/build/build_tests/clang/release/test_interoperability_clang" 2>&1
            [ $? -eq 0 ] && echo "   ✅ Clang Release: build/build_tests/clang/release/test_interoperability_clang" || echo "   ❌ Clang Release FAILED"
        fi
    fi
    echo ""
fi

# =============================================================================
# COMPILACIÓN CON INTEL
# =============================================================================
if [ "$COMPILER_ARG" = "intel" ] || [ "$COMPILER_ARG" = "all" ]; then
    echo "🔨 [3/4] Compilando con Intel..."
    
    if ! command -v icpx &> /dev/null; then
        echo "   ⚠️  Intel compiler (icpx) no disponible"
    else
        if [ "$MODE_ARG" = "debug" ] || [ "$MODE_ARG" = "all" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_tests/intel/debug"
            icpx $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
                -o "$PROJECT_ROOT/build/build_tests/intel/debug/test_interoperability_intel" 2>&1
            [ $? -eq 0 ] && echo "   ✅ Intel Debug: build/build_tests/intel/debug/test_interoperability_intel" || echo "   ❌ Intel Debug FAILED"
        fi
        
        if [ "$MODE_ARG" = "release" ] || [ "$MODE_ARG" = "all" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_tests/intel/release"
            icpx $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O2 -DNDEBUG \
                -o "$PROJECT_ROOT/build/build_tests/intel/release/test_interoperability_intel" 2>&1
            [ $? -eq 0 ] && echo "   ✅ Intel Release: build/build_tests/intel/release/test_interoperability_intel" || echo "   ❌ Intel Release FAILED"
        fi
    fi
    echo ""
fi

# =============================================================================
# COMPILACIÓN CON MSVC
# =============================================================================
if [ "$COMPILER_ARG" = "msvc" ] || [ "$COMPILER_ARG" = "all" ]; then
    echo "🔨 [4/4] Compilando con MSVC..."
    
    if ! command -v cl.exe &> /dev/null; then
        echo "   ⚠️  MSVC no disponible"
    else
        if [ "$MODE_ARG" = "debug" ] || [ "$MODE_ARG" = "all" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_tests/msvc/debug"
            cl.exe /std:c++20 /EHsc /W4 /I"$PROJECT_ROOT/include" /Zi /Od /MDd \
                "$PROJECT_ROOT/$SOURCE_FILE" \
                /Fe"$PROJECT_ROOT/build/build_tests/msvc/debug/test_interoperability_msvc.exe" 2>&1
            [ $? -eq 0 ] && echo "   ✅ MSVC Debug: build/build_tests/msvc/debug/test_interoperability_msvc.exe" || echo "   ❌ MSVC Debug FAILED"
        fi
        
        if [ "$MODE_ARG" = "release" ] || [ "$MODE_ARG" = "all" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_tests/msvc/release"
            cl.exe /std:c++20 /EHsc /W4 /I"$PROJECT_ROOT/include" /O2 /MD /DNDEBUG \
                "$PROJECT_ROOT/$SOURCE_FILE" \
                /Fe"$PROJECT_ROOT/build/build_tests/msvc/release/test_interoperability_msvc.exe" 2>&1
            [ $? -eq 0 ] && echo "   ✅ MSVC Release: build/build_tests/msvc/release/test_interoperability_msvc.exe" || echo "   ❌ MSVC Release FAILED"
        fi
    fi
    echo ""
fi

echo "========================================================================"
echo "                          BUILD COMPLETO"
echo "========================================================================"
