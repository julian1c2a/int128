#!/usr/bin/env bash
# Build script para int128_concepts_extracted_tests.cpp con 4 compiladores

set -u

# Detectar directorio del script y directorio raíz del proyecto
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"

# Validar argumentos
if [ $# -lt 2 ]; then
    echo "[FAIL] ERROR: Se requieren al menos 2 argumentos"
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
    echo "[FAIL] ERROR: Compilador inválido: $COMPILER"
    echo "Compiladores válidos: intel, msvc, gcc, clang, all"
    exit 1
fi

# Validar modo
if [[ ! "$MODE" =~ ^(debug|release|all)$ ]]; then
    echo "[FAIL] ERROR: Modo inválido: $MODE"
    echo "Modos válidos: debug, release, all"
    exit 1
fi

# Mostrar timestamp
echo "========================================="
echo " BUILD: int128_concepts_extracted_tests"
echo " Compilador: $COMPILER"
echo " Modo: $MODE"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="

# Archivo fuente
SOURCE_FILE="$PROJECT_ROOT/tests/int128_concepts_extracted_tests.cpp"

if [ ! -f "$SOURCE_FILE" ]; then
    echo "[FAIL] ERROR: No se encuentra el archivo $SOURCE_FILE"
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
    echo "[BUILD] [GCC] Compilando..."
    
    if ! command -v g++ &> /dev/null; then
        echo "   [WARN]  GCC no encontrado."
    else
        if [ "$MODE" = "all" ] || [ "$MODE" = "debug" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_tests/gcc/debug"
            g++ "$SOURCE_FILE" $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
                -o "$PROJECT_ROOT/build/build_tests/gcc/debug/int128_concepts_extracted_tests.exe"
            echo "   [OK] GCC Debug OK"
        fi
        
        if [ "$MODE" = "all" ] || [ "$MODE" = "release" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_tests/gcc/release"
            g++ "$SOURCE_FILE" $INCLUDE_DIR $COMMON_FLAGS -O3 -DNDEBUG \
                -o "$PROJECT_ROOT/build/build_tests/gcc/release/int128_concepts_extracted_tests.exe"
            echo "   [OK] GCC Release OK"
        fi
    fi
fi

# ---------------------------------------
# 2. Clang
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "clang" ]; then
    echo ""
    echo "[BUILD] [Clang] Compilando..."
    
    if ! command -v clang++ &> /dev/null; then
        echo "   [WARN]  Clang no encontrado."
    else
        if [ "$MODE" = "all" ] || [ "$MODE" = "debug" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_tests/clang/debug"
            clang++ "$SOURCE_FILE" $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
                -o "$PROJECT_ROOT/build/build_tests/clang/debug/int128_concepts_extracted_tests.exe"
            echo "   [OK] Clang Debug OK"
        fi
        
        if [ "$MODE" = "all" ] || [ "$MODE" = "release" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_tests/clang/release"
            clang++ "$SOURCE_FILE" $INCLUDE_DIR $COMMON_FLAGS -O3 -DNDEBUG \
                -o "$PROJECT_ROOT/build/build_tests/clang/release/int128_concepts_extracted_tests.exe"
            echo "   [OK] Clang Release OK"
        fi
    fi
fi

# ---------------------------------------
# 3. Intel ICX
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "intel" ]; then
    echo ""
    echo "[BUILD] [Intel] Compilando..."

    INTEL_CMD="icx"

    if ! command -v "$INTEL_CMD" &> /dev/null; then
        echo "   [WARN]  Intel compilador (icx) no disponible."
    else
        WIN_SOURCE=$(cygpath -m "$SOURCE_FILE")
        WIN_INCLUDE=$(cygpath -m "$PROJECT_ROOT/include")
        
        export MSYS2_ARG_CONV_EXCL="*"
        ICX_FLAGS_BASE="/std:c++20 /W3 /EHsc /Zc:__cplusplus /D_HAS_CXX20=1 -Xclang -fcxx-exceptions"
        
        if [ "$MODE" = "all" ] || [ "$MODE" = "debug" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_tests/intel/debug"
            WIN_OUT_DEBUG=$(cygpath -m "$PROJECT_ROOT/build/build_tests/intel/debug/int128_concepts_extracted_tests.exe")
            "$INTEL_CMD" "$WIN_SOURCE" -I"$WIN_INCLUDE" $ICX_FLAGS_BASE /Zi /Od \
                "/Fe:$WIN_OUT_DEBUG" > /dev/null
            if [ $? -eq 0 ]; then echo "   [OK] Intel Debug OK"; else echo "   [FAIL] Intel Debug FAILED"; fi
        fi
        
        if [ "$MODE" = "all" ] || [ "$MODE" = "release" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_tests/intel/release"
            WIN_OUT_RELEASE=$(cygpath -m "$PROJECT_ROOT/build/build_tests/intel/release/int128_concepts_extracted_tests.exe")
            "$INTEL_CMD" "$WIN_SOURCE" -I"$WIN_INCLUDE" $ICX_FLAGS_BASE /O2 /DNDEBUG \
                "/Fe:$WIN_OUT_RELEASE" > /dev/null
            if [ $? -eq 0 ]; then echo "   [OK] Intel Release OK"; else echo "   [FAIL] Intel Release FAILED"; fi
        fi
        
        unset MSYS2_ARG_CONV_EXCL
    fi
fi

# ---------------------------------------
# 4. MSVC
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "msvc" ]; then
    echo ""
    echo "[BUILD] [MSVC] Compilando..."

    if ! command -v cl &> /dev/null; then
        echo "   [WARN]  MSVC no disponible."
    else
        WIN_SOURCE=$(cygpath -m "$SOURCE_FILE")
        WIN_INCLUDE=$(cygpath -m "$PROJECT_ROOT/include")
        
        export MSYS2_ARG_CONV_EXCL="*"
        
        if [ "$MODE" = "all" ] || [ "$MODE" = "debug" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_tests/msvc/debug"
            WIN_OUT_DEBUG=$(cygpath -m "$PROJECT_ROOT/build/build_tests/msvc/debug/int128_concepts_extracted_tests.exe")
            cl "$WIN_SOURCE" /I"$WIN_INCLUDE" /std:c++20 /W4 /Zi /Od /EHsc \
                "/Fe:$WIN_OUT_DEBUG" > /dev/null
            echo "   [OK] MSVC Debug OK"
        fi
        
        if [ "$MODE" = "all" ] || [ "$MODE" = "release" ]; then
            mkdir -p "$PROJECT_ROOT/build/build_tests/msvc/release"
            WIN_OUT_RELEASE=$(cygpath -m "$PROJECT_ROOT/build/build_tests/msvc/release/int128_concepts_extracted_tests.exe")
            cl "$WIN_SOURCE" /I"$WIN_INCLUDE" /std:c++20 /W4 /O2 /DNDEBUG /EHsc \
                "/Fe:$WIN_OUT_RELEASE" > /dev/null
            echo "   [OK] MSVC Release OK"
        fi
        
        unset MSYS2_ARG_CONV_EXCL
    fi
fi

echo ""
echo "========================================="
echo " [OK] COMPILACIÓN COMPLETADA"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================"
