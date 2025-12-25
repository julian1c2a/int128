#!/usr/bin/env bash
# ==============================================================================
# check_uint128_algorithm_extracted_tests.bash
# ==============================================================================
# Executes uint128 algorithm test binaries across all compilers and modes
# Displays results in a summary matrix
# ==============================================================================

set -euo pipefail

# ========================= Configuration =========================

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_ROOT"

TYPE="uint128"
FEATURE="algorithm"
TEST_SOURCE="test_algorithm_functions"
BUILD_DIR="$PROJECT_ROOT/build/build_tests"

# ========================= Color Output =========================

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo_success() { echo -e "${GREEN}✓ $1${NC}"; }
echo_error() { echo -e "${RED}✗ $1${NC}"; }
echo_info() { echo -e "${YELLOW}➜ $1${NC}"; }
echo_header() { echo -e "${BLUE}$1${NC}"; }

# ========================= Test Results Tracking =========================

declare -A test_results

run_test() {
    local compiler="$1"
    local mode="$2"
    local exe_name="${TYPE}_${FEATURE}_tests_${compiler}"
    
    if [[ "$compiler" == "msvc" ]]; then
        exe_name="${exe_name}.exe"
    fi
    
    local exe_path="$BUILD_DIR/$compiler/$mode/$exe_name"
    local key="${compiler}_${mode}"
    
    if [[ ! -f "$exe_path" ]]; then
        test_results["$key"]="NOT_FOUND"
        return
    fi
    
    echo_info "Running: $compiler [$mode]"
    
    if "$exe_path" &>/dev/null; then
        test_results["$key"]="PASS"
        echo_success "  $compiler [$mode]: PASSED"
    else
        test_results["$key"]="FAIL"
        echo_error "  $compiler [$mode]: FAILED"
    fi
}

# ========================= Execute Tests =========================

echo_header "========================================================================"
echo_header "  UINT128 algorithm TESTS - Execution"
echo_header "========================================================================"
echo ""

for compiler in gcc clang intel msvc; do
    for mode in debug release; do
        run_test "$compiler" "$mode"
    done
    echo ""
done

# ========================= Display Summary =========================

echo_header "========================================================================"
echo_header "  TEST RESULTS SUMMARY"
echo_header "========================================================================"
echo ""

printf "%-12s | %-10s | %-10s\n" "Compiler" "Debug" "Release"
echo "------------------------------------------------------------------------"

for compiler in gcc clang intel msvc; do
    debug_result="${test_results[${compiler}_debug]:-NOT_RUN}"
    release_result="${test_results[${compiler}_release]:-NOT_RUN}"
    
    # Color coding
    if [[ "$debug_result" == "PASS" ]]; then
        debug_display="${GREEN}✓ PASS${NC}"
    elif [[ "$debug_result" == "FAIL" ]]; then
        debug_display="${RED}✗ FAIL${NC}"
    else
        debug_display="${YELLOW}⊘ SKIP${NC}"
    fi
    
    if [[ "$release_result" == "PASS" ]]; then
        release_display="${GREEN}✓ PASS${NC}"
    elif [[ "$release_result" == "FAIL" ]]; then
        release_display="${RED}✗ FAIL${NC}"
    else
        release_display="${YELLOW}⊘ SKIP${NC}"
    fi
    
    printf "%-12s | " "$compiler"
    echo -en "$debug_display"
    printf " | "
    echo -e "$release_display"
done

echo "========================================================================"
echo ""

# ========================= Exit Code =========================

failed_count=0
for result in "${test_results[@]}"; do
    if [[ "$result" == "FAIL" ]]; then
        ((failed_count++))
    fi
done

if [[ $failed_count -gt 0 ]]; then
    echo_error "Tests failed: $failed_count configuration(s)"
    exit 1
else
    echo_success "All tests passed!"
    exit 0
fi
