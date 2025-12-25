#!/usr/bin/env bash
# ==============================================================================
# build_int128_thread_safety_extracted_benchs.bash
# ==============================================================================
# Compiles int128 thread_safety benchmarks with all 4 compilers (GCC, Clang, Intel, MSVC)
# Output: build/build_benchmarks/{compiler}/{debug|release}/int128_thread_safety_benchmarks_{compiler}[.exe]
# ==============================================================================

set -euo pipefail

# ========================= Configuration =========================

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_ROOT"

TYPE="int128"
FEATURE="thread_safety"
BENCH_FILE="benchs/${TYPE}_${FEATURE}_extracted_benchs.cpp"
BUILD_DIR="build/build_benchs"

# ========================= Color Output =========================

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo_success() { echo -e "${GREEN}[OK] $1${NC}"; }
echo_error() { echo -e "${RED}[ERROR] $1${NC}"; }
echo_info() { echo -e "${YELLOW}-> $1${NC}"; }

# ========================= Check Benchmark File =========================

if [[ ! -f "$BENCH_FILE" ]]; then
    echo_error "Benchmark file not found: $BENCH_FILE"
    exit 1
fi

echo_info "Building int128 thread_safety benchmarks for all compilers..."
echo ""

# ========================= GCC Compilation =========================

echo_info "Building with GCC..."
COMPILER="gcc"
GCC_CXX="${GCC_CXX:-g++}"

for MODE in debug release; do
    OUTPUT_DIR="$BUILD_DIR/$COMPILER/$MODE"
    mkdir -p "$OUTPUT_DIR"
    
    OUTPUT="$OUTPUT_DIR/${TYPE}_${FEATURE}_benchs_${COMPILER}"
    
    COMMON_FLAGS="-std=c++20 -Wall -Wextra -pedantic -I./include"
    
    if [[ "$MODE" == "debug" ]]; then
        MODE_FLAGS="-O0 -g3 -DDEBUG"
    else
        MODE_FLAGS="-O3 -march=native -DNDEBUG"
    fi
    
    echo_info "  Compiling [$MODE]..."
    
    if $GCC_CXX $COMMON_FLAGS $MODE_FLAGS "$BENCH_FILE" -o "$OUTPUT" -latomic 2>&1; then
        if [[ -f "$OUTPUT" ]]; then
            echo_success "  GCC [$MODE]: $OUTPUT"
        else
            echo_error "  GCC [$MODE]: executable not created"
        fi
    else
        echo_error "  GCC [$MODE]: compilation failed"
    fi
done

echo ""

# ========================= Clang Compilation =========================

echo_info "Building with Clang..."
COMPILER="clang"
CLANG_CXX="${CLANG_CXX:-clang++}"

if ! command -v "$CLANG_CXX" &>/dev/null; then
    echo_error "Clang not found (${CLANG_CXX}). Skipping..."
else
    for MODE in debug release; do
        OUTPUT_DIR="$BUILD_DIR/$COMPILER/$MODE"
        mkdir -p "$OUTPUT_DIR"
        
        OUTPUT="$OUTPUT_DIR/${TYPE}_${FEATURE}_benchs_${COMPILER}"
        
        COMMON_FLAGS="-std=c++20 -Wall -Wextra -pedantic -I./include"
        
        if [[ "$MODE" == "debug" ]]; then
            MODE_FLAGS="-O0 -g3 -DDEBUG"
        else
            MODE_FLAGS="-O3 -march=native -DNDEBUG"
        fi
        
        echo_info "  Compiling [$MODE]..."
        
        if $CLANG_CXX $COMMON_FLAGS $MODE_FLAGS "$BENCH_FILE" -o "$OUTPUT" -latomic 2>&1; then
            if [[ -f "$OUTPUT" ]]; then
                echo_success "  Clang [$MODE]: $OUTPUT"
            else
                echo_error "  Clang [$MODE]: executable not created"
            fi
        else
            echo_error "  Clang [$MODE]: compilation failed"
        fi
    done
fi

echo ""

# ========================= Intel Compilation =========================

echo_info "Building with Intel ICX..."
COMPILER="intel"
INTEL_CXX="${INTEL_CXX:-icx}"

if ! command -v "$INTEL_CXX" &>/dev/null; then
    echo_error "Intel ICX not found (${INTEL_CXX}). Skipping..."
else
    for MODE in debug release; do
        OUTPUT_DIR="$BUILD_DIR/$COMPILER/$MODE"
        mkdir -p "$OUTPUT_DIR"
        
        OUTPUT="$OUTPUT_DIR/${TYPE}_${FEATURE}_benchs_${COMPILER}"
        
        COMMON_FLAGS="-std=c++20 -Wall -Wextra -I./include"
        
        if [[ "$MODE" == "debug" ]]; then
            MODE_FLAGS="-O0 -g -DDEBUG"
        else
            MODE_FLAGS="-O3 -march=native -DNDEBUG"
        fi
        
        echo_info "  Compiling [$MODE]..."
        
        if $INTEL_CXX $COMMON_FLAGS $MODE_FLAGS "$BENCH_FILE" -o "$OUTPUT" 2>&1; then
            if [[ -f "$OUTPUT" ]]; then
                echo_success "  Intel [$MODE]: $OUTPUT"
            else
                echo_error "  Intel [$MODE]: executable not created"
            fi
        else
            echo_error "  Intel [$MODE]: compilation failed"
        fi
    done
fi

echo ""

# ========================= MSVC Compilation =========================

echo_info "Building with MSVC..."
COMPILER="msvc"
MSVC_CL="${MSVC_CL:-cl.exe}"

if ! command -v "$MSVC_CL" &>/dev/null; then
    echo_error "MSVC not found (${MSVC_CL}). Skipping..."
else
    export MSYS2_ARG_CONV_EXCL="*"
    
    for MODE in debug release; do
        OUTPUT_DIR="$BUILD_DIR/$COMPILER/$MODE"
        mkdir -p "$OUTPUT_DIR"
        
        OUTPUT="$OUTPUT_DIR/${TYPE}_${FEATURE}_benchs_${COMPILER}.exe"
        
        COMMON_FLAGS="/std:c++20 /EHsc /W4 /I./include"
        
        if [[ "$MODE" == "debug" ]]; then
            MODE_FLAGS="/Od /Zi /DDEBUG"
        else
            MODE_FLAGS="/O2 /DNDEBUG"
        fi
        
        echo_info "  Compiling [$MODE]..."
        
        if $MSVC_CL $COMMON_FLAGS $MODE_FLAGS "$BENCH_FILE" /Fe:"$OUTPUT" /Fo:"$OUTPUT_DIR/" 2>&1; then
            # Clean up MSVC artifacts
            rm -f "$OUTPUT_DIR"/*.obj "$OUTPUT_DIR"/*.pdb 2>/dev/null || true
            
            if [[ -f "$OUTPUT" ]]; then
                echo_success "  MSVC [$MODE]: $OUTPUT"
            else
                echo_error "  MSVC [$MODE]: executable not created"
            fi
        else
            echo_error "  MSVC [$MODE]: compilation failed"
        fi
    done
fi

echo ""
echo_success "Build complete for int128 thread_safety benchmarks!"
