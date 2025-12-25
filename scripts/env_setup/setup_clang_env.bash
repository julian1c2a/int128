#!/bin/bash
# Script para configurar el entorno Clang
# Uso: source scripts/env_setup/setup_clang_env.bash

echo "-> Configurando entorno Clang..."

# Detectar Clang en el sistema
if command -v clang++ &> /dev/null; then
    export CLANG_PATH="$(which clang++)"
    export CLANG_DIR="$(dirname "$CLANG_PATH")"
    
    # Añadir al PATH si no está ya
    if [[ ":$PATH:" != *":$CLANG_DIR:"* ]]; then
        export PATH="$CLANG_DIR:$PATH"
    fi
    
    # Obtener versión
    CLANG_VERSION=$(clang++ --version | head -n 1)
    
    echo "[OK] Entorno Clang configurado correctamente"
    echo "   Compilador: $CLANG_PATH"
    echo "   Versión: $CLANG_VERSION"
    
    # Exportar para Python
    export CC=clang
    export CXX=clang++
    
    return 0
else
    echo "[ERROR] clang++ no encontrado en el PATH"
    echo "   Instala Clang o añádelo al PATH"
    return 1
fi
