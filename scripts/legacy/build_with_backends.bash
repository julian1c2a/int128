#!/usr/bin/env bash
#
# build_with_backends.bash - Compila benchmarks con backends opcionales
# Uso: ./build_with_backends.bash [gcc|clang|msvc|intel] [options]
#

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "${SCRIPT_DIR}")"
BENCHMARK_DIR="${PROJECT_ROOT}/benchmarks"
BUILD_DIR="${PROJECT_ROOT}/build/benchmarks"
BENCHMARK_RESULTS_DIR="${PROJECT_ROOT}/benchmark_results"

# Detectar compilador
COMPILER="${1:-gcc}"

# Opciones por defecto
ENABLE_UINT128=1
ENABLE_BOOST=0
ENABLE_BOOST_GMP=0
ENABLE_BOOST_TOMMATH=0

# Parse argumentos
shift
while [[ $# -gt 0 ]]; do
    case $1 in
        --no-uint128)
            ENABLE_UINT128=0
            shift
            ;;
        --with-boost)
            ENABLE_BOOST=1
            shift
            ;;
        --with-gmp)
            ENABLE_BOOST=1
            ENABLE_BOOST_GMP=1
            shift
            ;;
        --with-tommath)
            ENABLE_BOOST=1
            ENABLE_BOOST_TOMMATH=1
            shift
            ;;
        --all-backends)
            ENABLE_BOOST=1
            ENABLE_BOOST_GMP=1
            ENABLE_BOOST_TOMMATH=1
            shift
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            exit 1
            ;;
    esac
done

# Crear directorios
mkdir -p "${BUILD_DIR}"
mkdir -p "${BENCHMARK_RESULTS_DIR}"

# Nombres de los ejecutables
OUTPUT_EXE_UINT128="${BUILD_DIR}/uint128_benchmark_${COMPILER}"
OUTPUT_EXE_INT128="${BUILD_DIR}/int128_benchmark_${COMPILER}"

# Flags comunes
COMMON_FLAGS="-std=c++20 -O3 -Wall -I${PROJECT_ROOT}/include"
DEFINES=""

# Agregar defines según backends habilitados
if [ $ENABLE_UINT128 -eq 1 ]; then
    if [[ "$COMPILER" == "gcc" || "$COMPILER" == "clang" ]]; then
        DEFINES="$DEFINES -DHAS_UINT128_T -DHAS_INT128_T"
        echo -e "${GREEN}Enabled: __uint128_t and __int128_t native types${NC}"
    fi
fi

if [ $ENABLE_BOOST -eq 1 ]; then
    DEFINES="$DEFINES -DHAVE_BOOST"
    echo -e "${GREEN}Enabled: Boost.Multiprecision${NC}"
    
    if [ $ENABLE_BOOST_GMP -eq 1 ]; then
        DEFINES="$DEFINES -DHAVE_BOOST_GMP"
        echo -e "${GREEN}Enabled: Boost + GMP backend${NC}"
    fi
    
    if [ $ENABLE_BOOST_TOMMATH -eq 1 ]; then
        DEFINES="$DEFINES -DHAVE_BOOST_TOMMATH"
        echo -e "${GREEN}Enabled: Boost + tommath backend${NC}"
    fi
fi

# Bibliotecas
LIBS=""
if [ $ENABLE_BOOST_GMP -eq 1 ]; then
    LIBS="$LIBS -lgmp"
fi
if [ $ENABLE_BOOST_TOMMATH -eq 1 ]; then
    LIBS="$LIBS -ltommath"
fi

echo -e "\n${BLUE}Building with $COMPILER...${NC}"
echo -e "Defines: $DEFINES"
echo -e "Libraries: $LIBS"

