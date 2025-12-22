#!/bin/bash
# Script para compilar benchmarks de uint128_traits
# Uso: ./build_uint128_traits_extracted_benchs.bash [gcc|clang|intel|msvc|all] [debug|release]

set -euo pipefail

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Directorios
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
PROJECT_DIR="${ROOT_DIR}"

# Validar argumentos
if [ $# -lt 1 ]; then
    echo -e "${RED}Error: Se requiere al menos un argumento${NC}"
    echo "Uso: $0 [gcc|clang|intel|msvc|all] [debug|release]"
    exit 1
fi

COMPILER="${1}"
COMPILER=$(echo "$COMPILER" | tr '[:upper:]' '[:lower:]')
MODE="${2:-release}"
MODE=$(echo "$MODE" | tr '[:upper:]' '[:lower:]')

# Validar modo
if [ "$MODE" != "debug" ] && [ "$MODE" != "release" ]; then
    echo -e "${RED}Error: Modo inválido '$MODE'. Use 'debug' o 'release'${NC}"
    exit 1
fi

# Validar compilador
case "$COMPILER" in
    gcc|clang|intel|msvc|all)
        ;;
    *)
        echo -e "${RED}Error: Compilador inválido '$COMPILER'${NC}"
        echo "Uso: $0 [gcc|clang|intel|msvc|all] [debug|release]"
        exit 1
        ;;
esac

TIMESTAMP=$(date '+%Y-%m-%d %H:%M:%S')
echo -e "${BLUE}============================================================${NC}"
echo -e "${BLUE}  Compilación de uint128_traits benchmarks${NC}"
echo -e "${BLUE}  Fecha: ${TIMESTAMP}${NC}"
echo -e "${BLUE}  Modo: ${MODE}${NC}"
echo -e "${BLUE}============================================================${NC}"

# Función auxiliar
should_compile() {
    local comp=$1
    [ "$COMPILER" = "all" ] || [ "$COMPILER" = "$comp" ]
}

# Crear directorios
mkdir -p build/build_benchmarks/{gcc,clang,intel,msvc}/{debug,release}
mkdir -p benchmark_results/{gcc,clang,intel,msvc}/{debug,release}

# Archivos
BENCH_SRC="${PROJECT_DIR}/benchmarks/uint128_traits_extracted_benchs.cpp"
INCLUDE_DIR="${PROJECT_DIR}/include"

# -----------------------------------------------------------------------------
# GCC
# -----------------------------------------------------------------------------
build_gcc() {
    echo -e "\n${YELLOW}[GCC] Compilando benchmarks...${NC}"
    
    if ! command -v g++ &> /dev/null; then
        echo -e "${RED}  ⚠️  g++ no encontrado en el PATH${NC}"
        return 1
    fi
    
    local FLAGS="-std=c++20 -Wall -Wextra -I${INCLUDE_DIR}"
    if [ "$MODE" = "debug" ]; then
        FLAGS="$FLAGS -g -O0"
    else
        FLAGS="$FLAGS -O3 -march=native"
    fi
    
    local OUTPUT="build/build_benchmarks/gcc/${MODE}/uint128_traits_benchs_gcc"
    
    g++ $FLAGS "${BENCH_SRC}" -o "${OUTPUT}"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}    ✅ Benchmarks compilados OK${NC}"
        ls -lh "${OUTPUT}" | awk '{print "    Ejecutable:", $9, "(" $5 ")"}'
        return 0
    else
        echo -e "${RED}    ❌ Error compilando benchmarks${NC}"
        return 1
    fi
}

# -----------------------------------------------------------------------------
# CLANG
# -----------------------------------------------------------------------------
build_clang() {
    echo -e "\n${YELLOW}[CLANG] Compilando benchmarks...${NC}"
    
    if ! command -v clang++ &> /dev/null; then
        echo -e "${RED}  ⚠️  clang++ no encontrado en el PATH${NC}"
        return 1
    fi
    
    local FLAGS="-std=c++20 -Wall -Wextra -I${INCLUDE_DIR}"
    if [ "$MODE" = "debug" ]; then
        FLAGS="$FLAGS -g -O0"
    else
        FLAGS="$FLAGS -O3 -march=native"
    fi
    
    local OUTPUT="build/build_benchmarks/clang/${MODE}/uint128_traits_benchs_clang"
    
    clang++ $FLAGS "${BENCH_SRC}" -o "${OUTPUT}"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}    ✅ Benchmarks compilados OK${NC}"
        ls -lh "${OUTPUT}" | awk '{print "    Ejecutable:", $9, "(" $5 ")"}'
        return 0
    else
        echo -e "${RED}    ❌ Error compilando benchmarks${NC}"
        return 1
    fi
}

