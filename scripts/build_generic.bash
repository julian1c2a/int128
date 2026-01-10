#!/usr/bin/env bash
# ==============================================================================
# build_generic.bash - Script genérico de compilación
# ==============================================================================
# Compila tests, benchmarks o demos con todos los compiladores (GCC, Clang, Intel, MSVC)
# 
# Uso:
#   # Para tests y benchs (sintaxis simplificada - template unificado):
#   build_generic.bash <feature> <target> [compiler] [mode] [print]
#   
#   # Para demos:
#   build_generic.bash demos <category> <demo_name> [compiler] [mode] [print]
#
# Argumentos:
#   feature  : tt | bits | numeric | algorithm | etc. (o "demos" para demos)
#   target   : tests | benchs (o <category> si feature=demos)
#   compiler : gcc | clang | intel | msvc | all (default: all)
#   mode     : debug | release | all (default: all)
#   print    : yes | no (default: no) - Imprime comandos de compilación
#
# Ejemplos Tests/Benchs:
#   build_generic.bash bits tests
#   build_generic.bash numeric benchs gcc release
#   build_generic.bash algorithm tests all all yes
#
# Ejemplos Demos:
#   build_generic.bash demos tutorials 01_basic_operations gcc release
#   build_generic.bash demos examples ipv6_address clang debug
#   build_generic.bash demos showcase main gcc release
#
# NOTA: El tipo "int128" se usa implícitamente (template unificado int128_base_t<S>)
# ==============================================================================

set -euo pipefail

# ========================= Parse Arguments =========================