# Compilar según compilador
case "$COMPILER" in
    gcc)
        if ! command -v g++ &> /dev/null; then
            echo -e "${RED}Error: g++ not found${NC}"
            exit 1
        fi
        
        local success=0
        local failed=0
        
        # uint128
        echo -e "${BLUE}  Building uint128 benchmark...${NC}"
        g++ $COMMON_FLAGS $DEFINES \
            "${BENCHMARK_DIR}/uint128_extracted_benchmarks.cpp" \
            -o "${OUTPUT_EXE_UINT128}" \
            $LIBS
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}  [OK] uint128 benchmark successful${NC}"
            ((success++))
        else
            echo -e "${RED}  [ERROR] uint128 benchmark failed${NC}"
            ((failed++))
        fi
        
        # int128
        echo -e "${BLUE}  Building int128 benchmark...${NC}"
        g++ $COMMON_FLAGS $DEFINES \
            "${BENCHMARK_DIR}/int128_extracted_benchmarks.cpp" \
            -o "${OUTPUT_EXE_INT128}" \
            $LIBS
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}  [OK] int128 benchmark successful${NC}"
            ((success++))
        else
            echo -e "${RED}  [ERROR] int128 benchmark failed${NC}"
            ((failed++))
        fi
        
        if [ $failed -gt 0 ]; then
            echo -e "${RED}[ERROR] GCC builds complete: ${success} successful, ${failed} failed${NC}"
            exit 1
        else
            echo -e "${GREEN}[OK] GCC builds complete: ${success} successful${NC}"
        fi
        ;;
        
    clang)
        if ! command -v clang++ &> /dev/null; then
            echo -e "${RED}Error: clang++ not found${NC}"
            exit 1
        fi
        
        local success=0
        local failed=0
        
        # uint128
        echo -e "${BLUE}  Building uint128 benchmark...${NC}"
        clang++ $COMMON_FLAGS $DEFINES \
            "${BENCHMARK_DIR}/uint128_extracted_benchmarks.cpp" \
            -o "${OUTPUT_EXE_UINT128}" \
            $LIBS
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}  [OK] uint128 benchmark successful${NC}"
            ((success++))
        else
            echo -e "${RED}  [ERROR] uint128 benchmark failed${NC}"
            ((failed++))
        fi
        
        # int128
        echo -e "${BLUE}  Building int128 benchmark...${NC}"
        clang++ $COMMON_FLAGS $DEFINES \
            "${BENCHMARK_DIR}/int128_extracted_benchmarks.cpp" \
            -o "${OUTPUT_EXE_INT128}" \
            $LIBS
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}  [OK] int128 benchmark successful${NC}"
            ((success++))
        else
            echo -e "${RED}  [ERROR] int128 benchmark failed${NC}"
            ((failed++))
        fi
        
        if [ $failed -gt 0 ]; then
            echo -e "${RED}[ERROR] Clang builds complete: ${success} successful, ${failed} failed${NC}"
            exit 1
        else
            echo -e "${GREEN}[OK] Clang builds complete: ${success} successful${NC}"
        fi
        ;;
        
    msvc)
        if ! command -v cl.exe &> /dev/null; then
            echo -e "${RED}Error: MSVC cl.exe not found${NC}"
            echo -e "${YELLOW}Run from Visual Studio Developer Command Prompt${NC}"
            exit 1
        fi
        
        # MSVC usa diferentes flags
        MSVC_FLAGS="/std:c++20 /O2 /EHsc /W4 /I${PROJECT_ROOT}/include"
        MSVC_DEFINES=$(echo "$DEFINES" | sed 's/-D/\/D/g')
        OUTPUT_EXE_UINT128_WIN="${OUTPUT_EXE_UINT128}.exe"
        OUTPUT_EXE_INT128_WIN="${OUTPUT_EXE_INT128}.exe"
        
        local success=0
        local failed=0
        
        # uint128
        echo -e "${BLUE}  Building uint128 benchmark...${NC}"
        cl.exe $MSVC_FLAGS $MSVC_DEFINES \
            "${BENCHMARK_DIR}/uint128_extracted_benchmarks.cpp" \
            /Fe:"${OUTPUT_EXE_UINT128_WIN}" \
            $LIBS
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}  [OK] uint128 benchmark successful${NC}"
            ((success++))
        else
            echo -e "${RED}  [ERROR] uint128 benchmark failed${NC}"
            ((failed++))
        fi
        
        # int128
        echo -e "${BLUE}  Building int128 benchmark...${NC}"
        cl.exe $MSVC_FLAGS $MSVC_DEFINES \
            "${BENCHMARK_DIR}/int128_extracted_benchmarks.cpp" \
            /Fe:"${OUTPUT_EXE_INT128_WIN}" \
            $LIBS
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}  [OK] int128 benchmark successful${NC}"
            ((success++))
        else
            echo -e "${RED}  [ERROR] int128 benchmark failed${NC}"
            ((failed++))
        fi
        
        if [ $failed -gt 0 ]; then
            echo -e "${RED}[ERROR] MSVC builds complete: ${success} successful, ${failed} failed${NC}"
            exit 1
        else
            echo -e "${GREEN}[OK] MSVC builds complete: ${success} successful${NC}"
        fi
        ;;
        
    intel)
        # Intentar icpx (oneAPI) primero
        if command -v icpx &> /dev/null; then
            INTEL_CXX=icpx
        elif command -v icx &> /dev/null; then
            INTEL_CXX=icx
        else
            echo -e "${RED}Error: Intel C++ compiler not found${NC}"
            echo -e "${YELLOW}Install Intel oneAPI or classic compiler${NC}"
            exit 1
        fi
        
        local success=0
        local failed=0
        
        # uint128
        echo -e "${BLUE}  Building uint128 benchmark...${NC}"
        $INTEL_CXX $COMMON_FLAGS $DEFINES \
            "${BENCHMARK_DIR}/uint128_extracted_benchmarks.cpp" \
            -o "${OUTPUT_EXE_UINT128}" \
            $LIBS
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}  [OK] uint128 benchmark successful${NC}"
            ((success++))
        else
            echo -e "${RED}  [ERROR] uint128 benchmark failed${NC}"
            ((failed++))
        fi
        
        # int128
        echo -e "${BLUE}  Building int128 benchmark...${NC}"
        $INTEL_CXX $COMMON_FLAGS $DEFINES \
            "${BENCHMARK_DIR}/int128_extracted_benchmarks.cpp" \
            -o "${OUTPUT_EXE_INT128}" \
            $LIBS
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}  [OK] int128 benchmark successful${NC}"
            ((success++))
        else
            echo -e "${RED}  [ERROR] int128 benchmark failed${NC}"
            ((failed++))
        fi
        
        if [ $failed -gt 0 ]; then
            echo -e "${RED}[ERROR] Intel builds complete ($INTEL_CXX): ${success} successful, ${failed} failed${NC}"
            exit 1
        else
            echo -e "${GREEN}[OK] Intel builds complete ($INTEL_CXX): ${success} successful${NC}"
            exit 1
        fi
        ;;
        
    *)
        echo -e "${RED}Error: Unknown compiler '${COMPILER}'${NC}"
        echo -e "Usage: $0 [gcc|clang|msvc|intel] [options]"
        echo -e "\nOptions:"
        echo -e "  --no-uint128          Disable __uint128_t (default: enabled for GCC/Clang)"
        echo -e "  --with-boost          Enable Boost.Multiprecision"
        echo -e "  --with-gmp            Enable Boost + GMP backend"
        echo -e "  --with-tommath        Enable Boost + tommath backend"
        echo -e "  --all-backends        Enable all backends"
        exit 1
        ;;
esac

echo -e "\n${GREEN}Build complete!${NC}"
echo -e "Run uint128: ${OUTPUT_EXE_UINT128}"
echo -e "Run int128: ${OUTPUT_EXE_INT128}"
