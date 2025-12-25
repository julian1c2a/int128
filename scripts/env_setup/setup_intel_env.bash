#!/bin/bash

# =============================================================================
# SETUP INTEL ONEAPI + MSVC ENVIRONMENT (BASH)
# =============================================================================

# Obtener directorio del script actual
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PYTHON_SCRIPT="$SCRIPT_DIR/get_combined_env.py"
LOG_FILE="$SCRIPT_DIR/intel_debug.log"

echo "---------------------------------------------------------"
echo "   Iniciando configuración de entorno Intel oneAPI + MSVC"
echo "---------------------------------------------------------"

# 1. Verificar existencia del script Python
if [ ! -f "$PYTHON_SCRIPT" ]; then
    echo "[FAIL] ERROR FATAL: No se encuentra el archivo Python:"
    echo "   $PYTHON_SCRIPT"
    echo "   Asegúrate de copiar get_combined_env.py a la misma carpeta."
    read -p "Presiona Enter para continuar..."
    return 1
fi

# 2. Detectar comando python disponible
if command -v python3 &> /dev/null; then
    PYTHON_CMD="python3"
elif command -v python &> /dev/null; then
    PYTHON_CMD="python"
else
    echo "[FAIL] ERROR FATAL: Python no encontrado en el PATH."
    echo "   Instala Python o añádelo al PATH de Windows/MSYS2."
    read -p "Presiona Enter para continuar..."
    return 1
fi

echo "🔎 Generando variables de entorno..."

# 3. Ejecutar Python y capturar errores en log
TEMP_ENV_FILE=$(mktemp)
"$PYTHON_CMD" "$PYTHON_SCRIPT" > "$TEMP_ENV_FILE" 2> "$LOG_FILE"
RET_CODE=$?

if [ $RET_CODE -ne 0 ]; then
    echo "[FAIL] ERROR: El script de Python falló (Código $RET_CODE)."
    echo "--- LOG DE ERROR ($LOG_FILE) ---"
    cat "$LOG_FILE"
    echo "--------------------------------"
    rm -f "$TEMP_ENV_FILE"
    return 1
fi

# 4. Cargar el entorno generado
if [ -s "$TEMP_ENV_FILE" ]; then
    source "$TEMP_ENV_FILE"
else
    echo "[FAIL] ERROR: El script de Python no generó salida."
    cat "$LOG_FILE"
fi
rm -f "$TEMP_ENV_FILE"

# 5. Verificación final
echo "---------------------------------------------------------"
if command -v icx &> /dev/null; then
    echo "[OK] ÉXITO: Entorno cargado."
    echo "   Compilador: $(which icx)"
    # Check rápido de visual studio
    if [[ "$INCLUDE" == *"Visual Studio"* ]]; then
        echo "   MSVC Headers: DETECTADOS"
    else
        echo "[WARN]  ADVERTENCIA: Headers de MSVC no detectados en INCLUDE."
    fi
else
    echo "[FAIL] ERROR: 'icx' no está en el PATH."
fi
echo "---------------------------------------------------------"