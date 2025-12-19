#!/bin/bash

# Script auxiliar para configurar el entorno de Intel oneAPI
# Este script debe ser "sourced" no ejecutado directamente
# Uso: source scripts/setup_intel_env.bash

# Colores
CYAN='\033[0;36m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

# Ruta de instalación de Intel oneAPI
INTEL_ONEAPI_ROOT="/c/Program Files (x86)/Intel/oneAPI"

# Verificar que se esté usando source (no ejecutando directamente)
if [ "${BASH_SOURCE[0]}" -ef "$0" ]; then
    echo -e "${RED}✗ Error: Este script debe ejecutarse con 'source'${NC}"
    echo -e "${YELLOW}   Uso correcto: source scripts/setup_intel_env.bash${NC}"
    exit 1
fi

# Si estamos en Windows con MSYS2/Git Bash
if [ -d "$INTEL_ONEAPI_ROOT" ]; then
    echo -e "${CYAN}Configurando Intel oneAPI...${NC}"
    
    # Añadir binarios de Intel al PATH (al principio para tener prioridad)
    export PATH="$INTEL_ONEAPI_ROOT/compiler/latest/bin:$PATH"
    export PATH="$INTEL_ONEAPI_ROOT/compiler/latest/bin/intel64:$PATH"
    
    # Variables necesarias para el compilador
    export INTEL_ONEAPI_ROOT
    
    # Verificar que icpx o icx estén disponibles (sin ejecutar --version)
    local ICPX_PATH=$(command -v icpx 2>/dev/null)
    local ICX_PATH=$(command -v icx 2>/dev/null)
    
    if [ -n "$ICPX_PATH" ]; then
        echo -e "${GREEN}✓ Intel C++ compiler (icpx) encontrado en PATH${NC}"
        echo -e "   Ubicación: $ICPX_PATH"
    elif [ -n "$ICX_PATH" ]; then
        echo -e "${GREEN}✓ Intel C++ compiler (icx) encontrado en PATH${NC}"
        echo -e "   Ubicación: $ICX_PATH"
    else
        echo -e "${YELLOW}⚠ Compilador Intel añadido al PATH, pero no se pudo verificar${NC}"
        echo -e "${YELLOW}   Ejecute manualmente desde CMD: \"$INTEL_ONEAPI_ROOT\\setvars.bat\"${NC}"
    fi
    
    echo -e "${CYAN}PATH actualizado con binarios de Intel oneAPI${NC}"
else
    echo -e "${RED}✗ Intel oneAPI no encontrado en: $INTEL_ONEAPI_ROOT${NC}"
    echo -e "${YELLOW}   Verifique que Intel oneAPI esté instalado correctamente${NC}"
fi
