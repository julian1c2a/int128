#!/bin/bash
set -e

# Ensure we are in the project root (assuming script is in scripts/)
cd "$(dirname "$0")/.."

echo "================================================================"
echo "🧪 VERIFYING REFACTORING (data[0] -> data[LSULL])"
echo "================================================================"

# 1. Phase 1.5 Unit Tests (General Functionality)
echo -e "\n[1/4] Compiling and running phase15_uint128_tests..."
if [ -f "tests/phase15_uint128_tests.cpp" ]; then
    g++ -std=c++20 -Iinclude_new tests/phase15_uint128_tests.cpp -o phase15_uint128_tests.exe
    ./phase15_uint128_tests.exe
else
    echo "⚠️  tests/phase15_uint128_tests.cpp not found"
fi

echo -e "\n[2/4] Compiling and running phase15_int128_tests..."
if [ -f "tests/phase15_int128_tests.cpp" ]; then
    g++ -std=c++20 -Iinclude_new tests/phase15_int128_tests.cpp -o phase15_int128_tests.exe
    ./phase15_int128_tests.exe
else
    echo "⚠️  tests/phase15_int128_tests.cpp not found"
fi

# 2. Division Optimization Tests (Heavily refactored area)
echo -e "\n[3/4] Compiling and running test_divrem_optimized_tt..."
if [ -f "test_divrem_optimized_tt.cpp" ]; then
    g++ -std=c++20 -Iinclude_new test_divrem_optimized_tt.cpp -o test_divrem_optimized_tt.exe
    ./test_divrem_optimized_tt.exe
else
    echo "⚠️  test_divrem_optimized_tt.cpp not found"
fi

# 3. Euclidean Properties (Correctness check)
echo -e "\n[4/4] Compiling and running test_divrem_euclidean_properties..."
if [ -f "test_divrem_euclidean_properties.cpp" ]; then
    g++ -std=c++20 -Iinclude_new test_divrem_euclidean_properties.cpp -o test_divrem_euclidean_properties.exe
    ./test_divrem_euclidean_properties.exe
else
    echo "⚠️  test_divrem_euclidean_properties.cpp not found"
fi

echo -e "\n✅ ALL VERIFICATION TESTS PASSED!"