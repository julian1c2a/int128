#!/bin/bash
# Script para configurar el entorno MSVC basado en .bashrc
# Uso: source scripts/setup_msvc_env.bash

# Versiones detectadas
export MSVC_VER="14.50.35717"
export KIT_VER="10.0.26100.0"

# Rutas Raíz (Formato Unix para Bash)
export VS_ROOT="/c/Program Files/Microsoft Visual Studio/18/Community"
export KIT_ROOT="/c/Program Files (x86)/Windows Kits/10"

# Añadimos el compilador MSVC al PATH
export MSVC_BIN="$VS_ROOT/VC/Tools/MSVC/$MSVC_VER/bin/Hostx64/x64"
export PATH="$MSVC_BIN:$PATH"

# Añadimos herramientas del Kit (rc.exe, mt.exe)
export KIT_BIN="$KIT_ROOT/bin/$KIT_VER/x64"
export PATH="$KIT_BIN:$PATH"

# Variables de entorno estilo Windows (con formato C:\...)
export VS_ROOT_WIN="C:\Program Files\Microsoft Visual Studio\18\Community"
export KIT_ROOT_WIN="C:\Program Files (x86)\Windows Kits\10"

# Variable INCLUDE
export INCLUDE="${VS_ROOT_WIN}\VC\Tools\MSVC\\${MSVC_VER}\include;\
${KIT_ROOT_WIN}\Include\\${KIT_VER}\ucrt;\
${KIT_ROOT_WIN}\Include\\${KIT_VER}\shared;\
${KIT_ROOT_WIN}\Include\\${KIT_VER}\um;\
${KIT_ROOT_WIN}\Include\\${KIT_VER}\winrt"

# Variable LIB
export LIB="${VS_ROOT_WIN}\VC\Tools\MSVC\\${MSVC_VER}\lib\x64;\
${KIT_ROOT_WIN}\Lib\\${KIT_VER}\ucrt\x64;\
${KIT_ROOT_WIN}\Lib\\${KIT_VER}\um\x64"

# Verificación
if command -v cl.exe &> /dev/null; then
    echo "✅ Entorno MSVC configurado correctamente"
    echo "   Compilador: $(which cl.exe)"
    echo "   Versión: $(cl.exe 2>&1 | head -n 1)"
else
    echo "❌ Error: cl.exe no encontrado después de configurar PATH"
    echo "   PATH: $PATH"
    return 1
fi
