#!/bin/bash
# =============================================================================
# verify_refactor_wsl.bash - Verifica int128_base_t con múltiples compiladores en WSL
# =============================================================================
#
# Uso: 
#   Desde Windows PowerShell: wsl bash /mnt/c/.../verify_refactor_wsl.bash
#   Desde WSL: bash verify_refactor_wsl.bash
#
# Compiladores probados:
#   - g++-13, g++-14, g++-15
#   - clang++-18, clang++-19, clang++-20, clang++-21
#   - icpx (Intel oneAPI)
#
# Requisitos en Ubuntu 25.04:
#   sudo apt install g++-13 g++-14 g++-15
#   sudo apt install clang-18 clang-19 clang-20 clang-21
#   # Intel oneAPI: https://software.intel.com/content/www/us/en/develop/tools/oneapi/base-toolkit/download.html
# =============================================================================

set -e  # Salir si hay error

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Directorio del script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo -e "${CYAN}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║   VERIFICACIÓN int128_base_t EN WSL - MÚLTIPLES COMPILADORES ║${NC}"
echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}"
echo ""

# Lista de compiladores a probar
declare -a GCC_COMPILERS=("g++-15" "g++-14" "g++-13" "g++")
declare -a CLANG_COMPILERS=("clang++-21" "clang++-20" "clang++-19" "clang++-18" "clang++")
declare -a INTEL_COMPILERS=("icpx")

# Archivos de test
declare -a TEST_FILES=(
    "test_bit_functions.cpp"
    "test_divrem_euclidean_properties.cpp"
)

# Flags de compilación
CXX_FLAGS="-std=c++20 -O2 -I. -Wall -Wextra"

# Contadores
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
SKIPPED_TESTS=0

# Función para verificar si un compilador existe
compiler_exists() {
    command -v "$1" &> /dev/null
}

# Función para compilar y ejecutar un test
run_test() {
    local compiler="$1"
    local test_file="$2"
    local test_name="${test_file%.cpp}"
    local output_file="${test_name}_${compiler//\//_}"
    
    ((TOTAL_TESTS++))
    
    echo -ne "  ${BLUE}[$compiler]${NC} $test_file ... "
    
    # Compilar
    if $compiler $CXX_FLAGS "$test_file" -o "$output_file" 2>/dev/null; then
        # Ejecutar
        if ./"$output_file" > /dev/null 2>&1; then
            echo -e "${GREEN}✓ PASS${NC}"
            ((PASSED_TESTS++))
            rm -f "$output_file"
            return 0
        else
            echo -e "${RED}✗ FAIL (runtime)${NC}"
            ((FAILED_TESTS++))
            rm -f "$output_file"
            return 1
        fi
    else
        echo -e "${RED}✗ FAIL (compile)${NC}"
        ((FAILED_TESTS++))
        return 1
    fi
}

# Función para probar un grupo de compiladores
test_compiler_group() {
    local group_name="$1"
    shift
    local compilers=("$@")
    
    echo -e "\n${YELLOW}=== $group_name ===${NC}"
    
    for compiler in "${compilers[@]}"; do
        if compiler_exists "$compiler"; then
            local version=$($compiler --version 2>/dev/null | head -n1)
            echo -e "${CYAN}Compilador encontrado: $compiler${NC}"
            echo -e "  Versión: $version"
            
            for test_file in "${TEST_FILES[@]}"; do
                if [[ -f "$test_file" ]]; then
                    run_test "$compiler" "$test_file"
                else
                    echo -e "  ${YELLOW}[SKIP]${NC} $test_file no encontrado"
                    ((SKIPPED_TESTS++))
                fi
            done
        else
            echo -e "${YELLOW}[SKIP]${NC} $compiler no encontrado"
            ((SKIPPED_TESTS++))
        fi
    done
}

# Verificar entorno Intel si está disponible
setup_intel() {
    if [[ -f "/opt/intel/oneapi/setvars.sh" ]]; then
        echo -e "${CYAN}Configurando Intel oneAPI...${NC}"
        source /opt/intel/oneapi/setvars.sh > /dev/null 2>&1
        return 0
    elif [[ -f "$HOME/intel/oneapi/setvars.sh" ]]; then
        echo -e "${CYAN}Configurando Intel oneAPI...${NC}"
        source "$HOME/intel/oneapi/setvars.sh" > /dev/null 2>&1
        return 0
    fi
    return 1
}

# ============================================================================
# EJECUCIÓN PRINCIPAL
# ============================================================================

echo -e "${CYAN}Directorio de trabajo: $(pwd)${NC}"
echo -e "${CYAN}Tests a ejecutar: ${TEST_FILES[*]}${NC}"
echo ""

# Probar compiladores GCC
test_compiler_group "GCC" "${GCC_COMPILERS[@]}"

# Probar compiladores Clang
test_compiler_group "CLANG" "${CLANG_COMPILERS[@]}"

# Probar Intel (requiere setup especial)
echo -e "\n${YELLOW}=== INTEL ===${NC}"
if setup_intel; then
    for compiler in "${INTEL_COMPILERS[@]}"; do
        if compiler_exists "$compiler"; then
            local version=$($compiler --version 2>/dev/null | head -n1)
            echo -e "${CYAN}Compilador encontrado: $compiler${NC}"
            echo -e "  Versión: $version"
            
            for test_file in "${TEST_FILES[@]}"; do
                if [[ -f "$test_file" ]]; then
                    run_test "$compiler" "$test_file"
                else
                    echo -e "  ${YELLOW}[SKIP]${NC} $test_file no encontrado"
                    ((SKIPPED_TESTS++))
                fi
            done
        else
            echo -e "${YELLOW}[SKIP]${NC} $compiler no encontrado"
            ((SKIPPED_TESTS++))
        fi
    done
else
    echo -e "${YELLOW}[SKIP]${NC} Intel oneAPI no encontrado"
    ((SKIPPED_TESTS++))
fi

# ============================================================================
# RESUMEN
# ============================================================================

echo ""
echo -e "${CYAN}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║                        RESUMEN                             ║${NC}"
echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}"
echo ""
echo -e "  Total tests: $TOTAL_TESTS"
echo -e "  ${GREEN}Pasados:${NC} $PASSED_TESTS"
echo -e "  ${RED}Fallidos:${NC} $FAILED_TESTS"
echo -e "  ${YELLOW}Saltados:${NC} $SKIPPED_TESTS"
echo ""

if [[ $FAILED_TESTS -eq 0 ]]; then
    echo -e "${GREEN}╔════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║   ✓ TODOS LOS TESTS PASARON                                ║${NC}"
    echo -e "${GREEN}╚════════════════════════════════════════════════════════════╝${NC}"
    exit 0
else
    echo -e "${RED}╔════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${RED}║   ✗ ALGUNOS TESTS FALLARON                                 ║${NC}"
    echo -e "${RED}╚════════════════════════════════════════════════════════════╝${NC}"
    exit 1
fi
