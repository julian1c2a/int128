#!/usr/bin/env bash
# Script de diagnóstico para problemas de compilación

echo "========================================="
echo " Diagnóstico de Entorno"
echo "========================================="
echo ""

echo "1. Shell actual:"
echo "   SHELL: $SHELL"
echo "   TERM: $TERM"
echo ""

echo "2. Compilador Clang:"
which clang++
clang++ --version | head -3
echo ""

echo "3. Variables MSYSTEM:"
echo "   MSYSTEM: $MSYSTEM"
echo "   MSYSTEM_PREFIX: $MSYSTEM_PREFIX"
echo "   MSYSTEM_CARCH: $MSYSTEM_CARCH"
echo ""

echo "4. PATH (primeras 5 entradas):"
echo "$PATH" | tr ':' '\n' | head -5
echo ""

echo "5. Grep version:"
grep --version | head -1
echo ""

echo "6. Test de compilación directa (sin make):"
cd "$(dirname "$0")/.."
rm -f build/build_tests/clang/debug/test_diag.exe

echo "   Compilando..."
clang++ -std=c++17 -O0 -g -x c++ - -o build/build_tests/clang/debug/test_diag.exe 2>&1 <<'EOF'
#include <iostream>
int main() {
    std::cout << "Hello from Clang!" << std::endl;
    return 0;
}
EOF

if [ -f build/build_tests/clang/debug/test_diag.exe ]; then
    echo "   [OK] Compilación exitosa"
    echo "   Ejecutando:"
    ./build/build_tests/clang/debug/test_diag.exe
    rm -f build/build_tests/clang/debug/test_diag.exe
else
    echo "   [FAIL] Compilación falló"
fi
echo ""

echo "7. Test de compilación con pipe a grep:"
echo "   Compilando con pipe..."
clang++ -std=c++17 -O0 -g -x c++ - -o build/build_tests/clang/debug/test_diag2.exe 2>&1 <<'EOF' | grep -v "^$" || true
#include <iostream>
int main() {
    std::cout << "Hello" << std::endl;
    return 0;
}
EOF

if [ -f build/build_tests/clang/debug/test_diag2.exe ]; then
    echo "   [OK] Compilación con pipe exitosa"
    rm -f build/build_tests/clang/debug/test_diag2.exe
else
    echo "   [FAIL] Compilación con pipe falló"
fi
echo ""

echo "========================================="
echo " Diagnóstico completado"
echo "========================================="
