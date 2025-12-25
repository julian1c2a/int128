#!/bin/bash

# Script para compilar test_int128_cmath.cpp con múltiples compiladores
# Sigue la estructura modular: build/build_tests/{compiler}/{mode}/

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT" || exit 1

SOURCE_FILE="tests/test_int128_cmath.cpp"
INCLUDE_DIR="-Iinclude"
COMMON_FLAGS="-std=c++20 -Wall -Wextra"

echo "========================================================================"
echo "                  BUILD: int128_cmath_tests"
echo "                  Compilador: ${1:-all}"
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

# =============================================================================
# COMPILACIÓN CON GCC
# =============================================================================
if [ "$1" = "gcc" ] || [ "$1" = "all" ] || [ -z "$1" ]; then
    echo "[BUILD] [1/4] Compilando con GCC..."
    
    if ! command -v g++ &> /dev/null; then
        echo "   [WARN]  GCC no disponible"
    else
        # Debug
        mkdir -p "$PROJECT_ROOT/build/build_tests/gcc/debug"
        g++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
            -o "$PROJECT_ROOT/build/build_tests/gcc/debug/int128_cmath_tests_gcc" 2>&1
        result_debug=$?
        
        # Release
        mkdir -p "$PROJECT_ROOT/build/build_tests/gcc/release"
        g++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O2 -DNDEBUG \
            -o "$PROJECT_ROOT/build/build_tests/gcc/release/int128_cmath_tests_gcc" 2>&1
        result_release=$?
        
        if [ $result_debug -eq 0 ] && [ $result_release -eq 0 ]; then
            echo "   [OK] GCC Debug OK"
            echo "   [OK] GCC Release OK"
        elif [ $result_debug -eq 0 ]; then
            echo "   [OK] GCC Debug OK"
            echo "   [FAIL] GCC Release FAILED"
        elif [ $result_release -eq 0 ]; then
            echo "   [FAIL] GCC Debug FAILED"
            echo "   [OK] GCC Release OK"
        else
            echo "   [FAIL] GCC Debug FAILED"
            echo "   [FAIL] GCC Release FAILED"
        fi
    fi
fi

# =============================================================================
# COMPILACIÓN CON CLANG
# =============================================================================
if [ "$1" = "clang" ] || [ "$1" = "all" ] || [ -z "$1" ]; then
    echo "[BUILD] [2/4] Compilando con Clang..."
    
    if ! command -v clang++ &> /dev/null; then
        echo "   [WARN]  Clang no disponible"
    else
        # Debug
        mkdir -p "$PROJECT_ROOT/build/build_tests/clang/debug"
        clang++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
            -o "$PROJECT_ROOT/build/build_tests/clang/debug/int128_cmath_tests_clang" 2>&1
        result_debug=$?
        
        # Release
        mkdir -p "$PROJECT_ROOT/build/build_tests/clang/release"
        clang++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O2 -DNDEBUG \
            -o "$PROJECT_ROOT/build/build_tests/clang/release/int128_cmath_tests_clang" 2>&1
        result_release=$?
        
        if [ $result_debug -eq 0 ] && [ $result_release -eq 0 ]; then
            echo "   [OK] Clang Debug OK"
            echo "   [OK] Clang Release OK"
        elif [ $result_debug -eq 0 ]; then
            echo "   [OK] Clang Debug OK"
            echo "   [FAIL] Clang Release FAILED"
        elif [ $result_release -eq 0 ]; then
            echo "   [FAIL] Clang Debug FAILED"
            echo "   [OK] Clang Release OK"
        else
            echo "   [FAIL] Clang Debug FAILED"
            echo "   [FAIL] Clang Release FAILED"
        fi
    fi
fi

