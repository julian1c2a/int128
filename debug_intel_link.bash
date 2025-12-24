#!/usr/bin/env bash
# Script temporal para diagnosticar el problema de linkado con Intel ICX

set -e

PROJECT_ROOT="/c/msys64/ucrt64/home/julian/CppProjects/int128"
SOURCE_FILE="$PROJECT_ROOT/tests/uint128_t_extracted_tests.cpp"

# Verificar que icx esté disponible
if ! command -v icx &> /dev/null; then
    echo "❌ ERROR: icx no está disponible en el PATH"
    echo "PATH actual:"
    echo "$PATH" | tr ':' '\n' | grep -i intel
    exit 1
fi

echo "✅ icx encontrado: $(which icx)"
echo "Versión: $(icx --version | head -1)"
echo ""

WIN_SOURCE=$(cygpath -m "$SOURCE_FILE")
WIN_INCLUDE=$(cygpath -m "$PROJECT_ROOT/include")
WIN_OUT=$(cygpath -m "$PROJECT_ROOT/build/build_tests/intel/debug/uint128_t_extracted_tests_test.exe")

export MSYS2_ARG_CONV_EXCL="*"
ICX_FLAGS="/std:c++20 /W3 /EHsc /Zc:__cplusplus /D_HAS_CXX20=1 -Xclang -fcxx-exceptions"

echo "Compilando con Intel ICX (Debug)..."
echo "Comando: icx \"$WIN_SOURCE\" -I\"$WIN_INCLUDE\" $ICX_FLAGS /Zi /Od \"/Fe:$WIN_OUT\""
echo ""

# Ejecutar sin redireccionar para ver todos los mensajes
icx "$WIN_SOURCE" -I"$WIN_INCLUDE" $ICX_FLAGS /Zi /Od "/Fe:$WIN_OUT" 2>&1

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ Compilación exitosa"
    ls -lh "$PROJECT_ROOT/build/build_tests/intel/debug/uint128_t_extracted_tests_test.exe"
else
    echo ""
    echo "❌ Compilación fallida"
    exit 1
fi
