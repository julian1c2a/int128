#!/bin/bash
# Script de build para uint128_t
# Uso: ./build.sh [test_name] [debug|release]

# Check if cygpath is available
if ! command -v cygpath &> /dev/null; then
    echo -e "\033[31mError: cygpath no está disponible. Instálalo con:\033[0m"
    echo "pacman -S cygpath"
    exit 1
fi

TEST_NAME="${1:-}"
CONFIGURATION="${2:-debug}"

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Convert to Windows paths using cygpath
PROJECT_ROOT_WIN="$(cygpath -w "$PROJECT_ROOT")"
INCLUDE_DIR_WIN="$(cygpath -w "$PROJECT_ROOT/include")"
TESTS_DIR_WIN="$(cygpath -w "$PROJECT_ROOT/tests")"
BUILD_DIR_WIN="$(cygpath -w "$PROJECT_ROOT/build/test/$CONFIGURATION")"

# Create build directory (using Unix paths)
BUILD_DIR_UNIX="$PROJECT_ROOT/build/test/$CONFIGURATION"
mkdir -p "$BUILD_DIR_UNIX"

# Compiler path - use cygpath to handle spaces in path
MSVC_DIR="/c/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.50.35717/bin/Hostx64/x64"
CL_EXE="$(cygpath -w "$MSVC_DIR/cl.exe")"

build_test() {
    local test_file="$1"
    local test_basename="${test_file%.cpp}"
    local test_source_unix="$PROJECT_ROOT/tests/$test_file"
    local test_executable_unix="$BUILD_DIR_UNIX/${test_basename}.exe"
    
    # Convert paths to Windows format
    local test_source_win="$(cygpath -w "$test_source_unix")"
    local test_executable_win="$(cygpath -w "$test_executable_unix")"
    
    echo -e "\033[36mBuilding $test_file ($CONFIGURATION)...\033[0m"
    
    # Build flags
    local flags="/std:c++latest /EHsc /nologo"
    if [ "$CONFIGURATION" = "debug" ]; then
        flags="$flags /Od /Zi /MDd /DDEBUG"
    else
        flags="$flags /O2 /MD /DNDEBUG"
    fi
    
    # Execute compilation using cmd to properly handle Windows paths
    local cmd="\"$CL_EXE\" $flags /I \"$INCLUDE_DIR_WIN\" \"$test_source_win\" /Fe:\"$test_executable_win\""
    
    if cmd.exe /c "$cmd" > /dev/null 2>&1; then
        echo -e "\033[32m✓ $test_basename compiled successfully\033[0m"
        echo "$test_executable_unix"
    else
        echo -e "\033[31m✗ $test_basename compilation failed\033[0m"
        echo -e "\033[33mCommand: $cmd\033[0m"
        cmd.exe /c "$cmd"
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
echo -e "\033[35muint128_t Build System\033[0m"
echo -e "\033[36mConfiguration: $CONFIGURATION\033[0m"
echo -e "\033[36mBuild Directory: $BUILD_DIR_UNIX\033[0m"
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
    # Build all tests
    successful_builds=()
    failed_builds=()
    
    for test_file in "$PROJECT_ROOT/tests"/*.cpp; do
        if [ "$(basename "$test_file")" != "test_uint128.cpp" ]; then
            test_name="$(basename "$test_file")"
            if test_executable=$(build_test "$test_name"); then
                successful_builds+=("$test_executable")
            else
                failed_builds+=("$test_name")
            fi
            echo ""
        fi
    done
    
    echo -e "\033[35mBuild Summary:\033[0m"
    echo -e "\033[32mSuccessfully built: ${#successful_builds[@]} tests\033[0m"
    echo -e "\033[31mFailed: ${#failed_builds[@]} tests\033[0m"
    
    if [ ${#failed_builds[@]} -gt 0 ]; then
        echo -e "\033[31mFailed tests:\033[0m"
        for failed in "${failed_builds[@]}"; do
            echo "  - $failed"
        done
    fi
    
    if [ ${#successful_builds[@]} -gt 0 ]; then
        echo ""
        read -p "¿Ejecutar todos los tests? (y/N): " -n 1 -r
        echo ""
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            for test_exe in "${successful_builds[@]}"; do
                run_test "$test_exe"
            done
        fi
    fi
fi