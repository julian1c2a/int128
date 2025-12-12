#!/bin/bash
# Build rápido para desarrollo
# Compila y ejecuta un test específico en modo debug

if [ $# -eq 0 ]; then
    echo "Uso: ./test.sh <test_name>"
    echo "Ejemplo: ./test.sh user_literals_test"
    exit 1
fi

TEST_NAME="$1"

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Call the main build script in debug mode
"$SCRIPT_DIR/build.sh" "$TEST_NAME" "debug"