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

# Verificar que se esté usando source (no ejecutando directamente)
if [ "${BASH_SOURCE[0]}" -ef "$0" ]; then
    echo -e "${RED}✗ Error: Este script debe ejecutarse con 'source'${NC}"
    echo -e "${YELLOW}   Uso correcto: source scripts/setup_intel_env.bash${NC}"
    exit 1
fi

echo -e "${CYAN}Configurando Intel oneAPI...${NC}"

# Obtener el directorio del script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CACHE_FILE="$SCRIPT_DIR/.intel_env_cache.sh"

# Función para generar el cache
generate_cache() {
    echo -e "${CYAN}Generando cache de variables (primera vez, puede tardar)...${NC}"
    
    if [ -f "$SCRIPT_DIR/setvarsall_intel.py" ]; then
        # Ejecutar el script Python y guardar SOLO los exports en el cache
        # Los mensajes de diagnóstico van a stderr, los exports a stdout
        python3 "$SCRIPT_DIR/setvarsall_intel.py" > "$CACHE_FILE" 2>> "$SCRIPT_DIR/.intel_setup.log"
        
        if [ $? -eq 0 ] && [ -s "$CACHE_FILE" ]; then
            # Verificar que el cache contiene exports válidos
            if grep -q "^export" "$CACHE_FILE"; then
                echo -e "${GREEN}✓ Cache generado exitosamente${NC}"
                return 0
            else
                rm -f "$CACHE_FILE"
                echo -e "${RED}✗ Cache inválido (no contiene exports)${NC}"
                return 1
            fi
        else
            rm -f "$CACHE_FILE"
            echo -e "${RED}✗ Error al generar cache${NC}"
            [ -f "$SCRIPT_DIR/.intel_setup.log" ] && echo -e "${YELLOW}   Ver detalles en: $SCRIPT_DIR/.intel_setup.log${NC}"
            return 1
        fi
    else
        echo -e "${RED}✗ No se encontró setvarsall_intel.py${NC}"
        return 1
    fi
}

# Verificar si existe el cache y es reciente (menos de 1 día)
if [ -f "$CACHE_FILE" ]; then
    # Verificar edad del cache (en segundos)
    if [ $(uname -s) == "Linux" ] || [[ $(uname -s) == MINGW* ]] || [[ $(uname -s) == MSYS* ]]; then
        CACHE_AGE=$(($(date +%s) - $(stat -c %Y "$CACHE_FILE" 2>/dev/null || stat -f %m "$CACHE_FILE" 2>/dev/null || echo 0)))
        
        # Si el cache tiene más de 24 horas, regenerarlo
        if [ $CACHE_AGE -gt 86400 ]; then
            echo -e "${YELLOW}Cache antiguo, regenerando...${NC}"
            generate_cache
        fi
    fi
else
    # No existe cache, generarlo
    generate_cache
fi

# Cargar variables desde el cache
if [ -f "$CACHE_FILE" ] && [ -s "$CACHE_FILE" ]; then
    source "$CACHE_FILE"
    
    # Verificar que las variables críticas estén configuradas
    ICPX_PATH=$(command -v icpx 2>/dev/null)
    ICX_PATH=$(command -v icx 2>/dev/null)
    
    if [ -n "$ICPX_PATH" ]; then
        echo -e "${GREEN}✓ Intel C++ compiler (icpx) encontrado en PATH${NC}"
        echo -e "   Ubicación: $ICPX_PATH"
    elif [ -n "$ICX_PATH" ]; then
        echo -e "${GREEN}✓ Intel C++ compiler (icx) encontrado en PATH${NC}"
        echo -e "   Ubicación: $ICX_PATH"
    else
        echo -e "${YELLOW}⚠ Advertencia: Compilador Intel no encontrado${NC}"
    fi
    
    # Verificar variables de linker
    if [ -n "$INCLUDE" ] && [ -n "$LIB" ]; then
        echo -e "${GREEN}✓ Variables INCLUDE y LIB configuradas${NC}"
        
        # Verificar si las rutas incluyen headers estándar de C++ (Visual Studio)
        if ! echo "$INCLUDE" | grep -qi "microsoft.*vc\\|windows kits"; then
            echo -e "${RED}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
            echo -e "${RED}⚠  INTEL ONEAPI REQUIERE VISUAL STUDIO EN WINDOWS${NC}"
            echo -e "${RED}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
            echo -e "${YELLOW}Intel oneAPI para Windows está diseñado para trabajar con:${NC}"
            echo -e "${YELLOW}  • Headers estándar de C++ de Visual Studio${NC}"
            echo -e "${YELLOW}  • MSVC Runtime (vcruntime)${NC}"
            echo -e "${YELLOW}  • Windows Kits (UCRT)${NC}"
            echo -e ""
            echo -e "${YELLOW}Los headers de MSYS2/MinGW son incompatibles (usan MinGW-w64 runtime).${NC}"
            echo -e ""
            echo -e "${CYAN}Soluciones recomendadas:${NC}"
            echo -e "  1. ${GREEN}Instalar Visual Studio 2022 Build Tools${NC}"
            echo -e "     https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022"
            echo -e ""
            echo -e "  2. ${GREEN}Ejecutar desde 'x64 Native Tools Command Prompt for VS'${NC}"
            echo -e "     y luego: ${CYAN}call \"C:\\Program Files (x86)\\Intel\\oneAPI\\setvars.bat\"${NC}"
            echo -e ""
            echo -e "  3. ${GREEN}Usar GCC o Clang de MSYS2 (recomendado)${NC}"
            echo -e "     ${CYAN}./scripts/compile_int128_traits_extracted.sh gcc${NC}"
            echo -e "     ${CYAN}./scripts/compile_int128_traits_extracted.sh clang${NC}"
            echo -e "${RED}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
        fi
    else
        echo -e "${YELLOW}⚠ Advertencia: INCLUDE o LIB no configuradas${NC}"
        echo -e "${YELLOW}   Regenere el cache eliminando: $CACHE_FILE${NC}"
    fi
    
    echo -e "${CYAN}Entorno Intel oneAPI configurado completamente${NC}"
else
    echo -e "${RED}✗ Error: No se pudo configurar el entorno Intel${NC}"
    echo -e "${YELLOW}   Regenere manualmente ejecutando:${NC}"
    echo -e "${YELLOW}   python3 $SCRIPT_DIR/setvarsall_intel.py > $CACHE_FILE${NC}"
    return 1
fi
