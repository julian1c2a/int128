#!/usr/bin/env bash
# ==============================================================================
# build_generic.bash - Script genérico de compilación
# ==============================================================================
# Compila tests o benchmarks con todos los compiladores (GCC, Clang, Intel, MSVC)
# 
# Uso:
#   build_generic.bash <type> <feature> <target> [compiler] [mode] [print]
#
# Argumentos:
#   type     : uint128 | int128
#   feature  : bits | numeric | algorithm | iostreams | cmath | etc.
#   target   : tests | benchs
#   compiler : gcc | clang | intel | msvc | all (default: all)
#   mode     : debug | release | all (default: all)
#   print    : yes | no (default: no) - Imprime comandos de compilación
#
# Ejemplos:
#   build_generic.bash uint128 bits tests
#   build_generic.bash int128 numeric benchs gcc release
#   build_generic.bash uint128 algorithm tests all all yes
# ==============================================================================

set -euo pipefail

# ========================= Parse Arguments =========================

if [[ $# -lt 3 ]]; then
    echo "Error: Se requieren al menos 3 argumentos"
    echo "Uso: $0 <type> <feature> <target> [compiler] [mode] [print]"
    echo ""
    echo "Ejemplos:"
    echo "  $0 uint128 bits tests"
    echo "  $0 int128 numeric benchs gcc release"
    echo "  $0 uint128 algorithm tests all all yes"
    exit 1
fi

TYPE="$1"
FEATURE="$2"
TARGET="$3"
COMPILER="${4:-all}"
MODE="${5:-all}"
PRINT_COMMANDS="${6:-no}"

# ========================= Validation =========================

if [[ "$TYPE" != "uint128" && "$TYPE" != "int128" ]]; then
    echo "Error: TYPE debe ser 'uint128' o 'int128'"
    exit 1
fi

if [[ "$TARGET" != "tests" && "$TARGET" != "benchs" ]]; then
    echo "Error: TARGET debe ser 'tests' o 'benchs'"
    exit 1
fi

VALID_COMPILERS=("gcc" "clang" "intel" "msvc" "all")
if [[ ! " ${VALID_COMPILERS[*]} " =~ " ${COMPILER} " ]]; then
    echo "Error: COMPILER debe ser uno de: ${VALID_COMPILERS[*]}"
    exit 1
fi

VALID_MODES=("debug" "release" "all")
if [[ ! " ${VALID_MODES[*]} " =~ " ${MODE} " ]]; then
    echo "Error: MODE debe ser uno de: ${VALID_MODES[*]}"
    exit 1
fi

# ========================= Configuration =========================

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_ROOT"

if [[ "$TARGET" == "tests" ]]; then
    SOURCE_FILE="tests/${TYPE}_${FEATURE}_extracted_tests.cpp"
    BUILD_DIR="build/build_tests"
    OUTPUT_SUFFIX="tests"
else
    SOURCE_FILE="benchs/${TYPE}_${FEATURE}_extracted_benchs.cpp"
    BUILD_DIR="build/build_benchs"
    OUTPUT_SUFFIX="benchs"
fi

# ========================= Color Output =========================

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo_success() { echo -e "${GREEN}[OK] $1${NC}"; }
echo_error() { echo -e "${RED}[ERROR] $1${NC}"; }
echo_info() { echo -e "${YELLOW}-> $1${NC}"; }
echo_header() { echo -e "${BLUE}$1${NC}"; }

# ========================= Check Source File =========================

if [[ ! -f "$SOURCE_FILE" ]]; then
    echo_error "Source file not found: $SOURCE_FILE"
    exit 1
fi

echo_info "Building $TYPE $FEATURE $TARGET for all compilers..."
echo ""

# ========================= Compilation Function =========================

compile_with() {
    local compiler_name="$1"
    local compiler_cmd="$2"
    local skip_check="${3:-}"
    
    echo_info "Building with $compiler_name..."
    
    # Check if compiler exists (unless skip_check is set)
    if [[ -z "$skip_check" ]]; then
        if ! command -v "$compiler_cmd" &>/dev/null; then
            echo_error "$compiler_name not found ($compiler_cmd). Skipping..."
            echo ""
            return
        fi
    fi
    
    # Define modes to compile
    local modes_to_compile=()
    if [[ "$MODE" == "all" ]]; then
        modes_to_compile=("debug" "release")
    else
        modes_to_compile=("$MODE")
    fi
    
    # Compile for each mode
    for mode in "${modes_to_compile[@]}"; do
        local output_dir="$BUILD_DIR/$compiler_name/$mode"
        mkdir -p "$output_dir"
        
        local output="$output_dir/${TYPE}_${FEATURE}_${OUTPUT_SUFFIX}_${compiler_name}"
        
        # Add .exe extension for Windows compilers
        if [[ "$compiler_name" == "msvc" || "$compiler_name" == "intel" ]]; then
            output="${output}.exe"
        fi
        
        local common_flags="-std=c++20 -Wall -Wextra -pedantic -I./include"
        local mode_flags=""
        
        if [[ "$mode" == "debug" ]]; then
            mode_flags="-O0 -g3 -DDEBUG"
        else
            mode_flags="-O3 -march=native -DNDEBUG"
        fi
        
        echo_info "  Compiling [$mode]..."
        
        # Build command
        local cmd="$compiler_cmd $common_flags $mode_flags \"$SOURCE_FILE\" -o \"$output\""
        
        # Print command if requested
        if [[ "$PRINT_COMMANDS" == "yes" ]]; then
            echo_header "    $ $cmd"
        fi
        
        # Execute compilation
        if eval "$cmd" 2>&1; then
            if [[ -f "$output" ]]; then
                echo_success "  $compiler_name [$mode]: $output"
            else
                echo_error "  $compiler_name [$mode]: executable not created"
            fi
        else
            echo_error "  $compiler_name [$mode]: compilation failed"
        fi
    done
    
    echo ""
}

# ========================= Compile with Each Compiler =========================

if [[ "$COMPILER" == "gcc" || "$COMPILER" == "all" ]]; then
    GCC_CXX="${GCC_CXX:-g++}"
    compile_with "gcc" "$GCC_CXX" "skip_check"
fi

if [[ "$COMPILER" == "clang" || "$COMPILER" == "all" ]]; then
    CLANG_CXX="${CLANG_CXX:-clang++}"
    compile_with "clang" "$CLANG_CXX"
fi

if [[ "$COMPILER" == "intel" || "$COMPILER" == "all" ]]; then
    INTEL_CXX="${INTEL_CXX:-icx}"
    compile_with "intel" "$INTEL_CXX"
fi

if [[ "$COMPILER" == "msvc" || "$COMPILER" == "all" ]]; then
    MSVC_CXX="${MSVC_CXX:-cl.exe}"
    compile_with "msvc" "$MSVC_CXX"
fi

# ========================= Summary =========================

echo_success "Build complete for $TYPE $FEATURE $TARGET!"
