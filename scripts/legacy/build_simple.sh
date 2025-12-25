#!/bin/bash
# Script de build simple para uint128_t
# Uso: ./build_simple.sh [test_name] [debug|release]

TEST_NAME="${1:-}"
CONFIGURATION="${2:-debug}"

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$PROJECT_ROOT/build/test/$CONFIGURATION"

# Create build directory
mkdir -p "$BUILD_DIR"

# Compiler path
CL_EXE="/c/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.50.35717/bin/Hostx64/x64/cl.exe"

build_test() {
    local test_file="$1"
    local test_basename="${test_file%.cpp}"
    local test_source="tests/$test_file"
    local test_executable="$BUILD_DIR/${test_basename}.exe"
    
    echo -e "\033[36mBuilding $test_file ($CONFIGURATION)...\033[0m"
    
    # Build flags
    local flags=("/std:c++latest" "/EHsc" "/nologo" "/I" "include")
    if [ "$CONFIGURATION" = "debug" ]; then
        flags+=("/Od" "/Zi" "/MDd" "/DDEBUG")
    else
        flags+=("/O2" "/MD" "/DNDEBUG")
    fi
    
    # Add source and output
    flags+=("$test_source" "/Fe:$test_executable")
    
    # Execute compilation
    if "$CL_EXE" "${flags[@]}" > /dev/null 2>&1; then
        echo -e "\033[32m✓ $test_basename compiled successfully\033[0m"
        echo "$test_executable"
    else
        echo -e "\033[31m✗ $test_basename compilation failed\033[0m"
        echo -e "\033[33mDebug output:\033[0m"
        "$CL_EXE" "${flags[@]}"
        return 1
    fi
}

run_test() {
    local test_executable="$1"
    
    if [ -f "$test_executable" ]; then
        echo -e "\033[33mRunning $(basename "$test_executable")...\033[0m"
        echo "============================================================"
        "$test_executable"
        echo "============================================================"
        echo ""
    fi
}

# Main execution
echo -e "\033[35muint128_t Build System (Simple)\033[0m"
echo -e "\033[36mConfiguration: $CONFIGURATION\033[0m"
echo -e "\033[36mBuild Directory: $BUILD_DIR\033[0m"
echo ""

if [ -n "$TEST_NAME" ]; then
    # Build specific test
    if [[ "$TEST_NAME" == *.cpp ]]; then
        TEST_FILE="$TEST_NAME"
    else
        TEST_FILE="${TEST_NAME}.cpp"
    fi
    
    if test_executable=$(build_test "$TEST_FILE"); then
        run_test "$test_executable"
    fi
else
    echo "Uso: $0 <test_name> [debug|release]"
    echo "Ejemplo: $0 user_literals_test"
    exit 1
fi