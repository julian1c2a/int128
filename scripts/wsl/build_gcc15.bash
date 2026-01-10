#!/usr/bin/env bash
# =============================================================================
# WSL Build Script - GCC 15
# =============================================================================
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
source "$SCRIPT_DIR/common.bash"

COMPILER_NAME="gcc-15"
COMPILER_CMD="g++-15"

check_compiler "$COMPILER_CMD" "$COMPILER_NAME"
build_and_test "$COMPILER_NAME" "$COMPILER_CMD" "$@"