# -----------------------------------------------------------------------------
# INTEL ONEAPI (ICX)
# -----------------------------------------------------------------------------
build_intel() {
    echo -e "\n${YELLOW}[INTEL] Compilando benchmarks con Intel OneAPI...${NC}"
    
    if ! command -v icx &> /dev/null; then
        echo -e "${RED}  ⚠️  Intel 'icx' no encontrado en el PATH${NC}"
        echo -e "      Ejecuta primero: source scripts/setup_intel_combined.bash"
        return 1
    fi
    
    # Flags estilo Windows para ICX
    local FLAGS="/std:c++20 /W3 /EHsc /Zc:__cplusplus -D_HAS_CXX20=1 -Xclang -fcxx-exceptions"
    if [ "$MODE" = "debug" ]; then
        FLAGS="$FLAGS /Od /Zi"
    else
        FLAGS="$FLAGS /O3 /QxHost"
    fi
    
    local OUTPUT="build/build_benchmarks/intel/${MODE}/uint128_traits_benchs_intel.exe"
    
    # IMPORTANTE: export MSYS2_ARG_CONV_EXCL="*" evita que /O3 se convierta en C:\msys64\O3
    export MSYS2_ARG_CONV_EXCL="*"
    
    icx $FLAGS \
        -I"${INCLUDE_DIR}" \
        benchmarks/uint128_traits_extracted_benchs.cpp \
        -o build/build_benchmarks/intel/${MODE}/uint128_traits_benchs_intel.exe
    
    local result=$?
    unset MSYS2_ARG_CONV_EXCL
    
    if [ $result -eq 0 ]; then
        echo -e "${GREEN}    ✅ Benchmarks compilados OK${NC}"
        ls -lh --time-style='+%Y-%m-%d %H:%M:%S' "${OUTPUT}" | awk -v path="${OUTPUT}" '{print "    Ejecutable:", $6, $7, "-", path, "(" $5 ")"}'
        return 0
    else
        echo -e "${RED}    ❌ Error compilando benchmarks${NC}"
        return 1
    fi
}

# -----------------------------------------------------------------------------
# MSVC
# -----------------------------------------------------------------------------
build_msvc() {
    echo -e "\n${YELLOW}[MSVC] Compilando benchmarks...${NC}"
    
    if ! command -v cl.exe &> /dev/null; then
        echo -e "${RED}  ⚠️  cl.exe no encontrado en el PATH${NC}"
        echo -e "      Ejecuta vcvarsall.bat o usa vcvarsall.py"
        return 1
    fi
    
    local FLAGS="/std:c++20 /W4 /EHsc"
    if [ "$MODE" = "debug" ]; then
        FLAGS="$FLAGS /Od /Zi"
    else
        FLAGS="$FLAGS /O2 /GL /arch:AVX2"
    fi
    
    local OUTPUT="build/build_benchmarks/msvc/${MODE}/uint128_traits_benchs_msvc.exe"
    
    cl.exe $FLAGS \
        /I"${INCLUDE_DIR}" \
        "${BENCH_SRC}" \
        /Fe"${OUTPUT}" \
        > /dev/null 2>&1
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}    ✅ Benchmarks compilados OK${NC}"
        # Limpiar archivos temporales
        rm -f build/build_benchmarks/msvc/${MODE}/*.obj build/build_benchmarks/msvc/${MODE}/*.pdb
        ls -lh --time-style='+%Y-%m-%d %H:%M:%S' "${OUTPUT}" | awk -v path="${OUTPUT}" '{print "    Ejecutable:", $6, $7, "-", path, "(" $5 ")"}'
        return 0
    else
        echo -e "${RED}    ❌ Error compilando benchmarks${NC}"
        return 1
    fi
}

# -----------------------------------------------------------------------------
# MAIN
# -----------------------------------------------------------------------------
main() {
    local success=0
    local failed=0
    
    if should_compile "gcc"; then
        if build_gcc; then
            ((success++))
        else
            ((failed++))
        fi
    fi
    
    if should_compile "clang"; then
        if build_clang; then
            ((success++))
        else
            ((failed++))
        fi
    fi
    
    if should_compile "intel"; then
        if build_intel; then
            ((success++))
        else
            ((failed++))
        fi
    fi
    
    if should_compile "msvc"; then
        if build_msvc; then
            ((success++))
        else
            ((failed++))
        fi
    fi
    
    echo -e "\n${BLUE}============================================================${NC}"
    echo -e "${BLUE}  Resumen de compilación${NC}"
    echo -e "${BLUE}============================================================${NC}"
    echo -e "${GREEN}  Exitosos: ${success}${NC}"
    echo -e "${RED}  Fallidos: ${failed}${NC}"
    echo -e "${BLUE}============================================================${NC}"
    
    return ${failed}
}

main

