#!/bin/bash
# Test muy simple
echo "Starting build script..."
echo "Args: $1 $2"

TEST_NAME="$1"
echo "Test name: $TEST_NAME"

if [ -z "$TEST_NAME" ]; then
    echo "No test name provided"
    exit 1
fi

echo "About to compile..."
cd "$(dirname "$0")"
"/c/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.50.35717/bin/Hostx64/x64/cl.exe" \
    /std:c++latest /EHsc /nologo \
    /I include \
    "tests/${TEST_NAME}.cpp" \
    "/Fe:build/test/debug/${TEST_NAME}.exe"

echo "Compilation finished with exit code: $?"