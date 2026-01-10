#!/usr/bin/env bash
# =============================================================================
# WSL Common Functions - Shared by all compiler scripts
# =============================================================================

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

echo_info()    { echo -e "${CYAN}[INFO]${NC} $1"; }
echo_success() { echo -e "${GREEN}[OK]${NC} $1"; }
echo_error()   { echo -e "${RED}[FAIL]${NC} $1"; }
echo_warn()    { echo -e "${YELLOW}[WARN]${NC} $1"; }
echo_header()  { echo -e "${BLUE}$1${NC}"; }

# Paths (set by sourcing script)
PROJECT_ROOT="${PROJECT_ROOT:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
INCLUDE_DIR="$PROJECT_ROOT/include"
BUILD_BASE="$PROJECT_ROOT/build/build_tests"

# Valid features (template unificado int128_base_t)
VALID_FEATURES=("tt" "t" "algorithm" "bits" "cmath" "concepts" "format" "iostreams" "limits" "numeric" "ranges" "safe" "thread_safety" "traits")

# Check if compiler exists
check_compiler() {
    local cmd="$1"
    local name="$2"
    
    if ! command -v "$cmd" &> /dev/null; then
        echo_error "Compiler '$name' ($cmd) not found"
        exit 1
    fi
}

# Get source file for feature (always int128 template)
get_source_file() {
    local feature="$1"
    echo "$PROJECT_ROOT/tests/int128_${feature}_extracted_tests.cpp"
}

# Build and test function
# Usage: build_and_test <compiler_name> <compiler_cmd> <feature> [mode]
build_and_test() {
    local compiler_name="$1"
    local compiler_cmd="$2"
    local feature="$3"
    local mode="${4:-all}"
    
    # Determine source file
    local source_file
    source_file=$(get_source_file "$feature")
    
    if [[ ! -f "$source_file" ]]; then
        echo_error "Source file not found: $source_file"
        return 1
    fi
    
    local modes_to_run=()
    if [[ "$mode" == "all" ]]; then
        modes_to_run=("debug" "release")
    else
        modes_to_run=("$mode")
    fi
    
    local passed=0
    local failed=0
    
    for m in "${modes_to_run[@]}"; do
        echo_header "--- $compiler_name [$m] int128_${feature} ---"
        
        # Create output directory
        local output_dir="$BUILD_BASE/$compiler_name/$m"
        mkdir -p "$output_dir"
        local output="$output_dir/int128_${feature}_tests_$compiler_name"
        
        # Flags
        local common_flags="-std=c++20 -I\"$INCLUDE_DIR\" -Wall -Wextra -Wpedantic"
        local mode_flags=""
        local extra_flags=""
        
        case "$m" in
            debug)   mode_flags="-O0 -g" ;;
            release) mode_flags="-O2 -DNDEBUG" ;;
        esac
        
        # Thread safety requires extra flags
        local link_flags=""
        if [[ "$feature" == "thread_safety" ]]; then
            extra_flags="-pthread"
            # GCC/Clang need -latomic AFTER source file for 128-bit atomics
            if [[ "$compiler_cmd" == g++* ]] || [[ "$compiler_cmd" == clang++* ]]; then
                link_flags="-latomic"
            fi
        fi
        
        # Compile (link_flags after source file)
        local cmd="$compiler_cmd $common_flags $mode_flags $extra_flags \"$source_file\" -o \"$output\" $link_flags"
        echo_info "Compiling..."
        
        if eval "$cmd" 2>&1; then
            echo_success "Compilation OK"
        else
            echo_error "Compilation FAILED"
            failed=$((failed + 1))
            continue
        fi
        
        # Run tests
        echo_info "Running tests..."
        if "$output" 2>&1; then
            echo_success "Tests PASSED"
            passed=$((passed + 1))
        else
            echo_error "Tests FAILED"
            failed=$((failed + 1))
        fi
        echo ""
    done
    
    # Return status
    if [[ $failed -eq 0 ]]; then
        return 0
    else
        return 1
    fi
}

# Export functions and variables
export -f echo_info echo_success echo_error echo_warn echo_header
export -f check_compiler build_and_test get_source_file
export VALID_FEATURES
