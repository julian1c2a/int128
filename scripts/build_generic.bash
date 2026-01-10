#!/usr/bin/env bash
# ==============================================================================
# build_generic.bash - Script genérico de compilación
# ==============================================================================
# Compila tests, benchmarks o demos con todos los compiladores (GCC, Clang, Intel, MSVC)
# 
# Uso:
#   # Para tests (template unificado - TYPE implícito):
#   build_generic.bash <feature> tests [compiler] [mode] [print]
#   
#   # Para benchs (necesita TYPE porque archivos son distintos):
#   build_generic.bash <type> <feature> benchs [compiler] [mode] [print]
#   
#   # Para demos:
#   build_generic.bash demos <category> <demo_name> [compiler] [mode] [print]
#
# Argumentos:
#   type     : uint128 | int128 (solo para benchs)
#   feature  : tt | t | bits | numeric | algorithm | etc. (o "demos" para demos)
#   target   : tests | benchs (o <category> si feature=demos)
#   compiler : gcc | clang | intel | msvc | all (default: all)
#   mode     : debug | release | all (default: all)
#   print    : yes | no (default: no) - Imprime comandos de compilación
#
# Ejemplos Tests (sin TYPE - template unificado):
#   build_generic.bash bits tests
#   build_generic.bash numeric tests gcc release
#   build_generic.bash algorithm tests all all yes
#
# Ejemplos Benchs (con TYPE):
#   build_generic.bash uint128 bits benchs
#   build_generic.bash int128 numeric benchs gcc release
#   build_generic.bash uint128 algorithm benchs all all yes
#
# Ejemplos Demos:
#   build_generic.bash demos tutorials 01_basic_operations gcc release
#   build_generic.bash demos examples ipv6_address clang debug
#   build_generic.bash demos showcase main gcc release
# ==============================================================================

set -euo pipefail

# ========================= Parse Arguments =========================

