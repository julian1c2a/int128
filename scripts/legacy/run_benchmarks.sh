#!/bin/bash
# Script para ejecutar benchmarks normalizados de uint128_t
# Genera resultados comparables entre diferentes sistemas

set -e

BUILD_DIR="build"
CONFIG="release"  # Siempre release para benchmarks
TEST_NAME="normalized_benchmark"

echo "=== uint128_t Normalized Benchmark Runner ==="
echo "Building optimized version for accurate benchmarks..."

# Determine compiler command
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    # MSYS2 environment
    if command -v cl &> /dev/null; then
        echo "Using MSVC compiler through MSYS2"
        COMPILER="cl"
        COMPILE_CMD="cl /std:c++latest /O2 /DNDEBUG /Fe:$(cygpath -w "$BUILD_DIR/$CONFIG/${TEST_NAME}.exe") tests/$TEST_NAME.cpp"
    elif command -v g++ &> /dev/null; then
        echo "Using GCC in MSYS2"
        COMPILER="g++"
        COMPILE_CMD="g++ -std=c++20 -O3 -DNDEBUG -o $BUILD_DIR/$CONFIG/$TEST_NAME tests/$TEST_NAME.cpp"
    fi
else
    # Standard Unix-like
    echo "Using GCC/Clang"
    COMPILER="g++"
    COMPILE_CMD="g++ -std=c++20 -O3 -DNDEBUG -o $BUILD_DIR/$CONFIG/$TEST_NAME tests/$TEST_NAME.cpp"
fi

# Create build directories
mkdir -p "$BUILD_DIR/$CONFIG"
mkdir -p "documentation/benchmarks"

echo "Compiler: $COMPILER"
echo "Optimization: Maximum (-O2/-O3)"
echo ""

# Compile
echo "Compiling $TEST_NAME..."
if ! $COMPILE_CMD; then
    echo "❌ Compilation failed!"
    exit 1
fi

echo "✅ Compilation successful!"
echo ""

# Run benchmark
echo "Running normalized benchmark (this may take a few minutes)..."
echo "📊 Collecting performance statistics..."

if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    ./"$BUILD_DIR/$CONFIG/${TEST_NAME}.exe"
else
    ./"$BUILD_DIR/$CONFIG/$TEST_NAME"
fi

echo ""
echo "=== Benchmark Complete ==="
echo "📁 Results saved to: documentation/benchmarks/"
echo "🔍 CSV files contain detailed statistics for cross-machine comparison"
echo ""
echo "Key metrics explained:"
echo "  • Median_ns: Middle value (reduces outlier impact)"
echo "  • Relative_to_Baseline: Performance vs native uint64_t (lower is better)"
echo "  • Ops_per_second: Throughput measure"
echo ""
echo "💡 Tip: Run this on different machines to compare architectures!"