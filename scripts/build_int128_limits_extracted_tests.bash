#!/usr/bin/env bash
# Build script para int128_limits_extracted_tests.cpp con 4 compiladores

set -e

# Detectar directorio del script y directorio raíz del proyecto
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"

# Compilador a usar (por defecto: all)
COMPILER="${1:-all}"

# Mostrar timestamp
echo "========================================="
echo " BUILD: int128_limits_extracted_tests"
if [ "$COMPILER" != "all" ]; then
    echo " Compilador: $COMPILER"
fi
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="

# Archivo fuente
SOURCE_FILE="$PROJECT_ROOT/tests/int128_limits_extracted_tests.cpp"

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
mkdir -p "$PROJECT_ROOT/build/int128_limits_extracted_tests/gcc/debug"
mkdir -p "$PROJECT_ROOT/build/int128_limits_extracted_tests/gcc/release"

g++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
    -o "$PROJECT_ROOT/build/int128_limits_extracted_tests/gcc/debug/int128_limits_extracted_tests"
echo "   ✅ GCC Debug: build/int128_limits_extracted_tests/gcc/debug/int128_limits_extracted_tests"

g++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O3 -DNDEBUG \
    -o "$PROJECT_ROOT/build/int128_limits_extracted_tests/gcc/release/int128_limits_extracted_tests"
echo "   ✅ GCC Release: build/int128_limits_extracted_tests/gcc/release/int128_limits_extracted_tests"
fi

# ---------------------------------------
# 2. Clang
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "clang" ]; then
echo ""
echo "🔨 [2/4] Compilando con Clang..."
mkdir -p "$PROJECT_ROOT/build/int128_limits_extracted_tests/clang/debug"
mkdir -p "$PROJECT_ROOT/build/int128_limits_extracted_tests/clang/release"

clang++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
    -o "$PROJECT_ROOT/build/int128_limits_extracted_tests/clang/debug/int128_limits_extracted_tests"
echo "   ✅ Clang Debug: build/int128_limits_extracted_tests/clang/debug/int128_limits_extracted_tests"

clang++ $SOURCE_FILE $INCLUDE_DIR $COMMON_FLAGS -O3 -DNDEBUG \
    -o "$PROJECT_ROOT/build/int128_limits_extracted_tests/clang/release/int128_limits_extracted_tests"
echo "   ✅ Clang Release: build/int128_limits_extracted_tests/clang/release/int128_limits_extracted_tests"
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
    # Intel ICX necesita rutas sin espacios y formato Windows
    # Cambiar al directorio del proyecto para usar paths relativos
    cd "$PROJECT_ROOT"
    
    mkdir -p "build/int128_limits_extracted_tests/intel/debug"
    mkdir -p "build/int128_limits_extracted_tests/intel/release"
    
    # Evitar conversión de paths de MSYS2
    export MSYS2_ARG_CONV_EXCL="*"
    
    icx tests/int128_limits_extracted_tests.cpp -Iinclude /std:c++20 /W4 /Zi /Od /EHsc \
        /Fe:build/int128_limits_extracted_tests/intel/debug/int128_limits_extracted_tests.exe
    echo "   ✅ Intel ICX Debug: build/int128_limits_extracted_tests/intel/debug/int128_limits_extracted_tests.exe"
    
    icx tests/int128_limits_extracted_tests.cpp -Iinclude /std:c++20 /W4 /O3 /DNDEBUG /EHsc \
        /Fe:build/int128_limits_extracted_tests/intel/release/int128_limits_extracted_tests.exe
    echo "   ✅ Intel ICX Release: build/int128_limits_extracted_tests/intel/release/int128_limits_extracted_tests.exe"
    
    # Volver al directorio del script
    cd "$SCRIPT_DIR"
fi
fi

# ---------------------------------------
# 4. MSVC (cl.exe)
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "msvc" ]; then
echo ""
echo "🔨 [4/4] Compilando con MSVC..."

# Verificar si cl está disponible
if ! command -v cl &> /dev/null; then
    echo "   ⚠️  MSVC no disponible, omitiendo..."
else
    MSVC_SOURCE="$PROJECT_ROOT/tests/int128_limits_extracted_tests.cpp"
    MSVC_INCLUDE="/I$PROJECT_ROOT/include"
    
    # Cambiar al directorio del proyecto
    cd "$PROJECT_ROOT"
    
    mkdir -p "build/int128_limits_extracted_tests/msvc/debug"
    mkdir -p "build/int128_limits_extracted_tests/msvc/release"
    
    # Evitar conversión de paths
    export MSYS2_ARG_CONV_EXCL="*"
    
    cl tests/int128_limits_extracted_tests.cpp /Iinclude /std:c++20 /W4 /Zi /Od /EHsc \
        /Fe:build/int128_limits_extracted_tests/msvc/debug/int128_limits_extracted_tests.exe
    echo "   ✅ MSVC Debug: build/int128_limits_extracted_tests/msvc/debug/int128_limits_extracted_tests.exe"
    
    cl tests/int128_limits_extracted_tests.cpp /Iinclude /std:c++20 /W4 /O2 /DNDEBUG /EHsc \
        /Fe:build/int128_limits_extracted_tests/msvc/release/int128_limits_extracted_tests.exe
    echo "   ✅ MSVC Release: build/int128_limits_extracted_tests/msvc/release/int128_limits_extracted_tests.exe"
    
    # Volver al directorio del script
    cd "$SCRIPT_DIR"
fi
fi

echo ""
echo "========================================="
echo " ✅ COMPILACIÓN COMPLETADA"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="
