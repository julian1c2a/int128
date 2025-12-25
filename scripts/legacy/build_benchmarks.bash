#!/usr/bin/env bash
#
# build_benchmarks.bash
# Script para compilar los benchmarks con GCC, Clang, MSVC e Intel
#
# Uso:
#   ./scripts/build_benchmarks.bash [gcc|clang|msvc|intel|all]
#

set -euo pipefail

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Directorios
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${ROOT_DIR}/build"
BENCHMARK_DIR="${ROOT_DIR}/benchmarks"
BENCHMARK_RESULTS_DIR="${ROOT_DIR}/benchmark_results"

# Crear directorio de resultados si no existe
mkdir -p "${BENCHMARK_RESULTS_DIR}"

# Archivos - uint128_t
BENCHMARK_SRC_UINT128="${BENCHMARK_DIR}/uint128_extracted_benchmarks.cpp"
BENCHMARK_EXEC_GCC_UINT128="${BUILD_DIR}/uint128_benchmarks_gcc"
BENCHMARK_EXEC_CLANG_UINT128="${BUILD_DIR}/uint128_benchmarks_clang"
BENCHMARK_EXEC_MSVC_UINT128="${BUILD_DIR}/uint128_benchmarks_msvc.exe"
BENCHMARK_EXEC_INTEL_UINT128="${BUILD_DIR}/uint128_benchmarks_intel"

# Archivos - int128_t
BENCHMARK_SRC_INT128="${BENCHMARK_DIR}/int128_extracted_benchmarks.cpp"
BENCHMARK_EXEC_GCC_INT128="${BUILD_DIR}/int128_benchmarks_gcc"
BENCHMARK_EXEC_CLANG_INT128="${BUILD_DIR}/int128_benchmarks_clang"
BENCHMARK_EXEC_MSVC_INT128="${BUILD_DIR}/int128_benchmarks_msvc.exe"
BENCHMARK_EXEC_INTEL_INT128="${BUILD_DIR}/int128_benchmarks_intel"

# Flags comunes
INCLUDE_FLAGS="-I${ROOT_DIR}/include"
STD_FLAG="-std=c++20"
OPTIMIZE_FLAG="-O3 -march=native"
WARNING_FLAGS="-Wall -Wextra"

# Boost flags (opcional)
BOOST_FLAGS=""
# Descomentar si Boost está disponible:
# BOOST_FLAGS="-DHAVE_BOOST -I/path/to/boost/include"

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}Building int128/uint128 Benchmarks${NC}"
echo -e "${BLUE}========================================${NC}"

# Función para compilar con GCC
build_gcc() {
    echo -e "\n${YELLOW}Building with GCC...${NC}"
    
    if ! command -v g++ &> /dev/null; then
        echo -e "${RED}Error: g++ not found${NC}"
        return 1
    fi
    
    local success=0
    local failed=0
    
    # Build uint128_t benchmarks
    echo -e "${BLUE}  Building uint128_t benchmarks...${NC}"
    g++ ${STD_FLAG} ${OPTIMIZE_FLAG} ${WARNING_FLAGS} ${INCLUDE_FLAGS} ${BOOST_FLAGS} \
        "${BENCHMARK_SRC_UINT128}" \
        -o "${BENCHMARK_EXEC_GCC_UINT128}"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}  [OK] uint128_t benchmark successful${NC}"
        ((success++))
    else
        echo -e "${RED}  [ERROR] uint128_t benchmark failed${NC}"
        ((failed++))
    fi
    
    # Build int128_t benchmarks
    echo -e "${BLUE}  Building int128_t benchmarks...${NC}"
    g++ ${STD_FLAG} ${OPTIMIZE_FLAG} ${WARNING_FLAGS} ${INCLUDE_FLAGS} ${BOOST_FLAGS} \
        "${BENCHMARK_SRC_INT128}" \
        -o "${BENCHMARK_EXEC_GCC_INT128}"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}  [OK] int128_t benchmark successful${NC}"
        ((success++))
    else
        echo -e "${RED}  [ERROR] int128_t benchmark failed${NC}"
        ((failed++))
    fi
    
    echo -e "${GREEN}[OK] GCC build complete: ${success} successful, ${failed} failed${NC}"
    return ${failed}
}

