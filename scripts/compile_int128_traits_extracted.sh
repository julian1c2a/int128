#!/bin/bash
# Script de compilación para int128_traits tests y benchmarks
# Compila con 4 compiladores: GCC, Clang, Intel OneAPI, MSVC
# Estructura: build/build_tests/[compiler]/[mode] y build/build_benchmarks/[compiler]/[mode]
# Uso: ./compile_int128_traits_extracted.sh [gcc|clang|intel|msvc|all]
# Si no se especifica compilador, compila con todos

set -e

PROJECT_DIR="/c/msys64/ucrt64/home/julian/CppProjects/int128"
cd "$PROJECT_DIR"

# Definir rutas de compiladores
GCC_PATH="/c/msys64/ucrt64/bin/g++"
CLANG_PATH="/c/msys64/clang64/bin/clang++"
INTEL_PATH="/c/Program Files (x86)/Intel/oneAPI/compiler/latest/bin/icx"
MSVC_PATH="/c/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.42.34433/bin/Hostx64/x64/cl.exe"

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Determinar qué compiladores usar
COMPILER="${1:-all}"
COMPILER=$(echo "$COMPILER" | tr '[:upper:]' '[:lower:]')

echo -e "${BLUE}=============================================================${NC}"
echo -e "${BLUE}  Compilación de int128_traits Tests y Benchmarks${NC}"
if [ "$COMPILER" != "all" ]; then
    echo -e "${BLUE}  Compilador: ${COMPILER}${NC}"
fi
echo -e "${BLUE}=============================================================${NC}\n"

# Función para verificar si se debe compilar con un compilador específico
should_compile() {
    local comp=$1
    [ "$COMPILER" = "all" ] || [ "$COMPILER" = "$comp" ]
}

# Crear directorios
echo -e "${YELLOW}Creando estructura de directorios...${NC}"
mkdir -p build/build_tests/gcc/release
mkdir -p build/build_tests/clang/release
mkdir -p build/build_tests/intel/release
mkdir -p build/build_tests/msvc/release
mkdir -p build/build_benchmarks/gcc/release
mkdir -p build/build_benchmarks/clang/release
mkdir -p build/build_benchmarks/intel/release
mkdir -p build/build_benchmarks/msvc/release
mkdir -p benchmark_results
echo -e "${GREEN}✅ Directorios creados${NC}\n"

# Flags comunes
COMMON_FLAGS="-std=c++20 -Iinclude"
OPTIMIZE_FLAGS="-O3 -march=native"

SUCCESS_COUNT=0
TOTAL_COMPILATIONS=0

# Calcular total de compilaciones basado en los compiladores seleccionados
if should_compile "gcc"; then TOTAL_COMPILATIONS=$((TOTAL_COMPILATIONS + 2)); fi
if should_compile "clang"; then TOTAL_COMPILATIONS=$((TOTAL_COMPILATIONS + 2)); fi
if should_compile "intel"; then TOTAL_COMPILATIONS=$((TOTAL_COMPILATIONS + 2)); fi
if should_compile "msvc"; then TOTAL_COMPILATIONS=$((TOTAL_COMPILATIONS + 2)); fi

# =============================================================================
# COMPILACIÓN CON GCC (UCRT64)
# =============================================================================
if should_compile "gcc"; then
    echo -e "${BLUE}[GCC] Compilando con GCC (UCRT64)...${NC}"

    # Tests
    if "$GCC_PATH" $COMMON_FLAGS $OPTIMIZE_FLAGS \
        tests/int128_traits_extracted_tests.cpp \
        -o build/build_tests/gcc/release/int128_traits_tests_gcc.exe 2>&1 | tee /tmp/gcc_tests.log; then
        echo -e "${GREEN}  ✅ Tests compilados${NC}"
        SUCCESS_COUNT=$((SUCCESS_COUNT + 1))
    else
        echo -e "${RED}  ❌ Error en tests${NC}"
    fi

    # Benchmarks
    if "$GCC_PATH" $COMMON_FLAGS $OPTIMIZE_FLAGS \
        benchmarks/int128_traits_extracted_benchs.cpp \
    -o build/build_benchmarks/gcc/release/int128_traits_benchs_gcc.exe 2>&1 | tee /tmp/gcc_benchs.log; then
    echo -e "${GREEN}  ✅ Benchmarks compilados${NC}"
    SUCCESS_COUNT=$((SUCCESS_COUNT + 1))
else
    echo -e "${RED}  ❌ Error en benchmarks${NC}"
    fi
fi

