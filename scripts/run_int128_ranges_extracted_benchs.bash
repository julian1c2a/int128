#!/usr/bin/env bash
# ==============================================================================
# run_int128_iostreams_extracted_benchs.bash
# ==============================================================================
# Executes int128 iostreams benchmark binaries across all compilers and modes
# ==============================================================================

set -euo pipefail

# ========================= Configuration =========================

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_ROOT"

TYPE="int128"
FEATURE="ranges"
BUILD_DIR="$PROJECT_ROOT/build/build_benchmarks"

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

# ========================= Execute Benchmarks =========================

echo_header "========================================================================"
echo_header "  INT128 iostreams BENCHMARKS - Execution"
echo_header "========================================================================"
echo ""

executed=0
failed=0

for compiler in gcc clang intel msvc; do
    for mode in debug release; do
        exe_name="${TYPE}_${FEATURE}_benchmarks_${compiler}"
        
        if [[ "$compiler" == "msvc" ]]; then
            exe_name="${exe_name}.exe"
        fi
        
        exe_path="$BUILD_DIR/$compiler/$mode/$exe_name"
        
        if [[ ! -f "$exe_path" ]]; then
            echo_info "Skipping: $compiler [$mode] (not found)"
            continue
        fi
        
        echo_header "--------------------------------------------------------------------"
        echo_info "Running: $compiler [$mode]"
        echo_header "--------------------------------------------------------------------"
        
        if "$exe_path"; then
            echo_success "  $compiler [$mode]: completed"
            ((executed++))
        else
            echo_error "  $compiler [$mode]: failed"
            ((failed++))
        fi
        
        echo ""
    done
done

# ========================= Summary =========================

echo_header "========================================================================"
echo_header "  BENCHMARK EXECUTION SUMMARY"
echo_header "========================================================================"
echo ""
echo "Executed: $executed"
echo "Failed: $failed"
echo ""

if [[ $failed -gt 0 ]]; then
    echo_error "Some benchmarks failed"
    exit 1
else
    echo_success "All benchmarks completed successfully!"
    exit 0
fi