# Función para compilar con Clang
build_clang() {
    echo -e "\n${YELLOW}Building with Clang...${NC}"
    
    if ! command -v clang++ &> /dev/null; then
        echo -e "${RED}Error: clang++ not found${NC}"
        return 1
    fi
    
    local success=0
    local failed=0
    
    # Build uint128_t benchmarks
    echo -e "${BLUE}  Building uint128_t benchmarks...${NC}"
    clang++ ${STD_FLAG} ${OPTIMIZE_FLAG} ${WARNING_FLAGS} ${INCLUDE_FLAGS} ${BOOST_FLAGS} \
        "${BENCHMARK_SRC_UINT128}" \
        -o "${BENCHMARK_EXEC_CLANG_UINT128}"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}  [OK] uint128_t benchmark successful${NC}"
        ((success++))
    else
        echo -e "${RED}  [ERROR] uint128_t benchmark failed${NC}"
        ((failed++))
    fi
    
    # Build int128_t benchmarks
    echo -e "${BLUE}  Building int128_t benchmarks...${NC}"
    clang++ ${STD_FLAG} ${OPTIMIZE_FLAG} ${WARNING_FLAGS} ${INCLUDE_FLAGS} ${BOOST_FLAGS} \
        "${BENCHMARK_SRC_INT128}" \
        -o "${BENCHMARK_EXEC_CLANG_INT128}"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}  [OK] int128_t benchmark successful${NC}"
        ((success++))
    else
        echo -e "${RED}  [ERROR] int128_t benchmark failed${NC}"
        ((failed++))
    fi
    
    echo -e "${GREEN}[OK] Clang build complete: ${success} successful, ${failed} failed${NC}"
    return ${failed}
}

