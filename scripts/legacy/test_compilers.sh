#!/bin/bash
echo "=== Test de Compatibilidad Multiplataforma ==="

# Test con MSVC (Windows)
if command -v cl.exe &> /dev/null; then
    echo "🔧 Probando con Microsoft Visual C++..."
    cd "/c/msys64/ucrt64/home/julian/CppProjects/int128"
    cl.exe /std:c++17 /EHsc /I. test_cross_compiler.cpp /Fe:test_cross_compiler_msvc.exe
    if [ $? -eq 0 ]; then
        echo "✓ Compilación MSVC exitosa"
        ./test_cross_compiler_msvc.exe
        echo ""
    else
        echo "❌ Error en compilación MSVC"
    fi
fi

# Test con GCC (si está disponible)
if command -v g++ &> /dev/null; then
    echo "🔧 Probando con GCC..."
    cd "/c/msys64/ucrt64/home/julian/CppProjects/int128"
    g++ -std=c++17 -O2 -I. test_cross_compiler.cpp -o test_cross_compiler_gcc.exe
    if [ $? -eq 0 ]; then
        echo "✓ Compilación GCC exitosa"
        ./test_cross_compiler_gcc.exe
        echo ""
    else
        echo "❌ Error en compilación GCC"
    fi
fi

# Test con Clang (si está disponible)
if command -v clang++ &> /dev/null; then
    echo "🔧 Probando con Clang..."
    cd "/c/msys64/ucrt64/home/julian/CppProjects/int128"
    clang++ -std=c++17 -O2 -I. test_cross_compiler.cpp -o test_cross_compiler_clang.exe
    if [ $? -eq 0 ]; then
        echo "✓ Compilación Clang exitosa"
        ./test_cross_compiler_clang.exe
        echo ""
    else
        echo "❌ Error en compilación Clang"
    fi
fi

echo "🎯 Test de compatibilidad multiplataforma completado."