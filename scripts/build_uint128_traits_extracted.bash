#!/bin/bash
# Script para compilar tests y benchmarks de uint128_traits
# Uso: ./build_uint128_traits_extracted.bash [gcc|clang|intel|msvc|all]

set -u

PROJECT_DIR="/c/msys64/ucrt64/home/julian/CppProjects/int128"
cd "$PROJECT_DIR" || { echo "Error: No encuentro el directorio del proyecto"; exit 1; }

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

COMPILER="${1:-all}"
COMPILER=$(echo "$COMPILER" | tr '[:upper:]' '[:lower:]')

echo -e "${BLUE}============================================================${NC}"
echo -e "${BLUE}  Compilación de uint128_traits (Intel OneAPI)${NC}"
echo -e "${BLUE}============================================================${NC}"

should_compile() {
    local comp=$1
    [ "$COMPILER" = "all" ] || [ "$COMPILER" = "$comp" ]
}

mkdir -p build/build_tests/{gcc,clang,intel,msvc}/release
mkdir -p build/build_benchmarks/{gcc,clang,intel,msvc}/release

# -----------------------------------------------------------------------------
# INTEL ONEAPI (ICX) - MODO WINDOWS
# -----------------------------------------------------------------------------
if should_compile "intel"; then
    echo -e "\n${YELLOW}[INTEL] Compilando con Intel OneAPI...${NC}"

    if command -v icx &> /dev/null; then
        ICX_CMD="icx"
        
        # FLAGS CORREGIDOS
        # Usamos flags estilo Windows (/).
        # MSYS2 intentará convertirlos a rutas (C:/...) y romperá la compilación.
        # SOLUCIÓN: Usaremos MSYS2_ARG_CONV_EXCL="*" al ejecutar.
        
        # Flags:
        # /std:c++20      : Estándar
        # /O3             : Máxima velocidad
        # /QxHost         : Optimización CPU local
        # /W3             : Warnings
        # /EHsc           : Excepciones Windows
        # /Zc:__cplusplus : Macro de versión correcta
        # -D_HAS_CXX20=1  : Forzar STL
        # -Xclang ...     : Forzar excepciones en frontend
        
        ICX_FLAGS="/std:c++20 /O3 /QxHost /W3 /EHsc /Zc:__cplusplus -D_HAS_CXX20=1 -Xclang -fcxx-exceptions"
        
        echo -e "${GREEN}  - Compilando tests...${NC}"
        
        # IMPORTANTE: export MSYS2_ARG_CONV_EXCL="*" evita que /O3 se convierta en C:\msys64\O3
        export MSYS2_ARG_CONV_EXCL="*"
        
        "$ICX_CMD" $ICX_FLAGS \
            -I"$PROJECT_DIR/include" \
            tests/uint128_traits_extracted_tests.cpp \
            -o build/build_tests/intel/release/uint128_traits_tests_intel.exe
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}    ✅ Tests compilados OK${NC}"
            ls -l --time-style=+%H:%M:%S build/build_tests/intel/release/uint128_traits_tests_intel.exe | awk '{print "    Nuevo ejecutable:", $6, $7}'
        else
            echo -e "${RED}    ❌ Error compilando tests${NC}"
            exit 1
        fi

        echo -e "${GREEN}  - Compilando benchmarks...${NC}"
        "$ICX_CMD" $ICX_FLAGS \
            -I"$PROJECT_DIR/include" \
            benchmarks/uint128_traits_extracted_benchs.cpp \
            -o build/build_benchmarks/intel/release/uint128_traits_benchs_intel.exe
            
        if [ $? -eq 0 ]; then
             echo -e "${GREEN}    ✅ Benchmarks compilados OK${NC}"
             ls -l --time-style=+%H:%M:%S build/build_benchmarks/intel/release/uint128_traits_benchs_intel.exe | awk '{print "    Nuevo ejecutable:", $6, $7}'
        fi
        
        # Limpiamos la variable por si acaso afecta a otros comandos (aunque el script acaba aquí)
        unset MSYS2_ARG_CONV_EXCL
        
    else
        echo -e "${RED}  ⚠️  Intel 'icx' no encontrado en el PATH.${NC}"
        echo -e "      Ejecuta primero: source scripts/setup_intel_combined.bash"
    fi
fi

# (Resto de compiladores omitidos para brevedad)

echo -e "\n${BLUE}============================================================${NC}"
echo -e "${BLUE}  Fin del proceso${NC}"
echo -e "${BLUE}============================================================${NC}"