# Función para compilar con MSVC
build_msvc() {
    echo -e "\n${YELLOW}Building with MSVC...${NC}"
    
    # Intentar activar MSVC usando vcvarsall.py
    if [ -f "${ROOT_DIR}/vcvarsall.py" ]; then
        echo -e "${BLUE}Activating MSVC environment...${NC}"
        
        # Ejecutar vcvarsall.py y guardar el script generado
        VCVARS_SCRIPT="${BUILD_DIR}/vcvars_temp.bash"
        python "${ROOT_DIR}/vcvarsall.py" > "${VCVARS_SCRIPT}"
        
        if [ $? -eq 0 ] && [ -f "${VCVARS_SCRIPT}" ]; then
            echo -e "${GREEN}[OK] MSVC environment activated${NC}"
            source "${VCVARS_SCRIPT}"
            rm -f "${VCVARS_SCRIPT}"
        else
            echo -e "${RED}Error: Could not activate MSVC environment${NC}"
            return 1
        fi
    fi
    
    if ! command -v cl.exe &> /dev/null; then
        echo -e "${RED}Error: cl.exe not found${NC}"
        echo -e "${YELLOW}Hint: Run vcvarsall.bat or use vcvarsall.py${NC}"
        return 1
    fi
    
    # MSVC flags
    MSVC_STD="/std:c++20"
    MSVC_OPT="/O2 /GL /arch:AVX2"
    MSVC_WARN="/W4"
    MSVC_INCLUDE="/I${ROOT_DIR}/include"
    MSVC_OUT="/Fe${BENCHMARK_EXEC_MSVC}"
    MSVC_EHsc="/EHsc"
    MSVC_BOOST=""
    # Descomentar si Boost está disponible:
    # MSVC_BOOST="/DHAVE_BOOST /I/path/to/boost/include"
    
    local success=0
    local failed=0
    
    # Build uint128_t benchmarks
    echo -e "${BLUE}  Building uint128_t benchmarks...${NC}"
    MSVC_OUT_UINT128="/Fe${BENCHMARK_EXEC_MSVC_UINT128}"
    cl.exe ${MSVC_STD} ${MSVC_OPT} ${MSVC_WARN} ${MSVC_EHsc} ${MSVC_INCLUDE} ${MSVC_BOOST} \
        "${BENCHMARK_SRC_UINT128}" \
        ${MSVC_OUT_UINT128} \
        > /dev/null 2>&1
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}  [OK] uint128_t benchmark successful${NC}"
        ((success++))
    else
        echo -e "${RED}  [ERROR] uint128_t benchmark failed${NC}"
        ((failed++))
    fi
    
    # Build int128_t benchmarks
    echo -e "${BLUE}  Building int128_t benchmarks...${NC}"
    MSVC_OUT_INT128="/Fe${BENCHMARK_EXEC_MSVC_INT128}"
    cl.exe ${MSVC_STD} ${MSVC_OPT} ${MSVC_WARN} ${MSVC_EHsc} ${MSVC_INCLUDE} ${MSVC_BOOST} \
        "${BENCHMARK_SRC_INT128}" \
        ${MSVC_OUT_INT128} \
        > /dev/null 2>&1
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}  [OK] int128_t benchmark successful${NC}"
        ((success++))
    else
        echo -e "${RED}  [ERROR] int128_t benchmark failed${NC}"
        ((failed++))
    fi
    
    # Limpiar archivos temporales de MSVC
    rm -f "${BUILD_DIR}"/*.obj "${BUILD_DIR}"/*.pdb
    
    echo -e "${GREEN}[OK] MSVC build complete: ${success} successful, ${failed} failed${NC}"
    return ${failed}
}

# Función para compilar con Intel C++ (icpx o icx)
build_intel() {
    echo -e "\n${YELLOW}Building with Intel C++...${NC}"
    
    # Intentar icpx primero (oneAPI), luego icx (legacy)
    local intel_compiler=""
    if command -v icpx &> /dev/null; then
        intel_compiler="icpx"
    elif command -v icx &> /dev/null; then
        intel_compiler="icx"
    else
        echo -e "${RED}Error: Intel C++ compiler (icpx/icx) not found${NC}"
        echo -e "${YELLOW}Hint: Source Intel oneAPI environment (setvars.sh)${NC}"
        return 1
    fi
    
    echo -e "${BLUE}Using Intel compiler: ${intel_compiler}${NC}"
    
    local success=0
    local failed=0
    
    # Build uint128_t benchmarks
    echo -e "${BLUE}  Building uint128_t benchmarks...${NC}"
    ${intel_compiler} ${STD_FLAG} ${OPTIMIZE_FLAG} ${WARNING_FLAGS} ${INCLUDE_FLAGS} ${BOOST_FLAGS} \
        "${BENCHMARK_SRC_UINT128}" \
        -o "${BENCHMARK_EXEC_INTEL_UINT128}"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}  [OK] uint128_t benchmark successful${NC}"
        ((success++))
    else
        echo -e "${RED}  [ERROR] uint128_t benchmark failed${NC}"
        ((failed++))
    fi
    
    # Build int128_t benchmarks
    echo -e "${BLUE}  Building int128_t benchmarks...${NC}"
    ${intel_compiler} ${STD_FLAG} ${OPTIMIZE_FLAG} ${WARNING_FLAGS} ${INCLUDE_FLAGS} ${BOOST_FLAGS} \
        "${BENCHMARK_SRC_INT128}" \
        -o "${BENCHMARK_EXEC_INTEL_INT128}"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}  [OK] int128_t benchmark successful${NC}"
        ((success++))
    else
        echo -e "${RED}  [ERROR] int128_t benchmark failed${NC}"
        ((failed++))
    fi
    
    echo -e "${GREEN}[OK] Intel build complete: ${success} successful, ${failed} failed${NC}"
    return ${failed}
}

# Función para compilar todos
build_all() {
    local success=0
    local failed=0
    
    if build_gcc; then
        ((success++))
    else
        ((failed++))
    fi
    
    if build_clang; then
        ((success++))
    else
        ((failed++))
    fi
    
    if build_msvc; then
        ((success++))
    else
        ((failed++))
    fi
    
    if build_intel; then
        ((success++))
    else
        ((failed++))
    fi
    
    echo -e "\n${BLUE}========================================${NC}"
    echo -e "${BLUE}Build Summary${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo -e "${GREEN}Successful: ${success}${NC}"
    echo -e "${RED}Failed: ${failed}${NC}"
    
    return ${failed}
}

# Main
main() {
    # Crear build directory si no existe
    mkdir -p "${BUILD_DIR}"
    
    # Determinar qué compilar
    local target="${1:-all}"
    
    case "$target" in
        gcc)
            build_gcc
            ;;
        clang)
            build_clang
            ;;
        msvc)
            build_msvc
            ;;
        intel)
            build_intel
            ;;
        all)
            build_all
            ;;
        *)
            echo -e "${RED}Error: Unknown target '${target}'${NC}"
            echo -e "Usage: $0 [gcc|clang|msvc|intel|all]"
            exit 1
            ;;
    esac
}

main "$@"