if [[ $# -lt 2 ]]; then
    echo "Error: Se requieren al menos 2 argumentos"
    echo ""
    echo "Uso Tests (sin TYPE):"
    echo "  $0 <feature> tests [compiler] [mode] [print]"
    echo "  $0 bits tests"
    echo "  $0 numeric tests gcc release"
    echo ""
    echo "Uso Benchs (con TYPE):"
    echo "  $0 <type> <feature> benchs [compiler] [mode] [print]"
    echo "  $0 uint128 bits benchs"
    echo "  $0 int128 numeric benchs gcc release"
    echo ""
    echo "Uso Demos:"
    echo "  $0 demos <category> <demo_name> [compiler] [mode]"
    exit 1
fi

# ========================= Detect Syntax =========================

# Determinar si es: tests, benchs (con TYPE), o demos
IS_DEMO=false
IS_BENCH=false
IS_TEST=false

if [[ "$1" == "demos" ]]; then
    IS_DEMO=true
    FEATURE="demos"
    CATEGORY="$2"
    DEMO_NAME="${3:-}"
    COMPILER="${4:-all}"
    MODE="${5:-all}"
    PRINT_COMMANDS="${6:-no}"
    
    if [[ -z "$DEMO_NAME" ]]; then
        echo "Error: Se requiere el nombre del demo"
        echo "Uso: $0 demos <category> <demo_name> [compiler] [mode]"
        exit 1
    fi
    
    # Validate category
    VALID_CATEGORIES=("general" "tutorials" "examples" "showcase" "comparison" "performance" "integration")
    if [[ ! " ${VALID_CATEGORIES[*]} " =~ " ${CATEGORY} " ]]; then
        echo "Error: CATEGORY debe ser uno de: ${VALID_CATEGORIES[*]}"
        exit 1
    fi

elif [[ "$1" == "uint128" || "$1" == "int128" ]]; then
    # Sintaxis benchs con TYPE explícito (legacy): <type> <feature> benchs [compiler] [mode]
    IS_BENCH=true
    TYPE="$1"
    FEATURE="$2"
    TARGET="${3:-benchs}"
    COMPILER="${4:-all}"
    MODE="${5:-all}"
    PRINT_COMMANDS="${6:-no}"
    
    if [[ "$TARGET" != "benchs" ]]; then
        echo "Error: Sintaxis con TYPE solo válida para benchs"
        echo "Para tests use: $0 <feature> tests [compiler] [mode]"
        exit 1
    fi

elif [[ "$2" == "tests" ]]; then
    # Sintaxis tests: <feature> tests [compiler] [mode]
    IS_TEST=true
    FEATURE="$1"
    TARGET="tests"
    TYPE="int128"  # Template unificado
    COMPILER="${3:-all}"
    MODE="${4:-all}"
    PRINT_COMMANDS="${5:-no}"

elif [[ "$2" == "benchs" ]]; then
    # Sintaxis benchs unificada: <feature> benchs [compiler] [mode]
    IS_BENCH=true
    FEATURE="$1"
    TARGET="benchs"
    TYPE="int128"  # Default para legacy fallback
    COMPILER="${3:-all}"
    MODE="${4:-all}"
    PRINT_COMMANDS="${5:-no}"

else
    echo "Error: No se pudo determinar el tipo de build"
    echo ""
    echo "Sintaxis válidas:"
    echo "  Tests:  $0 <feature> tests [compiler] [mode]"
    echo "  Benchs: $0 <type> <feature> benchs [compiler] [mode]"
    echo "  Demos:  $0 demos <category> <demo_name> [compiler] [mode]"
    exit 1
fi

# ========================= Validation =========================

# Validate feature (for tests and benchs)
if [[ "$IS_TEST" == true || "$IS_BENCH" == true ]]; then
    VALID_FEATURES=("t" "tt" "traits" "limits" "concepts" "algorithm" "algorithms" "iostreams" "bits" "cmath" "numeric" "ranges" "format" "safe" "thread_safety" "comparison_boost" "interop")
    if [[ ! " ${VALID_FEATURES[*]} " =~ " ${FEATURE} " ]]; then
        echo "Error: FEATURE debe ser uno de: ${VALID_FEATURES[*]}"
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
    OUTPUT_SUFFIX="${DEMO_NAME}"
    echo_info "Building demo: ${CATEGORY}/${DEMO_NAME}..."
    
elif [[ "$IS_TEST" == true ]]; then
    # Tests usan template unificado (int128_*)
    # Primero intentar int128_<feature>_extracted_tests.cpp
    # Si no existe, intentar int128_<feature>_tests.cpp (alias: tt → t)
    if [[ "$FEATURE" == "tt" ]]; then
        SOURCE_FILE="tests/int128_tt_extracted_tests.cpp"
    else
        SOURCE_FILE="tests/int128_${FEATURE}_extracted_tests.cpp"
    fi
    BUILD_DIR="build/build_tests"
    OUTPUT_SUFFIX="tests"
    echo_info "Building tests for feature '$FEATURE' (template unificado)..."
    
elif [[ "$IS_BENCH" == true ]]; then
    # Benchmarks: primero intentar archivo unificado int128_<feature>_extracted_benchs.cpp
    # Si no existe, caer al formato con TYPE: <type>_<feature>_extracted_benchs.cpp
    BUILD_DIR="build/build_benchs"
    OUTPUT_SUFFIX="benchs"
    
    # Intentar formato unificado primero
    SOURCE_FILE="benchs/int128_${FEATURE}_extracted_benchs.cpp"
    
    if [[ ! -f "$SOURCE_FILE" ]]; then
        # Formato legacy con TYPE
        if [[ "$FEATURE" == "tt" ]]; then
            SOURCE_FILE="benchs/${TYPE}_t_extracted_benchs.cpp"
        else
            SOURCE_FILE="benchs/${TYPE}_${FEATURE}_extracted_benchs.cpp"
        fi
        echo_info "Building benchs for ${TYPE} ${FEATURE} (legacy format)..."
    else
        echo_info "Building benchs for ${FEATURE} (unified template)..."
    fi
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
