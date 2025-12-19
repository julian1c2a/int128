#!/usr/bin/env bash
#
# benchmark.bash
# Wrapper script para ejecutar benchmarks desde la raíz del proyecto
#
# Uso:
#   ./benchmark.bash [build|run|all|clean]
#

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Delegar al script real
exec "${SCRIPT_DIR}/scripts/benchmark.bash" "$@"