if [[ $# -lt 2 ]]; then
    echo "Error: Se requieren al menos 2 argumentos"
    echo "Uso: $0 <feature> <target> [compiler] [mode] [print]"
    echo ""
    echo "Tests/Benchs:"
    echo "  $0 bits tests"
    echo "  $0 numeric benchs gcc release"
    echo "  $0 algorithm tests all all yes"
    echo ""
    echo "Demos:"
    echo "  $0 demos tutorials 01_basic_operations gcc release"
    echo "  $0 demos examples ipv6_address clang debug"
    exit 1
fi

# Compatibilidad hacia atrás: detectar sintaxis vieja (uint128/int128 como primer arg)
if [[ "$1" == "uint128" || "$1" == "int128" ]]; then
    echo "-> NOTA: Sintaxis vieja detectada. TYPE '$1' ignorado (template unificado)."
    shift  # Eliminar el primer argumento (TYPE)
fi

FEATURE="$1"
TARGET="$2"
COMPILER="${3:-all}"
MODE="${4:-all}"
PRINT_COMMANDS="${5:-no}"

# TYPE siempre es int128 con el template unificado
TYPE="int128"

# ========================= Validation =========================

# Determine if this is a demo build
IS_DEMO=false
if [[ "$FEATURE" == "demos" ]]; then
    IS_DEMO=true
    CATEGORY="$TARGET"
    DEMO_NAME="$COMPILER"
    # Shift arguments for demos
    COMPILER="${MODE:-all}"
    MODE="${PRINT_COMMANDS:-all}"
    PRINT_COMMANDS="${6:-no}"
    
    # Validate category
    VALID_CATEGORIES=("general" "tutorials" "examples" "showcase" "comparison" "performance" "integration")
    if [[ ! " ${VALID_CATEGORIES[*]} " =~ " ${CATEGORY} " ]]; then
        echo "Error: CATEGORY debe ser uno de: ${VALID_CATEGORIES[*]}"
        exit 1
    fi
else
    # Validate for tests/benchs
    VALID_FEATURES=("t" "tt" "traits" "limits" "concepts" "algorithm" "algorithms" "iostreams" "bits" "cmath" "numeric" "ranges" "format" "safe" "thread_safety" "comparison_boost" "interop")
    if [[ ! " ${VALID_FEATURES[*]} " =~ " ${FEATURE} " ]]; then
        echo "Error: FEATURE debe ser uno de: ${VALID_FEATURES[*]}"
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

VALID_MODES=("debug" "debug-asan" "debug-ubsan" "release" "release-O1" "release-O2" "release-O3" "release-Ofast" "all")
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
        # GCC en Windows/MSYS2 no tiene sanitizers disponibles
        # OSTYPE puede ser "msys", "cygwin", o "mingw*" dependiendo del entorno
        if [[ "$compiler_name" == "gcc" && ("$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" || "$OSTYPE" == mingw*) ]]; then
            modes_to_compile=("debug" "release" "release-O1" "release-O2" "release-O3" "release-Ofast")
            echo_info "  (Nota: sanitizers no disponibles para GCC en MSYS2, saltando debug-asan/ubsan)"
        else
            modes_to_compile=("debug" "debug-asan" "debug-ubsan" "release" "release-O1" "release-O2" "release-O3" "release-Ofast")
        fi
    else
        # Si el usuario pide específicamente un sanitizer con GCC en MSYS2, advertir
        if [[ "$compiler_name" == "gcc" && ("$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" || "$OSTYPE" == mingw*) && ("$MODE" == "debug-asan" || "$MODE" == "debug-ubsan") ]]; then
            echo_error "Sanitizers no disponibles para GCC en MSYS2. Use Clang o Intel."
            return
        fi
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
        
        local common_flags=""
        local mode_flags=""
        local link_flags=""
        
        # Set compiler-specific flags
        if [[ "$compiler_name" == "msvc" ]]; then
            # MSVC flags - convert paths to Windows format
            # IMPORTANT: Use MSYS2_ARG_CONV_EXCL to prevent MSYS2 from converting /flags to paths
            export MSYS2_ARG_CONV_EXCL="*"
            
            local win_source=$(cygpath -w "$SOURCE_FILE" 2>/dev/null || echo "$SOURCE_FILE" | sed 's|/|\\|g')
            local win_output=$(cygpath -w "$output" 2>/dev/null || echo "$output" | sed 's|/|\\|g')
            local win_include=$(cygpath -w "./include" 2>/dev/null || echo ".\\include")
            
            common_flags="/std:c++20 /W4 /EHsc /I\"$win_include\""
            
            if [[ "$mode" == "debug" ]]; then
                mode_flags="/Od /Zi /DDEBUG"
            elif [[ "$mode" == "debug-asan" ]]; then
                mode_flags="/Od /Zi /DDEBUG /fsanitize=address"
            elif [[ "$mode" == "debug-ubsan" ]]; then
                # MSVC doesn't have UBSan, use RTC instead
                mode_flags="/Od /Zi /DDEBUG /RTC1"
            elif [[ "$mode" == "release-O1" ]]; then
                mode_flags="/O1 /DNDEBUG"
            elif [[ "$mode" == "release-O2" ]]; then
                mode_flags="/O2 /DNDEBUG"
            elif [[ "$mode" == "release-O3" ]]; then
                mode_flags="/Ox /GL /DNDEBUG"
            elif [[ "$mode" == "release-Ofast" ]]; then
                mode_flags="/Ox /GL /fp:fast /DNDEBUG"
            else
                # Default release
                mode_flags="/O2 /DNDEBUG"
            fi
            
            # MSVC output flag is /Fe not -o
            local cmd="$compiler_cmd $common_flags $mode_flags \"$win_source\" /Fe:\"$win_output\""
        elif [[ "$compiler_name" == "intel" ]]; then
            # Intel ICX in clang-cl mode (Windows with MSVC environment)
            # Uses -Qstd:c++20 instead of -std=c++20
            # IMPORTANT: Use - prefix instead of / to avoid MSYS2 path conversion
            local win_source=$(cygpath -w "$SOURCE_FILE" 2>/dev/null || echo "$SOURCE_FILE" | sed 's|/|\\|g')
            local win_output=$(cygpath -w "$output" 2>/dev/null || echo "$output" | sed 's|/|\\|g')
            local win_include=$(cygpath -w "./include" 2>/dev/null || echo ".\\include")
            
            # Intel ICX flags in clang-cl mode (use - prefix to avoid MSYS2 path issues)
            # Use -Xclang to pass exception flags to the underlying clang frontend
            common_flags="-Qstd:c++20 -W4 -EHsc -Xclang -fcxx-exceptions -I\"$win_include\""
            
            if [[ "$mode" == "debug" ]]; then
                mode_flags="-Od -Zi -DDEBUG"
            elif [[ "$mode" == "debug-asan" ]]; then
                mode_flags="-Od -Zi -DDEBUG -fsanitize=address"
            elif [[ "$mode" == "debug-ubsan" ]]; then
                mode_flags="-Od -Zi -DDEBUG -fsanitize=undefined"
            elif [[ "$mode" == "release-O1" ]]; then
                mode_flags="-O1 -DNDEBUG"
            elif [[ "$mode" == "release-O2" ]]; then
                mode_flags="-O2 -DNDEBUG"
            elif [[ "$mode" == "release-O3" ]]; then
                mode_flags="-O3 -Qipo -DNDEBUG"
            elif [[ "$mode" == "release-Ofast" ]]; then
                mode_flags="-O3 -Qipo -fp:fast -DNDEBUG"
            else
                # Default release
                mode_flags="-O2 -DNDEBUG"
            fi
            
            local cmd="$compiler_cmd $common_flags $mode_flags \"$win_source\" -Fe:\"$win_output\""
        else
            # GCC/Clang/Intel flags
            common_flags="-std=c++20 -Wall -Wextra -pedantic -I./include"
            
            # Check if source file uses threading (for pthread and atomic flags)
            if grep -q -E '<thread>|std::thread|pthread|thread_safety\.hpp' "$SOURCE_FILE"; then
                common_flags="$common_flags -pthread"
                if grep -q -E '<atomic>|std::atomic|atomic_|thread_safety\.hpp' "$SOURCE_FILE"; then
                    link_flags="-latomic"
                fi
            fi
            
            if [[ "$mode" == "debug" ]]; then
                mode_flags="-O0 -g3 -DDEBUG"
            elif [[ "$mode" == "debug-asan" ]]; then
                mode_flags="-O0 -g3 -DDEBUG -fsanitize=address -fno-omit-frame-pointer"
                link_flags="$link_flags -fsanitize=address"
            elif [[ "$mode" == "debug-ubsan" ]]; then
                mode_flags="-O0 -g3 -DDEBUG -fsanitize=undefined -fno-omit-frame-pointer"
                link_flags="$link_flags -fsanitize=undefined"
            elif [[ "$mode" == "release-O1" ]]; then
                mode_flags="-O1 -DNDEBUG"
            elif [[ "$mode" == "release-O2" ]]; then
                mode_flags="-O2 -DNDEBUG"
            elif [[ "$mode" == "release-O3" ]]; then
                # GCC tiene -fexpensive-optimizations, Clang no
                if [[ "$compiler_name" == "gcc" ]]; then
                    mode_flags="-O3 -fexpensive-optimizations -funroll-loops -ftree-vectorize -march=native -DNDEBUG"
                else
                    mode_flags="-O3 -funroll-loops -ftree-vectorize -march=native -DNDEBUG"
                fi
            elif [[ "$mode" == "release-Ofast" ]]; then
                # -Ofast está deprecado en Clang, usar -O3 -ffast-math
                if [[ "$compiler_name" == "gcc" ]]; then
                    mode_flags="-Ofast -fexpensive-optimizations -funroll-loops -ftree-vectorize -ffast-math -march=native -DNDEBUG"
                else
                    mode_flags="-O3 -ffast-math -funroll-loops -ftree-vectorize -march=native -DNDEBUG"
                fi
            else
                # Default release
                mode_flags="-O3 -march=native -DNDEBUG"
            fi
            
            local cmd="$compiler_cmd $common_flags $mode_flags \"$SOURCE_FILE\" -o \"$output\" $link_flags"
        fi
        
        echo_info "  Compiling [$mode]..."
        
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
    echo_success "Build complete for $FEATURE $TARGET!"
fi
