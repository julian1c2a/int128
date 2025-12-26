#!/usr/bin/env bash
# Script para verificar y reparar el cl.exe corrupto

echo "================================================================================"
echo "🔧 DIAGNÓSTICO Y REPARACIÓN DE cl.exe CORRUPTO"
echo "================================================================================"

MSVC_DIR="/c/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.50.35717/bin/Hostx64/x64"

# Verificar si el directorio existe
if [ ! -d "$MSVC_DIR" ]; then
    echo "❌ El directorio de MSVC no existe: $MSVC_DIR"
    exit 1
fi

echo ""
echo "📂 Directorio de MSVC: $MSVC_DIR"
echo "--------------------------------------------------------------------------------"

cd "$MSVC_DIR" || exit 1

# Listar archivos cl*
echo ""
echo "📋 Archivos 'cl*' en el directorio:"
ls -lh cl* 2>/dev/null || echo "   ⚠️  No se encontraron archivos cl*"

echo ""
echo "🔍 Verificando contenido de archivos..."
echo "--------------------------------------------------------------------------------"

# Verificar cada archivo cl
for file in cl cl.exe; do
    if [ -f "$file" ]; then
        echo ""
        echo "📄 Archivo: $file"
        
        # Obtener información
        size=$(stat -c%s "$file" 2>/dev/null || stat -f%z "$file" 2>/dev/null)
        echo "   Tamaño: $size bytes"
        
        # Intentar identificar el tipo con 'file'
        file_type=$(file "$file" 2>/dev/null)
        echo "   Tipo: $file_type"
        
        # Buscar strings característicos
        echo "   Buscando strings característicos..."
        
        if strings "$file" | grep -q "PHONON"; then
            echo "   ⚠️  ENCONTRADO: Este es Quantum ESPRESSO PHONON"
        fi
        
        if strings "$file" | grep -q "Microsoft"; then
            echo "   ✅ ENCONTRADO: Contiene referencias a Microsoft"
        fi
        
        if strings "$file" | grep -q "Optimizing Compiler"; then
            echo "   ✅ ENCONTRADO: Es el compilador de Microsoft"
        fi
    else
        echo ""
        echo "📄 Archivo: $file - NO EXISTE"
    fi
done

echo ""
echo "================================================================================"
echo "💡 SOLUCIONES"
echo "================================================================================"

# Verificar si existe backup
if [ -f "cl.exe.backup" ] || [ -f "cl.backup" ]; then
    echo ""
    echo "✅ Se encontraron backups. Puedes intentar restaurar:"
    echo "   cd '$MSVC_DIR'"
    echo "   cp cl.exe.backup cl.exe"
else
    echo ""
    echo "⚠️  No se encontraron backups."
fi

echo ""
echo "📝 Opciones para solucionar:"
echo ""
echo "1. 🔄 REINSTALAR MSVC (Recomendado):"
echo "   - Abre Visual Studio Installer"
echo "   - Modifica la instalación"
echo "   - Repara los componentes de C++ Build Tools"
echo ""
echo "2. 💾 HACER BACKUP Y RENOMBRAR (Temporal):"
echo "   cd '$MSVC_DIR'"
echo "   mv cl.exe phonon_qe.exe  # Renombrar el archivo corrupto"
echo "   # Luego reinstalar MSVC"
echo ""
echo "3. 🔍 BUSCAR cl.exe CORRECTO EN OTROS DIRECTORIOS:"
echo "   # Verificar si hay otras versiones de MSVC instaladas"
echo "   find '/c/Program Files/Microsoft Visual Studio' -name 'cl.exe' -type f 2>/dev/null"
echo ""
echo "4. 🎯 USAR OTRA INSTALACIÓN DE MSVC:"
echo "   # Si tienes Visual Studio 2022 en otra ubicación"
echo ""

# Buscar otras instalaciones de cl.exe
echo "================================================================================"
echo "🔍 BUSCANDO OTRAS INSTALACIONES DE cl.exe..."
echo "================================================================================"

echo ""
echo "Esto puede tomar unos momentos..."
other_cl=$(find "/c/Program Files" -path "*VC/Tools/MSVC*/bin/*/x64/cl.exe" -type f 2>/dev/null | head -n 5)

if [ -n "$other_cl" ]; then
    echo ""
    echo "✅ Encontradas otras instalaciones:"
    echo "$other_cl" | while read -r path; do
        echo ""
        echo "   📍 $path"
        
        # Verificar si es el correcto
        if timeout 1 "$path" 2>&1 | grep -q "Microsoft"; then
            echo "      ✅ Este parece ser el compilador correcto de Microsoft"
        elif timeout 1 "$path" 2>&1 | grep -q "PHONON"; then
            echo "      ⚠️  Este también está corrupto con PHONON"
        fi
    done
else
    echo ""
    echo "❌ No se encontraron otras instalaciones de cl.exe"
fi

echo ""
echo "================================================================================"
echo "✅ Diagnóstico completado"
echo "================================================================================"
