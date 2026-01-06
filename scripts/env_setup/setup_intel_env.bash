#!/bin/bash

# =============================================================================
# SETUP INTEL ONEAPI + MSVC ENVIRONMENT (BASH)
# =============================================================================

# Detectar sistema operativo
OS_NAME=$(uname -s)

# -----------------------------------------------------------------------------
# LINUX / WSL SUPPORT
# -----------------------------------------------------------------------------
if [[ "$OS_NAME" == "Linux" ]]; then
    # En Linux, verificamos si icx/icpx ya está en el PATH o cargamos setvars.sh estándar
    if command -v icx &> /dev/null || command -v icpx &> /dev/null; then
        echo "[OK] Intel oneAPI detectado en PATH (Linux)." >&2
        # Usar return si es sourced, exit si es ejecutado, para no matar el shell padre si se usa && env
        return 0 2>/dev/null || exit 0
    fi
    
    if [ -f "/opt/intel/oneapi/setvars.sh" ]; then
        source "/opt/intel/oneapi/setvars.sh" > /dev/null 2>&1
        if command -v icx &> /dev/null || command -v icpx &> /dev/null; then
            echo "[OK] Entorno Intel oneAPI cargado (Linux)." >&2
            return 0 2>/dev/null || exit 0
        fi
    fi
    # Si fallamos en Linux, dejamos que el script continúe o falle, pero evitamos el script de Python de Windows si es posible
    echo "[WARN] No se detectó Intel oneAPI en Linux. Intentando fallback..." >&2
    return 1 2>/dev/null || exit 1
fi

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

# Advertencia si el script no fue "sourced"
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    echo "⚠️  ADVERTENCIA: Este script se ejecutó en un subshell."
    echo "   Para que los cambios persistan, usa: source ${0}"
fi