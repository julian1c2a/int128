#!/bin/bash
# Script para configurar el entorno Clang
# Uso: source scripts/env_setup/setup_clang_env.bash

echo "-> Configurando entorno Clang..."

# Prioridad de búsqueda para entornos Clang en MSYS2
# 1. CLANG64 - Entorno Clang puro con libc++
# 2. MINGW64/UCRT64 - Clang con libstdc++ (si clang64 no existe)
CLANG_CANDIDATES=(
    "/c/msys64/clang64/bin"
    "/clang64/bin"
    "/mingw64/bin"
    "/ucrt64/bin"
)

CLANG_PATH=""
CLANG_DIR=""
USE_LIBCXX=0

# Buscar el mejor entorno Clang disponible
for candidate in "${CLANG_CANDIDATES[@]}"; do
    if [ -x "${candidate}/clang++.exe" ] || [ -x "${candidate}/clang++" ]; then
        CLANG_DIR="$candidate"
        CLANG_PATH="${candidate}/clang++"
        # Si encontramos clang64, usamos libc++
        if [[ "$candidate" == *"clang64"* ]]; then
            USE_LIBCXX=1
            echo "   Detectado: Entorno Clang puro (clang64) con libc++"
        else
            echo "   Detectado: Entorno Clang con libstdc++ (${candidate##*/})"
        fi
        break
    fi
done

# Fallback: buscar clang++ en PATH actual
if [ -z "$CLANG_PATH" ] && command -v clang++ &> /dev/null; then
    CLANG_PATH="$(which clang++)"
    CLANG_DIR="$(dirname "$CLANG_PATH")"
    echo "   Detectado: Clang en PATH existente"
fi

# Verificar que encontramos Clang
if [ -z "$CLANG_PATH" ]; then
    echo "[ERROR] clang++ no encontrado"
    echo "   Opciones:"
    echo "   1. Instalar entorno clang64: pacman -S mingw-w64-clang-x86_64-toolchain"
    echo "   2. Instalar Clang en otro entorno: pacman -S mingw-w64-x86_64-clang"
    return 1
fi

# Configurar PATH
if [[ ":$PATH:" != *":$CLANG_DIR:"* ]]; then
    export PATH="$CLANG_DIR:$PATH"
fi

# Obtener versión
CLANG_VERSION=$(clang++ --version | head -n 1)

# Configurar variables de compilación
export CC=clang
export CXX=clang++

# Si estamos en clang64, configurar para usar libc++
if [ $USE_LIBCXX -eq 1 ]; then
    export CXXFLAGS="-stdlib=libc++ $CXXFLAGS"
    export LDFLAGS="-stdlib=libc++ $LDFLAGS"
    
    # Configurar rutas para el entorno clang64
    CLANG64_ROOT="${CLANG_DIR%/bin}"
    export CPLUS_INCLUDE_PATH="${CLANG64_ROOT}/include/c++/v1:$CPLUS_INCLUDE_PATH"
    export LIBRARY_PATH="${CLANG64_ROOT}/lib:$LIBRARY_PATH"
    export LD_LIBRARY_PATH="${CLANG64_ROOT}/bin:$LD_LIBRARY_PATH"
fi

echo "[OK] Entorno Clang configurado correctamente"
echo "   Compilador: $CLANG_PATH"
echo "   Versión: $CLANG_VERSION"
if [ $USE_LIBCXX -eq 1 ]; then
    echo "   Biblioteca estándar: libc++ (LLVM)"
else
    echo "   Biblioteca estándar: libstdc++ (GCC)"
fi

return 0
