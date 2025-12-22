#!/bin/bash

# Script auxiliar para configurar el entorno de Intel oneAPI
# Uso: source setup_intel_env.bash

CYAN='\033[0;36m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CACHE_FILE="$SCRIPT_DIR/.intel_env_cache.sh"
LOG_FILE="$SCRIPT_DIR/.intel_setup.log"

echo -e "${CYAN}[Intel Env] Iniciando configuración...${NC}"

# 1. Regeneración inteligente del caché
FORCE_REGEN=0
if [ ! -f "$CACHE_FILE" ]; then
    FORCE_REGEN=1
elif [ -f "$SCRIPT_DIR/setvarsall_intel.py" ]; then
    # Si el script python es más nuevo que el cache, regenerar
    if [ "$SCRIPT_DIR/setvarsall_intel.py" -nt "$CACHE_FILE" ]; then
        FORCE_REGEN=1
    fi
fi

if [ $FORCE_REGEN -eq 1 ]; then
    echo -e "${YELLOW}[Intel Env] Generando cache de variables (esto tarda unos segundos)...${NC}"
    # Ejecutamos el python. Es vital que el python esté en el PATH o usar ruta absoluta
    # Forzamos intel64 y vs2022 o vs2019 si es posible, el script python se encarga
    python3 "$SCRIPT_DIR/setvarsall_intel.py" > "$CACHE_FILE" 2> "$LOG_FILE"
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}[Error] Falló la ejecución de setvarsall_intel.py${NC}"
        cat "$LOG_FILE"
        return 1
    fi
fi

# 2. Cargar el entorno
if [ -f "$CACHE_FILE" ]; then
    source "$CACHE_FILE"
    echo -e "${GREEN}[Intel Env] Variables cargadas correctamente.${NC}"
else
    echo -e "${RED}[Error] No se encontró el archivo de cache: $CACHE_FILE${NC}"
    return 1
fi

# 3. Verificación de Salud (Health Check)
# Comprobamos si tenemos acceso a headers de windows (stdio.h via INCLUDE)
if [[ "$INCLUDE" != *"Microsoft Visual Studio"* && "$INCLUDE" != *"Windows Kits"* ]]; then
    echo -e "${RED}⚠️  ADVERTENCIA CRÍTICA: Entorno MSVC no detectado en INCLUDE${NC}"
    echo -e "${YELLOW}   El compilador Intel (icx) necesita las librerías de Visual Studio para compilar.${NC}"
    echo -e "${YELLOW}   El script intentó cargar 'setvars-vcvarsall.bat' pero parece haber fallado.${NC}"
    echo -e "${YELLOW}   Revise el log en: $LOG_FILE${NC}"
else
    echo -e "${GREEN}[Intel Env] Entorno MSVC detectado. Listo para compilar.${NC}"
fi

# 4. Alias útiles para MSYS2
# Intel usa icx.exe, pero a veces queremos invocarlo como cc o c++
alias icx="icx.exe"
alias icpx="icpx.exe"

# Imprimir versiones
icx --version | head -n 1