#!/usr/bin/env bash
# Build script para uint128_limits_extracted_tests.cpp con 4 compiladores

set -u

# Detectar directorio del script y directorio raíz del proyecto
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"

# Compilador a usar (por defecto: all)
COMPILER="${1:-all}"
COMPILER=$(echo "$COMPILER" | tr '[:upper:]' '[:lower:]')

# Mostrar timestamp
echo "========================================="
echo " BUILD: uint128_limits_extracted_tests"
if [ "$COMPILER" != "all" ]; then
    echo " Compilador: $COMPILER"
fi
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="

# Archivo fuente
SOURCE_FILE="$PROJECT_ROOT/tests/uint128_limits_extracted_tests.cpp"

if [ ! -f "$SOURCE_FILE" ]; then
    echo "❌ ERROR: No se encuentra el archivo $SOURCE_FILE"
    exit 1
fi

echo ""
echo "📄 Archivo fuente:"
ls -lh "$SOURCE_FILE" | awk '{print "   " $9 " (" $5 ")"}'
echo ""

# Flags comunes GCC/Clang
COMMON_FLAGS="-std=c++20 -Wall -Wextra"
INCLUDE_DIR="-I$PROJECT_ROOT/include"

# ---------------------------------------
# 1. GCC
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "gcc" ]; then
    echo "🔨 [1/4] Compilando con GCC..."
    mkdir -p "$PROJECT_ROOT/build/uint128_limits_extracted_tests/gcc/debug"
    mkdir -p "$PROJECT_ROOT/build/uint128_limits_extracted_tests/gcc/release"

    if command -v g++ &> /dev/null; then
        g++ "$SOURCE_FILE" $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
            -o "$PROJECT_ROOT/build/uint128_limits_extracted_tests/gcc/debug/uint128_limits_extracted_tests.exe"
        echo "   ✅ GCC Debug OK"

        g++ "$SOURCE_FILE" $INCLUDE_DIR $COMMON_FLAGS -O3 -DNDEBUG \
            -o "$PROJECT_ROOT/build/uint128_limits_extracted_tests/gcc/release/uint128_limits_extracted_tests.exe"
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
    mkdir -p "$PROJECT_ROOT/build/uint128_limits_extracted_tests/clang/debug"
    mkdir -p "$PROJECT_ROOT/build/uint128_limits_extracted_tests/clang/release"

    if command -v clang++ &> /dev/null; then
        clang++ "$SOURCE_FILE" $INCLUDE_DIR $COMMON_FLAGS -g -O0 \
            -o "$PROJECT_ROOT/build/uint128_limits_extracted_tests/clang/debug/uint128_limits_extracted_tests.exe"
        echo "   ✅ Clang Debug OK"

        clang++ "$SOURCE_FILE" $INCLUDE_DIR $COMMON_FLAGS -O3 -DNDEBUG \
            -o "$PROJECT_ROOT/build/uint128_limits_extracted_tests/clang/release/uint128_limits_extracted_tests.exe"
        echo "   ✅ Clang Release OK"
    else
        echo "   ⚠️  Clang no encontrado."
    fi
fi

# ---------------------------------------
# 3. Intel ICX (Modo Windows)
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "intel" ]; then
    echo ""
    echo "🔨 [3/4] Compilando con Intel OneAPI (icx)..."

    # Usamos explícitamente icx porque es el driver compatible con cl.exe
    INTEL_CMD="icx"

    if ! command -v "$INTEL_CMD" &> /dev/null; then
        echo "   ⚠️  Intel compilador (icx) no disponible."
    else
        mkdir -p "$PROJECT_ROOT/build/uint128_limits_extracted_tests/intel/debug"
        mkdir -p "$PROJECT_ROOT/build/uint128_limits_extracted_tests/intel/release"
        
        # 1. Convertir rutas a formato Windows Mixed (C:/...)
        WIN_SOURCE=$(cygpath -m "$SOURCE_FILE")
        WIN_INCLUDE=$(cygpath -m "$PROJECT_ROOT/include")
        WIN_OUT_DEBUG=$(cygpath -m "$PROJECT_ROOT/build/uint128_limits_extracted_tests/intel/debug/uint128_limits_extracted_tests.exe")
        WIN_OUT_RELEASE=$(cygpath -m "$PROJECT_ROOT/build/uint128_limits_extracted_tests/intel/release/uint128_limits_extracted_tests.exe")

        # 2. Desactivar conversión de argumentos de MSYS2
        export MSYS2_ARG_CONV_EXCL="*"
        
        # 3. Flags Windows para icx
        # Nota: -Xclang -fcxx-exceptions sigue siendo necesario para habilitar try/catch correctamente
        ICX_FLAGS_BASE="/std:c++20 /W3 /EHsc /Zc:__cplusplus /D_HAS_CXX20=1 -Xclang -fcxx-exceptions"
        
        # DEBUG
        "$INTEL_CMD" "$WIN_SOURCE" -I"$WIN_INCLUDE" $ICX_FLAGS_BASE /Zi /Od \
            "/Fe:$WIN_OUT_DEBUG" > /dev/null
        if [ $? -eq 0 ]; then echo "   ✅ Intel Debug OK"; else echo "   ❌ Intel Debug FAILED"; fi
        
        # RELEASE
        "$INTEL_CMD" "$WIN_SOURCE" -I"$WIN_INCLUDE" $ICX_FLAGS_BASE /O2 /DNDEBUG \
            "/Fe:$WIN_OUT_RELEASE" > /dev/null
        if [ $? -eq 0 ]; then echo "   ✅ Intel Release OK"; else echo "   ❌ Intel Release FAILED"; fi
        
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
        mkdir -p "$PROJECT_ROOT/build/build_tests/msvc/debug"
        mkdir -p "$PROJECT_ROOT/build/build_tests/msvc/release"
        
        INCLUDE_DIR="./include"
        TEST_SRC="tests/uint128_limits_extracted_tests.cpp"
        
        export MSYS2_ARG_CONV_EXCL="*"
        
        # DEBUG
        cl.exe /std:c++20 /W4 /EHsc /Od /Zi \
            /I"${INCLUDE_DIR}" \
            "${TEST_SRC}" \
            /Fe"build/build_tests/msvc/debug/uint128_limits_tests_msvc.exe" \
            > /dev/null 2>&1
        
        result_debug=$?
        
        if [ $result_debug -eq 0 ]; then
            echo "   ✅ MSVC Debug OK"
            rm -f build/build_tests/msvc/debug/*.obj build/build_tests/msvc/debug/*.pdb
        else
            echo "   ❌ MSVC Debug FAILED"
        fi
        
        # RELEASE
        cl.exe /std:c++20 /W4 /EHsc /O2 /DNDEBUG \
            /I"${INCLUDE_DIR}" \
            "${TEST_SRC}" \
            /Fe"build/build_tests/msvc/release/uint128_limits_tests_msvc.exe" \
            > /dev/null 2>&1
        
        result_release=$?
        
        if [ $result_release -eq 0 ]; then
            echo "   ✅ MSVC Release OK"
            rm -f build/build_tests/msvc/release/*.obj build/build_tests/msvc/release/*.pdb
        else
            echo "   ❌ MSVC Release FAILED"
        fi
        
        unset MSYS2_ARG_CONV_EXCL
    fi
fi

echo ""
echo "========================================="
echo " ✅ COMPILACIÓN COMPLETADA"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="