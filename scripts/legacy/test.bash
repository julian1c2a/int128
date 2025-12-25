#!/bin/bash
# Wrapper para ejecutar el script de test desde la raíz del proyecto
# Simplemente redirige a scripts/test_extracted.bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
"$SCRIPT_DIR/scripts/test_extracted.bash" "$@"
