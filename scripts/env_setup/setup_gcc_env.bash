#!/bin/bash
# Script para configurar el entorno GCC
# Uso: source scripts/env_setup/setup_gcc_env.bash

echo "-> Configurando entorno GCC..."

# Detectar GCC en el sistema
if command -v g++ &> /dev/null; then
    export GCC_PATH="$(which g++)"
    export GCC_DIR="$(dirname "$GCC_PATH")"
    
    # Añadir al PATH si no está ya
    if [[ ":$PATH:" != *":$GCC_DIR:"* ]]; then
        export PATH="$GCC_DIR:$PATH"
    fi
    
    # Obtener versión
    GCC_VERSION=$(g++ --version | head -n 1)
    
    echo "[OK] Entorno GCC configurado correctamente"
    echo "   Compilador: $GCC_PATH"
    echo "   Versión: $GCC_VERSION"
    
    # Exportar para Python
    export CC=gcc
    export CXX=g++
    
    return 0
else
    echo "[ERROR] g++ no encontrado en el PATH"
    echo "   Instala GCC o añádelo al PATH"
    return 1
fi
