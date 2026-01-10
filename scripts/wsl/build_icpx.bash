#!/usr/bin/env bash
# =============================================================================
# WSL Build Script - Intel ICPX
# =============================================================================
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
source "$SCRIPT_DIR/common.bash"

# Load Intel oneAPI environment if available
if [[ -f /opt/intel/oneapi/setvars.sh ]]; then
    source /opt/intel/oneapi/setvars.sh --force > /dev/null 2>&1
fi

COMPILER_NAME="icpx"
COMPILER_CMD="icpx"

check_compiler "$COMPILER_CMD" "$COMPILER_NAME"
build_and_test "$COMPILER_NAME" "$COMPILER_CMD" "$@"
