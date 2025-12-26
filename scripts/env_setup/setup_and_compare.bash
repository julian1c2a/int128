#!/usr/bin/env bash
# Script para configurar un compilador y ver los cambios en el entorno
# Uso: source setup_and_compare.bash <gcc|clang|msvc|intel>

if [ $# -eq 0 ]; then
    echo "Uso: source setup_and_compare.bash <gcc|clang|msvc|intel>"
    echo ""
    echo "Ejemplo:"
    echo "  source setup_and_compare.bash gcc"
    return 1 2>/dev/null || exit 1
fi

COMPILER="$1"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Verificar que existe el script de setup
SETUP_SCRIPT="${SCRIPT_DIR}/setup_${COMPILER}_env.bash"
if [ ! -f "$SETUP_SCRIPT" ]; then
    echo "❌ No existe el script: $SETUP_SCRIPT"
    return 1 2>/dev/null || exit 1
fi

echo "════════════════════════════════════════════════════════════════"
echo "  Configuración del compilador: $COMPILER"
echo "════════════════════════════════════════════════════════════════"
echo ""

# 1. Guardar entorno ANTES
echo "📸 Capturando entorno inicial..."
python "$SCRIPT_DIR/compare_env.py" before
echo ""

# 2. Ejecutar el script de configuración
echo "⚙️  Configurando $COMPILER..."
source "$SETUP_SCRIPT"
SETUP_STATUS=$?
echo ""

if [ $SETUP_STATUS -ne 0 ]; then
    echo "❌ Error al configurar el compilador"
    return $SETUP_STATUS 2>/dev/null || exit $SETUP_STATUS
fi

# 3. Guardar entorno DESPUÉS
echo "📸 Capturando entorno modificado..."
python "$SCRIPT_DIR/compare_env.py" after
echo ""

# 4. Mostrar diferencias
echo "🔍 Analizando cambios..."
echo ""
python "$SCRIPT_DIR/compare_env.py" diff

echo ""
echo "════════════════════════════════════════════════════════════════"
echo "✅ Configuración completada"
echo "════════════════════════════════════════════════════════════════"
