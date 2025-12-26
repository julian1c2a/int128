#!/usr/bin/env bash
# Script para renombrar archivos cl corruptos y permitir reinstalar MSVC

MSVC_DIR="/c/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.50.35717/bin"

echo "================================================================================"
echo "⚠️  RENOMBRAR ARCHIVOS CL CORRUPTOS (PHONON)"
echo "================================================================================"
echo ""
echo "Este script renombrará los archivos cl y cl.exe corruptos para que puedas"
echo "reinstalar o reparar MSVC sin conflictos."
echo ""
echo "Archivos a renombrar en: $MSVC_DIR"
echo ""
read -p "¿Continuar? (s/n): " confirm

if [[ ! "$confirm" =~ ^[sS]$ ]]; then
    echo "Operación cancelada."
    exit 0
fi

renamed_count=0

# Buscar en subdirectorios
for subdir in "Hostx64/x64" "HostX64/x64" "Hostx86/x86" "HostX86/x86"; do
    full_dir="$MSVC_DIR/$subdir"
    
    if [ -d "$full_dir" ]; then
        echo ""
        echo "Procesando: $full_dir"
        echo "--------------------------------------------------------------------------------"
        
        cd "$full_dir" || continue
        
        # Renombrar cl (sin extensión)
        if [ -f "cl" ]; then
            if timeout 1 ./cl 2>&1 | grep -q "PHONON"; then
                echo "  ⚠️  Renombrando: cl → cl.phonon.backup"
                mv cl cl.phonon.backup
                renamed_count=$((renamed_count + 1))
            else
                echo "  ✅ cl parece correcto (no se renombra)"
            fi
        fi
        
        # Renombrar cl.exe
        if [ -f "cl.exe" ]; then
            if timeout 1 ./cl.exe 2>&1 | grep -q "PHONON"; then
                echo "  ⚠️  Renombrando: cl.exe → cl.exe.phonon.backup"
                mv cl.exe cl.exe.phonon.backup
                renamed_count=$((renamed_count + 1))
            else
                echo "  ✅ cl.exe parece correcto (no se renombra)"
            fi
        fi
    fi
done

echo ""
echo "================================================================================"
echo "✅ Completado"
echo "================================================================================"
echo "  Archivos renombrados: $renamed_count"
echo ""
echo "SIGUIENTE PASO:"
echo "  1. Abre Visual Studio Installer"
echo "  2. Click en 'Más' → 'Reparar'"
echo "  3. O reinstala los componentes de C++"
echo ""
echo "================================================================================"
