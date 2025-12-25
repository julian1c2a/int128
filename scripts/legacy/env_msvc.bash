#!/bin/bash

# ==============================================================================
# MSVC ENVIRONMENT SETUP FOR BASH
# ==============================================================================
# Ejecuta esto con "source env_msvc.bash" o ". env_msvc.bash"
# No lo ejecutes como "./env_msvc.bash" o las variables se perderán al salir.
# ==============================================================================

echo "🔎 Buscando instalación de Visual Studio..."

# 1. Detección de Rutas (Lógica v8)
POSSIBLE_ROOTS=("/c/Program Files/Microsoft Visual Studio" "/d/Program Files/Microsoft Visual Studio")
FOUND_MSVC_PATH=""
HIGHEST_VERSION="0.0.0"

for ROOT in "${POSSIBLE_ROOTS[@]}"; do
    if [ -d "$ROOT" ]; then
        while IFS= read -r TOOLS_DIR; do
            [ -z "$TOOLS_DIR" ] && continue
            while IFS= read -r VER_DIR; do
                if [ ! -z "$VER_DIR" ]; then
                    VER_NUM=$(basename "$VER_DIR")
                    if [ "$VER_NUM" != "0.0.0" ] && [ "$(printf '%s\n' "$HIGHEST_VERSION" "$VER_NUM" | sort -V | tail -n1)" == "$VER_NUM" ]; then
                        HIGHEST_VERSION=$VER_NUM
                        FOUND_MSVC_PATH=$VER_DIR
                    fi
                fi
            done < <(find "$TOOLS_DIR" -maxdepth 1 -type d -regextype posix-extended -regex ".*/[0-9]+\.[0-9]+\.[0-9]+" | sort -V)
        done < <(find "$ROOT" -maxdepth 9 -type d -name "MSVC" 2>/dev/null)
    fi
done

if [ -z "$FOUND_MSVC_PATH" ]; then
    echo "[FAIL] Error: No se encontró MSVC."
    return 1
fi

# Convertir ruta a formato Windows (C:/...)
MSVC_BASE=$(echo "$FOUND_MSVC_PATH" | sed -E 's|^/([a-zA-Z])|\1:/|')
echo "[OK] MSVC encontrado: $MSVC_BASE"

# 2. Detección de Windows Kits
KIT_ROOT="/c/Program Files (x86)/Windows Kits/10/Include"
NEW_KIT_VERSION=""
if [ -d "$KIT_ROOT" ]; then
    while IFS= read -r K_PATH; do
        NEW_KIT_VERSION=$(basename "$K_PATH")
    done < <(find "$KIT_ROOT" -maxdepth 1 -type d -regextype posix-extended -regex ".*/10\.0\.[0-9]+\.[0-9]+" | sort -V | tail -n 1)
fi
echo "[OK] Windows Kit: $NEW_KIT_VERSION"

# 3. Construcción de Variables de Entorno (Formato Windows con ;)
# Nota: Bash en MSYS2 convierte automáticamente ; a : en PATH, pero para LIB/INCLUDE
# que usan las herramientas de Windows, a veces es delicado.
# Exportaremos en formato Windows y dejaremos que las herramientas lo usen.

KIT_BASE="C:/Program Files (x86)/Windows Kits/10"

# INCLUDE
I1="${MSVC_BASE}/include"
I2="${KIT_BASE}/Include/${NEW_KIT_VERSION}/ucrt"
I3="${KIT_BASE}/Include/${NEW_KIT_VERSION}/shared"
I4="${KIT_BASE}/Include/${NEW_KIT_VERSION}/um"
I5="${KIT_BASE}/Include/${NEW_KIT_VERSION}/winrt"
export INCLUDE="${I1};${I2};${I3};${I4};${I5}"

# LIB
L1="${MSVC_BASE}/lib/x64"
L2="${KIT_BASE}/Lib/${NEW_KIT_VERSION}/ucrt/x64"
L3="${KIT_BASE}/Lib/${NEW_KIT_VERSION}/um/x64"
export LIB="${L1};${L2};${L3}"

# PATH
# Añadimos al PATH actual la ruta del compilador
BIN_DIR="${MSVC_BASE}/bin/Hostx64/x64"
# Convertimos BIN_DIR a formato Unix para el PATH de Bash
BIN_DIR_UNIX="/$(echo $BIN_DIR | sed 's|:| |' | sed 's| |/|' | sed 's|\\|/|g')"
# Corrección rápida de unidad (C:/ -> /c/)
BIN_DIR_UNIX=$(echo "$BIN_DIR" | sed -E 's|^([a-zA-Z]):|/\L\1|')

export PATH="$BIN_DIR_UNIX:$PATH"

echo "-----------------------------------------------------"
echo "🛠️ Entorno configurado. Ahora puedes ejecutar 'make'."
echo "   Compilador: cl.exe (Check: $(which cl 2>/dev/null))"
echo "-----------------------------------------------------"