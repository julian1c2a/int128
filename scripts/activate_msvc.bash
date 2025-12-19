#!/bin/bash

# Uso: source activate_msvc.bash [x64|x86]
# Si no se pasa argumento, usa x64 por defecto.

ARCH=${1:-x64}

# Comprobamos que python esté disponible
if ! command -v python &> /dev/null; then
    echo "Error: Python no encontrado. Instálalo o usa 'python3' si corresponde."
    return 1
fi

# Generamos las variables de entorno
OUTPUT=$(python scripts/vcvarsall.py "$ARCH")

if [ $? -ne 0 ]; then
    echo "Error al obtener entorno de MSVC."
    return 1
fi

# Evaluamos la salida en el shell actual
eval "$OUTPUT"

# Verificación rápida
if command -v cl &> /dev/null; then
    echo "✅ Entorno de Visual Studio ($ARCH) activado correctamente."
    echo "   Compilador: $(which cl)"
else
    echo "❌ Algo falló. 'cl.exe' no está en el PATH."
fi