#!/usr/bin/env bash
# ==============================================================================
# build_generic.bash - Script genérico de compilación
# ==============================================================================
# Compila tests, benchmarks o demos con todos los compiladores (GCC, Clang, Intel, MSVC)
# 
# Uso:
#   # Para tests y benchs (sintaxis clásica):
#   build_generic.bash <type> <feature> <target> [compiler] [mode] [print]
#   
#   # Para demos (nueva sintaxis):
#   build_generic.bash demos <category> <demo_name> [compiler] [mode] [print]
#
# Argumentos:
#   type     : uint128 | int128 | demos
#   feature  : bits | numeric | algorithm | etc. (o <category> si type=demos)
#   target   : tests | benchs | <demo_name> (si type=demos)
#   compiler : gcc | clang | intel | msvc | all (default: all)
#   mode     : debug | release | all (default: all)
#   print    : yes | no (default: no) - Imprime comandos de compilación
#
# Ejemplos Tests/Benchs:
#   build_generic.bash uint128 bits tests
#   build_generic.bash int128 numeric benchs gcc release
#   build_generic.bash uint128 algorithm tests all all yes
#
# Ejemplos Demos:
#   build_generic.bash demos tutorials 01_basic_operations gcc release
#   build_generic.bash demos examples ipv6_address clang debug
#   build_generic.bash demos showcase main gcc release
# ==============================================================================

set -euo pipefail

# ========================= Parse Arguments =========================

if [[ $# -lt 3 ]]; then
    echo "Error: Se requieren al menos 3 argumentos"
    echo "Uso: $0 <type> <feature> <target> [compiler] [mode] [print]"
    echo ""
    echo "Tests/Benchs:"
    echo "  $0 uint128 bits tests"
    echo "  $0 int128 numeric benchs gcc release"
    echo "  $0 uint128 algorithm tests all all yes"
    echo ""
    echo "Demos:"
    echo "  $0 demos tutorials 01_basic_operations gcc release"
    echo "  $0 demos examples ipv6_address clang debug"
    exit 1
fi

TYPE="$1"
FEATURE="$2"
TARGET="$3"
COMPILER="${4:-all}"
MODE="${5:-all}"
PRINT_COMMANDS="${6:-no}"

# ========================= Validation =========================

# Determine if this is a demo build
IS_DEMO=false
if [[ "$TYPE" == "demos" ]]; then
    IS_DEMO=true
    CATEGORY="$FEATURE"
    DEMO_NAME="$TARGET"
    
    # Validate category
    VALID_CATEGORIES=("general" "tutorials" "examples" "showcase" "comparison" "performance" "integration")
    if [[ ! " ${VALID_CATEGORIES[*]} " =~ " ${CATEGORY} " ]]; then
        echo "Error: CATEGORY debe ser uno de: ${VALID_CATEGORIES[*]}"
        exit 1
    fi
else
    # Validate for tests/benchs
    if [[ "$TYPE" != "uint128" && "$TYPE" != "int128" ]]; then
        echo "Error: TYPE debe ser 'uint128', 'int128' o 'demos'"
        exit 1
    fi
    
    if [[ "$TARGET" != "tests" && "$TARGET" != "benchs" ]]; then
        echo "Error: TARGET debe ser 'tests' o 'benchs'"
        exit 1
    fi
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

# ========================= Determine Paths =========================

# Determine source file and build directory
if [[ "$IS_DEMO" == true ]]; then
    SOURCE_FILE="demos/${CATEGORY}/${DEMO_NAME}.cpp"
    BUILD_DIR="build/build_demos"
    OUTPUT_SUFFIX="${DEMO_NAME}"  # Output name is the demo name
    echo_info "Building demo: ${CATEGORY}/${DEMO_NAME}..."
elif [[ "$TARGET" == "tests" ]]; then
    SOURCE_FILE="tests/${TYPE}_${FEATURE}_extracted_tests.cpp"
    BUILD_DIR="build/build_tests"
    OUTPUT_SUFFIX="tests"
    echo_info "Building $TYPE $FEATURE $TARGET for all compilers..."
else
    SOURCE_FILE="benchs/${TYPE}_${FEATURE}_extracted_benchs.cpp"
    BUILD_DIR="build/build_benchs"
    OUTPUT_SUFFIX="benchs"
    echo_info "Building $TYPE $FEATURE $TARGET for all compilers..."
fi

# ========================= Check Source File =========================

if [[ ! -f "$SOURCE_FILE" ]]; then
    echo_error "Source file not found: $SOURCE_FILE"
    exit 1
fi

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
        
        # Build output filename
        if [[ "$IS_DEMO" == true ]]; then
            # Demos: just <demo_name>
            local output="$output_dir/${OUTPUT_SUFFIX}"
        else
            # Tests/benchs: <type>_<feature>_<suffix>_<compiler>
            local output="$output_dir/${TYPE}_${FEATURE}_${OUTPUT_SUFFIX}_${compiler_name}"
        fi
        
        # Add .exe extension for Windows compilers
        if [[ "$compiler_name" == "msvc" || "$compiler_name" == "intel" ]]; then
            output="${output}.exe"
        fi
        
        local common_flags="-std=c++20 -Wall -Wextra -pedantic -I./include"
        local mode_flags=""
        local link_flags=""
        
        # Check if source file uses threading (for pthread and atomic flags)
        if grep -q -E '<thread>|std::thread|pthread|thread_safety\.hpp' "$SOURCE_FILE"; then
            common_flags="$common_flags -pthread"
            if grep -q -E '<atomic>|std::atomic|atomic_|thread_safety\.hpp' "$SOURCE_FILE"; then
                link_flags="-latomic"
            fi
        fi
        
        if [[ "$mode" == "debug" ]]; then
            mode_flags="-O0 -g3 -DDEBUG"
        else
            mode_flags="-O3 -march=native -DNDEBUG"
        fi
        
        echo_info "  Compiling [$mode]..."
        
        # Build command
        local cmd="$compiler_cmd $common_flags $mode_flags \"$SOURCE_FILE\" -o \"$output\" $link_flags"
        
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

if [[ "$IS_DEMO" == true ]]; then
    echo_success "Build complete for demo ${CATEGORY}/${DEMO_NAME}!"
else
    echo_success "Build complete for $TYPE $FEATURE $TARGET!"
fi
