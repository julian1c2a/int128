#!/usr/bin/env bash
# =============================================================================
# WSL Build Script - Clang 20
# =============================================================================
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
source "$SCRIPT_DIR/common.bash"

COMPILER_NAME="clang-20"
COMPILER_CMD="clang++-20"

check_compiler "$COMPILER_CMD" "$COMPILER_NAME"
build_and_test "$COMPILER_NAME" "$COMPILER_CMD" "$@"
