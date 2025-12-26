#!/usr/bin/env bash
# Script para buscar aliases, funciones y scripts que intercepten comandos

echo "================================================================================"
echo "🔍 BÚSQUEDA DE ALIASES, FUNCIONES Y WRAPPERS"
echo "================================================================================"

# 1. Buscar aliases
echo ""
echo "📌 1. ALIASES (alias)"
echo "--------------------------------------------------------------------------------"
alias | grep -E 'cl|phonon|qe' || echo "   ✅ No se encontraron aliases relevantes"

# 2. Buscar funciones
echo ""
echo "📌 2. FUNCIONES DE BASH (declare -F)"
echo "--------------------------------------------------------------------------------"
declare -F | grep -E 'cl|phonon|qe' || echo "   ✅ No se encontraron funciones relevantes"

# 3. Ver si 'cl' es una función
echo ""
echo "📌 3. VERIFICAR SI 'cl' ES UNA FUNCIÓN"
echo "--------------------------------------------------------------------------------"
if type cl &>/dev/null; then
    echo "   ⚠️  'cl' está definido:"
    type cl
else
    echo "   ✅ 'cl' no está definido como función/alias"
fi

# 4. Ver si 'cl.exe' es una función
echo ""
echo "📌 4. VERIFICAR SI 'cl.exe' ES UNA FUNCIÓN"
echo "--------------------------------------------------------------------------------"
if type cl.exe &>/dev/null; then
    echo "   ⚠️  'cl.exe' está definido:"
    type cl.exe
else
    echo "   ✅ 'cl.exe' no está definido como función/alias"
fi

# 5. Buscar scripts wrapper en directorios comunes
echo ""
echo "📌 5. BUSCAR SCRIPTS WRAPPER"
echo "--------------------------------------------------------------------------------"
WRAPPER_DIRS=(
    "$HOME/bin"
    "$HOME/.local/bin"
    "/usr/local/bin"
    "/usr/bin"
    "/bin"
)

for dir in "${WRAPPER_DIRS[@]}"; do
    if [ -d "$dir" ]; then
        wrapper_files=$(find "$dir" -maxdepth 1 -name "cl" -o -name "cl.exe" 2>/dev/null)
        if [ -n "$wrapper_files" ]; then
            echo "   ⚠️  Encontrado en $dir:"
            echo "$wrapper_files" | while read -r file; do
                echo "      • $file"
                if [ -f "$file" ]; then
                    file "$file"
                fi
            done
        fi
    fi
done

# 6. Probar ejecución directa vs ejecución con tipo
echo ""
echo "📌 6. DIFERENCIA ENTRE EJECUCIONES"
echo "--------------------------------------------------------------------------------"
echo "   Probando 'cl.exe' (puede interceptar alias/función):"
echo "   ────────────────────────────────────────────────────"
timeout 2 cl.exe 2>&1 | head -n 3 || echo "   Timeout o error"

echo ""
echo "   Probando 'command cl.exe' (bypass de alias/función):"
echo "   ────────────────────────────────────────────────────"
timeout 2 command cl.exe 2>&1 | head -n 3 || echo "   Timeout o error"

echo ""
echo "   Probando con ruta completa:"
echo "   ────────────────────────────────────────────────────"
MSVC_CL="/c/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.50.35717/bin/Hostx64/x64/cl.exe"
if [ -f "$MSVC_CL" ]; then
    timeout 2 "$MSVC_CL" 2>&1 | head -n 3 || echo "   Timeout o error"
else
    echo "   ⚠️  Archivo no encontrado en: $MSVC_CL"
    # Probar con case diferente
    MSVC_CL_ALT="/c/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.50.35717/bin/HostX64/x64/cl.exe"
    if [ -f "$MSVC_CL_ALT" ]; then
        echo "   ✅ Encontrado con case alternativo"
        timeout 2 "$MSVC_CL_ALT" 2>&1 | head -n 3 || echo "   Timeout o error"
    fi
fi

# 7. Buscar en archivos de configuración
echo ""
echo "📌 7. BUSCAR EN ARCHIVOS DE CONFIGURACIÓN"
echo "--------------------------------------------------------------------------------"
CONFIG_FILES=(
    "$HOME/.bashrc"
    "$HOME/.bash_profile"
    "$HOME/.bash_aliases"
    "$HOME/.profile"
)

for config in "${CONFIG_FILES[@]}"; do
    if [ -f "$config" ]; then
        matches=$(grep -n -E '(^|[[:space:]])cl[[:space:]]|function cl|alias cl' "$config" 2>/dev/null)
        if [ -n "$matches" ]; then
            echo "   ⚠️  Encontrado en $config:"
            echo "$matches"
        fi
    fi
done

echo ""
echo "================================================================================"
echo "✅ Búsqueda completada"
echo "================================================================================"