# =============================================================================
# COMPILACIÓN CON INTEL ICX
# =============================================================================
if [ "$1" = "intel" ] || [ "$1" = "all" ] || [ -z "$1" ]; then
    echo "[BUILD] [3/4] Compilando con Intel ICX..."
    
    if ! command -v icpx &> /dev/null; then
        echo "   [WARN]  Intel ICX no disponible"
    else
        # Debug
        mkdir -p "$PROJECT_ROOT/build/build_tests/intel/debug"
        icpx $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
            -o "$PROJECT_ROOT/build/build_tests/intel/debug/int128_cmath_tests_intel" 2>&1
        result_debug=$?
        
        # Release
        mkdir -p "$PROJECT_ROOT/build/build_tests/intel/release"
        icpx $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O2 -DNDEBUG \
            -o "$PROJECT_ROOT/build/build_tests/intel/release/int128_cmath_tests_intel" 2>&1
        result_release=$?
        
        if [ $result_debug -eq 0 ] && [ $result_release -eq 0 ]; then
            echo "   [OK] Intel Debug OK"
            echo "   [OK] Intel Release OK"
        elif [ $result_debug -eq 0 ]; then
            echo "   [OK] Intel Debug OK"
            echo "   [FAIL] Intel Release FAILED"
        elif [ $result_release -eq 0 ]; then
            echo "   [FAIL] Intel Debug FAILED"
            echo "   [OK] Intel Release OK"
        else
            echo "   [FAIL] Intel Debug FAILED"
            echo "   [FAIL] Intel Release FAILED"
        fi
    fi
fi

# =============================================================================
# COMPILACIÓN CON MSVC
# =============================================================================
if [ "$1" = "msvc" ] || [ "$1" = "all" ] || [ -z "$1" ]; then
    echo "[BUILD] [4/4] Compilando con MSVC..."
    
    if ! command -v cl.exe &> /dev/null; then
        echo "   [WARN]  MSVC no disponible"
    else
        # Proteger argumentos de MSYS2 path conversion
        export MSYS2_ARG_CONV_EXCL="*"
        
        # Debug
        mkdir -p "$PROJECT_ROOT/build/build_tests/msvc/debug"
        cl.exe /std:c++20 /Zi /Od /MDd /EHsc /W4 \
            /I"include" \
            "$SOURCE_FILE" \
            /Fe:"$PROJECT_ROOT/build/build_tests/msvc/debug/int128_cmath_tests_msvc.exe" \
            /Fo:"$PROJECT_ROOT/build/build_tests/msvc/debug/" 2>&1 | grep -E "(error|warning|Generating)"
        result_debug=$?
        
        # Limpiar archivos intermedios
        rm -f "$PROJECT_ROOT/build/build_tests/msvc/debug/"*.obj 2>/dev/null
        rm -f "$PROJECT_ROOT/build/build_tests/msvc/debug/"*.pdb 2>/dev/null
        
        # Release
        mkdir -p "$PROJECT_ROOT/build/build_tests/msvc/release"
        cl.exe /std:c++20 /O2 /MD /EHsc /W4 /DNDEBUG \
            /I"include" \
            "$SOURCE_FILE" \
            /Fe:"$PROJECT_ROOT/build/build_tests/msvc/release/int128_cmath_tests_msvc.exe" \
            /Fo:"$PROJECT_ROOT/build/build_tests/msvc/release/" 2>&1 | grep -E "(error|warning|Generating)"
        result_release=$?
        
        # Limpiar archivos intermedios
        rm -f "$PROJECT_ROOT/build/build_tests/msvc/release/"*.obj 2>/dev/null
        rm -f "$PROJECT_ROOT/build/build_tests/msvc/release/"*.pdb 2>/dev/null
        
        # Restaurar comportamiento de path conversion
        unset MSYS2_ARG_CONV_EXCL
        
        if [ $result_debug -eq 0 ] && [ $result_release -eq 0 ]; then
            echo "   [OK] MSVC Debug OK"
            echo "   [OK] MSVC Release OK"
        elif [ $result_debug -eq 0 ]; then
            echo "   [OK] MSVC Debug OK"
            echo "   [FAIL] MSVC Release FAILED"
        elif [ $result_release -eq 0 ]; then
            echo "   [FAIL] MSVC Debug FAILED"
            echo "   [OK] MSVC Release OK"
        else
            echo "   [FAIL] MSVC Debug FAILED"
            echo "   [FAIL] MSVC Release FAILED"
        fi
    fi
fi

echo ""
echo "========================================================================"
echo "                  [OK] COMPILACIÓN COMPLETADA"
echo "                  $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================================================"
