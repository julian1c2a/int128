#!/usr/bin/env bash
# Build script para int128_concepts_extracted_benchs.cpp con 4 compiladores

set -e

# Detectar directorio del script y directorio raíz del proyecto
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"

# Compilador a usar (por defecto: all)
COMPILER="${1:-all}"
COMPILER=$(echo "$COMPILER" | tr '[:upper:]' '[:lower:]')

# Mostrar timestamp
echo "========================================="
echo " BUILD: int128_concepts_extracted_benchs"
if [ "$COMPILER" != "all" ]; then
    echo " Compilador: $COMPILER"
fi
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="

# Archivo fuente
SOURCE_FILE="$PROJECT_ROOT/benchmarks/int128_concepts_extracted_benchs.cpp"

# Verificar que existe el archivo
if [ ! -f "$SOURCE_FILE" ]; then
    echo "❌ ERROR: No se encuentra el archivo $SOURCE_FILE"
    exit 1
fi

# Mostrar info del archivo
echo ""
echo "📄 Archivo fuente:"
ls -lh "$SOURCE_FILE" | awk '{print "   " $9 " (" $5 ")"}'
echo ""

# Flags comunes
COMMON_FLAGS="-std=c++20 -Wall -Wextra"
INCLUDE_DIR="-I$PROJECT_ROOT/include"

# ---------------------------------------
# 1. GCC
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "gcc" ]; then
    echo "🔨 [1/4] Compilando con GCC..."
    mkdir -p "$PROJECT_ROOT/build/int128_concepts_extracted_benchs/gcc/debug"
    mkdir -p "$PROJECT_ROOT/build/int128_concepts_extracted_benchs/gcc/release"

    if command -v g++ &> /dev/null; then
        g++ "$SOURCE_FILE" $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
            -o "$PROJECT_ROOT/build/int128_concepts_extracted_benchs/gcc/debug/int128_concepts_extracted_benchs.exe"
        echo "   ✅ GCC Debug OK"

        g++ "$SOURCE_FILE" $INCLUDE_DIR $COMMON_FLAGS -O3 -DNDEBUG \
            -o "$PROJECT_ROOT/build/int128_concepts_extracted_benchs/gcc/release/int128_concepts_extracted_benchs.exe"
        echo "   ✅ GCC Release OK"
    else
        echo "   ⚠️  GCC no encontrado."
    fi
fi

# ---------------------------------------
# 2. Clang
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "clang" ]; then
    echo ""
    echo "🔨 [2/4] Compilando con Clang..."
    mkdir -p "$PROJECT_ROOT/build/int128_concepts_extracted_benchs/clang/debug"
    mkdir -p "$PROJECT_ROOT/build/int128_concepts_extracted_benchs/clang/release"

    if command -v clang++ &> /dev/null; then
        clang++ "$SOURCE_FILE" $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
            -o "$PROJECT_ROOT/build/int128_concepts_extracted_benchs/clang/debug/int128_concepts_extracted_benchs.exe"
        echo "   ✅ Clang Debug OK"

        clang++ "$SOURCE_FILE" $INCLUDE_DIR $COMMON_FLAGS -O3 -DNDEBUG \
            -o "$PROJECT_ROOT/build/int128_concepts_extracted_benchs/clang/release/int128_concepts_extracted_benchs.exe"
        echo "   ✅ Clang Release OK"
    else
        echo "   ⚠️  Clang no encontrado."
    fi
fi

# ---------------------------------------
# 3. Intel ICX
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "intel" ]; then
    echo ""
    echo "🔨 [3/4] Compilando con Intel ICX..."

    # Verificar si icx está disponible
    if ! command -v icx &> /dev/null; then
        echo "   ⚠️  Intel ICX no disponible, omitiendo..."
    else
        mkdir -p "$PROJECT_ROOT/build/int128_concepts_extracted_benchs/intel/debug"
        mkdir -p "$PROJECT_ROOT/build/int128_concepts_extracted_benchs/intel/release"
        
        # Convertir rutas a formato Windows
        WIN_SOURCE=$(cygpath -m "$SOURCE_FILE")
        WIN_INCLUDE=$(cygpath -m "$PROJECT_ROOT/include")
        WIN_OUT_DEBUG=$(cygpath -m "$PROJECT_ROOT/build/int128_concepts_extracted_benchs/intel/debug/int128_concepts_extracted_benchs.exe")
        WIN_OUT_RELEASE=$(cygpath -m "$PROJECT_ROOT/build/int128_concepts_extracted_benchs/intel/release/int128_concepts_extracted_benchs.exe")
        
        export MSYS2_ARG_CONV_EXCL="*"
        
        # DEBUG
        icx "$WIN_SOURCE" -I"$WIN_INCLUDE" /std:c++20 /W3 /Zi /Od /EHsc /Zc:__cplusplus /D_HAS_CXX20=1 -Xclang -fcxx-exceptions \
            "/Fe:$WIN_OUT_DEBUG" > /dev/null
        if [ $? -eq 0 ]; then echo "   ✅ Intel ICX Debug OK"; else echo "   ❌ Intel ICX Debug FAILED"; fi
        
        # RELEASE
        icx "$WIN_SOURCE" -I"$WIN_INCLUDE" /std:c++20 /W3 /O2 /DNDEBUG /EHsc /Zc:__cplusplus /D_HAS_CXX20=1 -Xclang -fcxx-exceptions \
            "/Fe:$WIN_OUT_RELEASE" > /dev/null
        if [ $? -eq 0 ]; then echo "   ✅ Intel ICX Release OK"; else echo "   ❌ Intel ICX Release FAILED"; fi
        
        unset MSYS2_ARG_CONV_EXCL
    fi
fi

# ---------------------------------------
# 4. MSVC (cl.exe)
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "msvc" ]; then
    echo ""
    echo "🔨 [4/4] Compilando con MSVC..."

    if ! command -v cl &> /dev/null; then
        echo "   ⚠️  MSVC no disponible, omitiendo..."
    else
        mkdir -p "$PROJECT_ROOT/build/int128_concepts_extracted_benchs/msvc/debug"
        mkdir -p "$PROJECT_ROOT/build/int128_concepts_extracted_benchs/msvc/release"
        
        WIN_SOURCE=$(cygpath -m "$SOURCE_FILE")
        WIN_INCLUDE=$(cygpath -m "$PROJECT_ROOT/include")
        WIN_OUT_DEBUG=$(cygpath -m "$PROJECT_ROOT/build/int128_concepts_extracted_benchs/msvc/debug/int128_concepts_extracted_benchs.exe")
        WIN_OUT_RELEASE=$(cygpath -m "$PROJECT_ROOT/build/int128_concepts_extracted_benchs/msvc/release/int128_concepts_extracted_benchs.exe")
        
        export MSYS2_ARG_CONV_EXCL="*"
        
        # DEBUG
        cl "$WIN_SOURCE" /I"$WIN_INCLUDE" /std:c++20 /W4 /Zi /Od /EHsc \
            "/Fe:$WIN_OUT_DEBUG" > /dev/null
        echo "   ✅ MSVC Debug OK"
        
        # RELEASE
        cl "$WIN_SOURCE" /I"$WIN_INCLUDE" /std:c++20 /W4 /O2 /DNDEBUG /EHsc \
            "/Fe:$WIN_OUT_RELEASE" > /dev/null
        echo "   ✅ MSVC Release OK"
        
        unset MSYS2_ARG_CONV_EXCL
    fi
fi

echo ""
echo "========================================="
echo " ✅ COMPILACIÓN COMPLETADA"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="
