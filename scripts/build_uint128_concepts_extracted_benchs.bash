#!/usr/bin/env bash
# Build script para uint128_concepts_extracted_benchs.cpp con 4 compiladores

set -u

# Detectar directorio del script y directorio raíz del proyecto
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"

# Validar argumentos
if [ $# -lt 2 ]; then
    echo "❌ ERROR: Se requieren al menos 2 argumentos"
    echo "Uso: $0 [compiler] [mode] [print]"
    echo "  compiler: intel, msvc, gcc, clang, all"
    echo "  mode: debug, release, all"
    echo "  print: opcional, cualquier valor para activar"
    exit 1
fi

# Compilador y modo
COMPILER="${1}"
MODE="${2}"
PRINT_MODE="${3:-}"

COMPILER=$(echo "$COMPILER" | tr '[:upper:]' '[:lower:]')
MODE=$(echo "$MODE" | tr '[:upper:]' '[:lower:]')

# Validar compilador
if [[ ! "$COMPILER" =~ ^(intel|msvc|gcc|clang|all)$ ]]; then
    echo "❌ ERROR: Compilador inválido: $COMPILER"
    echo "Compiladores válidos: intel, msvc, gcc, clang, all"
    exit 1
fi

# Validar modo
if [[ ! "$MODE" =~ ^(debug|release|all)$ ]]; then
    echo "❌ ERROR: Modo inválido: $MODE"
    echo "Modos válidos: debug, release, all"
    exit 1
fi

# Mostrar timestamp
echo "========================================="
echo " BUILD: uint128_concepts_extracted_benchs"
echo " Compilador: $COMPILER"
echo " Modo: $MODE"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="

# Archivo fuente
SOURCE_FILE="$PROJECT_ROOT/benchmarks/uint128_concepts_extracted_benchs.cpp"

if [ ! -f "$SOURCE_FILE" ]; then
    echo "❌ ERROR: No se encuentra el archivo $SOURCE_FILE"
    exit 1
fi

if [ -n "$PRINT_MODE" ]; then
    echo ""
    echo "📄 Archivo fuente:"
    ls -lh "$SOURCE_FILE" | awk '{print "   " $9 " (" $5 ")"}'
    echo ""
fi

# Flags comunes GCC/Clang
COMMON_FLAGS="-std=c++20 -Wall -Wextra"
INCLUDE_DIR="-I$PROJECT_ROOT/include"

# ---------------------------------------
# 1. GCC
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "gcc" ]; then
    echo "🔨 [GCC] Compilando..."
    
    if ! command -v g++ &> /dev/null; then
        echo "   ⚠️  GCC no encontrado."
    else
        if [ "$MODE" = "all" ] || [ "$MODE" = "debug" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_benchmarks/gcc/debug"
            g++ "$SOURCE_FILE" $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
                -o "$PROJECT_ROOT/build/build_benchmarks/gcc/debug/uint128_concepts_extracted_benchs.exe"
            echo "   ✅ GCC Debug OK"
        fi
        
        if [ "$MODE" = "all" ] || [ "$MODE" = "release" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_benchmarks/gcc/release"
            g++ "$SOURCE_FILE" $INCLUDE_DIR $COMMON_FLAGS -O3 -DNDEBUG \
                -o "$PROJECT_ROOT/build/build_benchmarks/gcc/release/uint128_concepts_extracted_benchs.exe"
            echo "   ✅ GCC Release OK"
        fi
    fi
fi

# ---------------------------------------
# 2. Clang
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "clang" ]; then
    echo ""
    echo "🔨 [Clang] Compilando..."
    
    if ! command -v clang++ &> /dev/null; then
        echo "   ⚠️  Clang no encontrado."
    else
        if [ "$MODE" = "all" ] || [ "$MODE" = "debug" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_benchmarks/clang/debug"
            clang++ "$SOURCE_FILE" $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
                -o "$PROJECT_ROOT/build/build_benchmarks/clang/debug/uint128_concepts_extracted_benchs.exe"
            echo "   ✅ Clang Debug OK"
        fi
        
        if [ "$MODE" = "all" ] || [ "$MODE" = "release" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_benchmarks/clang/release"
            clang++ "$SOURCE_FILE" $INCLUDE_DIR $COMMON_FLAGS -O3 -DNDEBUG \
                -o "$PROJECT_ROOT/build/build_benchmarks/clang/release/uint128_concepts_extracted_benchs.exe"
            echo "   ✅ Clang Release OK"
        fi
    fi
fi

# ---------------------------------------
# 3. Intel ICX
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "intel" ]; then
    echo ""
    echo "🔨 [Intel] Compilando..."

    INTEL_CMD="icx"

    if ! command -v "$INTEL_CMD" &> /dev/null; then
        echo "   ⚠️  Intel compilador (icx) no disponible."
    else
        WIN_SOURCE=$(cygpath -m "$SOURCE_FILE")
        WIN_INCLUDE=$(cygpath -m "$PROJECT_ROOT/include")
        
        export MSYS2_ARG_CONV_EXCL="*"
        ICX_FLAGS_BASE="/std:c++20 /W3 /EHsc /Zc:__cplusplus /D_HAS_CXX20=1 -Xclang -fcxx-exceptions"
        
        if [ "$MODE" = "all" ] || [ "$MODE" = "debug" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_benchmarks/intel/debug"
            WIN_OUT_DEBUG=$(cygpath -m "$PROJECT_ROOT/build/build_benchmarks/intel/debug/uint128_concepts_extracted_benchs.exe")
            "$INTEL_CMD" "$WIN_SOURCE" -I"$WIN_INCLUDE" $ICX_FLAGS_BASE /Zi /Od \
                "/Fe:$WIN_OUT_DEBUG" > /dev/null
            if [ $? -eq 0 ]; then echo "   ✅ Intel Debug OK"; else echo "   ❌ Intel Debug FAILED"; fi
        fi
        
        if [ "$MODE" = "all" ] || [ "$MODE" = "release" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_benchmarks/intel/release"
            WIN_OUT_RELEASE=$(cygpath -m "$PROJECT_ROOT/build/build_benchmarks/intel/release/uint128_concepts_extracted_benchs.exe")
            "$INTEL_CMD" "$WIN_SOURCE" -I"$WIN_INCLUDE" $ICX_FLAGS_BASE /O2 /DNDEBUG \
                "/Fe:$WIN_OUT_RELEASE" > /dev/null
            if [ $? -eq 0 ]; then echo "   ✅ Intel Release OK"; else echo "   ❌ Intel Release FAILED"; fi
        fi
        
        unset MSYS2_ARG_CONV_EXCL
    fi
fi

# ---------------------------------------
# 4. MSVC
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "msvc" ]; then
    echo ""
    echo "🔨 [MSVC] Compilando..."

    if ! command -v cl &> /dev/null; then
        echo "   ⚠️  MSVC no disponible."
    else
        WIN_SOURCE=$(cygpath -m "$SOURCE_FILE")
        WIN_INCLUDE=$(cygpath -m "$PROJECT_ROOT/include")
        
        export MSYS2_ARG_CONV_EXCL="*"
        
        if [ "$MODE" = "all" ] || [ "$MODE" = "debug" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_benchmarks/msvc/debug"
            WIN_OUT_DEBUG=$(cygpath -m "$PROJECT_ROOT/build/build_benchmarks/msvc/debug/uint128_concepts_extracted_benchs.exe")
            cl "$WIN_SOURCE" /I"$WIN_INCLUDE" /std:c++20 /W4 /Zi /Od /EHsc \
                "/Fe:$WIN_OUT_DEBUG" > /dev/null
            echo "   ✅ MSVC Debug OK"
        fi
        
        if [ "$MODE" = "all" ] || [ "$MODE" = "release" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_benchmarks/msvc/release"
            WIN_OUT_RELEASE=$(cygpath -m "$PROJECT_ROOT/build/build_benchmarks/msvc/release/uint128_concepts_extracted_benchs.exe")
            cl "$WIN_SOURCE" /I"$WIN_INCLUDE" /std:c++20 /W4 /O2 /DNDEBUG /EHsc \
                "/Fe:$WIN_OUT_RELEASE" > /dev/null
            echo "   ✅ MSVC Release OK"
        fi
        
        unset MSYS2_ARG_CONV_EXCL
    fi
fi

echo ""
echo "========================================="
echo " ✅ COMPILACIÓN COMPLETADA"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================"