# =============================================================================
# COMPILACIÓN CON CLANG (CLANG64)
# =============================================================================
if should_compile "clang"; then
    echo -e "\n${BLUE}[CLANG] Compilando con Clang (CLANG64)...${NC}"

    # Tests
    if "$CLANG_PATH" $COMMON_FLAGS $OPTIMIZE_FLAGS \
        tests/int128_traits_extracted_tests.cpp \
        -o build/build_tests/clang/release/int128_traits_tests_clang.exe 2>&1 | tee /tmp/clang_tests.log; then
        echo -e "${GREEN}  ✅ Tests compilados${NC}"
        SUCCESS_COUNT=$((SUCCESS_COUNT + 1))
    else
        echo -e "${RED}  ❌ Error en tests${NC}"
    fi

    # Benchmarks
    if "$CLANG_PATH" $COMMON_FLAGS $OPTIMIZE_FLAGS \
        benchmarks/int128_traits_extracted_benchs.cpp \
        -o build/build_benchmarks/clang/release/int128_traits_benchs_clang.exe 2>&1 | tee /tmp/clang_benchs.log; then
        echo -e "${GREEN}  ✅ Benchmarks compilados${NC}"
        SUCCESS_COUNT=$((SUCCESS_COUNT + 1))
    else
        echo -e "${RED}  ❌ Error en benchmarks${NC}"
    fi
fi

# =============================================================================
# COMPILACIÓN CON INTEL ONEAPI
# =============================================================================
# COMPILACIÓN CON INTEL ONEAPI
# =============================================================================
if should_compile "intel"; then
    echo -e "\n${BLUE}[INTEL] Compilando con Intel OneAPI...${NC}"

    # Configurar entorno Intel
    if [ -f "/c/Program Files (x86)/Intel/oneAPI/setvars.sh" ]; then
        source "/c/Program Files (x86)/Intel/oneAPI/setvars.sh" > /dev/null 2>&1
    fi

    INTEL_FLAGS="-std=c++20 -Iinclude -O3"

    # Tests
    if "$INTEL_PATH" $INTEL_FLAGS \
        tests/int128_traits_extracted_tests.cpp \
        -o build/build_tests/intel/release/int128_traits_tests_intel.exe 2>&1 | tee /tmp/intel_tests.log; then
        echo -e "${GREEN}  ✅ Tests compilados${NC}"
        SUCCESS_COUNT=$((SUCCESS_COUNT + 1))
    else
        echo -e "${RED}  ❌ Error en tests${NC}"
    fi

    # Benchmarks
    if "$INTEL_PATH" $INTEL_FLAGS \
        benchmarks/int128_traits_extracted_benchs.cpp \
        -o build/build_benchmarks/intel/release/int128_traits_benchs_intel.exe 2>&1 | tee /tmp/intel_benchs.log; then
        echo -e "${GREEN}  ✅ Benchmarks compilados${NC}"
        SUCCESS_COUNT=$((SUCCESS_COUNT + 1))
    else
        echo -e "${RED}  ❌ Error en benchmarks${NC}"
    fi
fi

# =============================================================================
# COMPILACIÓN CON MSVC
# =============================================================================
if should_compile "msvc"; then
    echo -e "\n${BLUE}[MSVC] Compilando con MSVC...${NC}"

    # Configurar entorno MSVC
    export PATH="/c/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.42.34433/bin/Hostx64/x64:$PATH"
    export INCLUDE="C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.42.34433\\include;C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.22621.0\\ucrt;C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.22621.0\\shared;C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.22621.0\\um"
    export LIB="C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.42.34433\\lib\\x64;C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.22621.0\\ucrt\\x64;C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.22621.0\\um\\x64"

    MSVC_FLAGS="/std:c++20 /EHsc /O2 /I include"

    # Tests
    if "$MSVC_PATH" $MSVC_FLAGS \
        tests/int128_traits_extracted_tests.cpp \
        /Fe:build/build_tests/msvc/release/int128_traits_tests_msvc.exe 2>&1 | tee /tmp/msvc_tests.log; then
        echo -e "${GREEN}  ✅ Tests compilados${NC}"
        SUCCESS_COUNT=$((SUCCESS_COUNT + 1))
    else
        echo -e "${RED}  ❌ Error en tests${NC}"
    fi

    # Benchmarks
    if "$MSVC_PATH" $MSVC_FLAGS \
        benchmarks/int128_traits_extracted_benchs.cpp \
        /Fe:build/build_benchmarks/msvc/release/int128_traits_benchs_msvc.exe 2>&1 | tee /tmp/msvc_benchs.log; then
        echo -e "${GREEN}  ✅ Benchmarks compilados${NC}"
        SUCCESS_COUNT=$((SUCCESS_COUNT + 1))
    else
        echo -e "${RED}  ❌ Error en benchmarks${NC}"
    fi
fi

# =============================================================================
# RESUMEN
# =============================================================================
echo -e "\n${BLUE}=============================================================${NC}"
echo -e "${BLUE}  Resumen de Compilación${NC}"
echo -e "${BLUE}=============================================================${NC}"
echo -e "Compilaciones exitosas: ${GREEN}$SUCCESS_COUNT${NC} de ${TOTAL_COMPILATIONS}"

if [ $SUCCESS_COUNT -eq $TOTAL_COMPILATIONS ]; then
    echo -e "\n${GREEN}✅ TODAS LAS COMPILACIONES EXITOSAS${NC}\n"
    exit 0
else
    echo -e "\n${YELLOW}⚠️  Algunas compilaciones fallaron${NC}\n"
    exit 1
fi
