#!/bin/bash
# Build script para uint128_t usando MSVC desde MSYS2
# Uso: ./msys_build.sh [test_name] [debug|release]

set -e  # Exit on any error

TEST_NAME="${1:-}"
CONFIGURATION="${2:-debug}"

if [ -z "$TEST_NAME" ]; then
    echo "Uso: $0 <test_name> [debug|release]"
    echo "Ejemplo: $0 user_literals_test debug"
    exit 1
fi

# Setup paths
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$PROJECT_ROOT/build/test/$CONFIGURATION"
mkdir -p "$BUILD_DIR"

# Add .exe extension if not present
if [[ "$TEST_NAME" != *.cpp ]]; then
    TEST_FILE="${TEST_NAME}.cpp"
else
    TEST_FILE="$TEST_NAME"
    TEST_NAME="${TEST_NAME%.cpp}"
fi

TEST_SOURCE="tests/$TEST_FILE"
TEST_EXECUTABLE="$BUILD_DIR/${TEST_NAME}.exe"

echo "========================================"
echo "uint128_t Build Script for MSYS2"
echo "========================================"
echo "Test: $TEST_NAME"
echo "Configuration: $CONFIGURATION"
echo "Source: $TEST_SOURCE"
echo "Output: $TEST_EXECUTABLE"
echo ""

# Check if source file exists
if [ ! -f "$TEST_SOURCE" ]; then
    echo "Error: Source file $TEST_SOURCE not found!"
    exit 1
fi

# Compiler flags
FLAGS="/std:c++latest /EHsc /nologo /I include"
if [ "$CONFIGURATION" = "debug" ]; then
    FLAGS="$FLAGS /Od /Zi /MDd /DDEBUG"
else
    FLAGS="$FLAGS /O2 /MD /DNDEBUG"
fi

# Use the Windows version of cl.exe with proper quoting
CL_PATH="/c/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.50.35717/bin/Hostx64/x64/cl.exe"

echo "Compiling..."
if "$CL_PATH" $FLAGS "$TEST_SOURCE" "/Fe:$TEST_EXECUTABLE"; then
    echo ""
    echo "✓ Compilation successful!"
    echo ""
    
    if [ -f "$TEST_EXECUTABLE" ]; then
        echo "Running test..."
        echo "----------------------------------------"
        "$TEST_EXECUTABLE"
        echo "----------------------------------------"
        echo "Test completed successfully!"
    else
        echo "Warning: Executable not found at $TEST_EXECUTABLE"
    fi
else
    echo ""
    echo "✗ Compilation failed!"
    exit 1
fi