#!/usr/bin/env bash
# Buscar todos los archivos ejecutables 'cl' en el PATH

echo "================================================================================"
echo "🔍 BUSCANDO TODOS LOS EJECUTABLES 'cl' EN PATH"
echo "================================================================================"

IFS=':' read -ra PATH_DIRS <<< "$PATH"

echo ""
echo "Buscando en ${#PATH_DIRS[@]} directorios del PATH..."
echo "Extensiones ejecutables: .exe, .bat, .cmd, .sh, .com, (sin extensión)"
echo ""

found_count=0

# Extensiones ejecutables comunes en Windows/MSYS2
EXEC_EXTS=("" ".exe" ".bat" ".cmd" ".sh" ".com" ".py")

for dir in "${PATH_DIRS[@]}"; do
    if [ -d "$dir" ]; then
        # Buscar archivos 'cl' con diferentes extensiones
        for ext in "${EXEC_EXTS[@]}"; do
            filename="cl${ext}"
            filepath="$dir/$filename"
            
            if [ -f "$filepath" ] && [ -x "$filepath" ]; then
                found_count=$((found_count + 1))
                echo "================================================================================"
                echo "📍 ENCONTRADO #$found_count"
                echo "================================================================================"
                echo "Archivo: $filepath"
                echo ""
                
                # Información del archivo
                ls -lh "$filepath"
                echo ""
                
                # Ver tipo de archivo
                echo "Tipo de archivo:"
                file "$filepath" 2>/dev/null || echo "  No se pudo determinar"
                echo ""
                
                # Para archivos de texto (bat, cmd, sh, py), mostrar contenido
                case "$ext" in
                    .bat|.cmd|.sh|.py)
                        echo "Contenido (primeras 10 líneas):"
                        head -n 10 "$filepath" | sed 's/^/  /'
                        echo ""
                        ;;
                    *)
                        # Para binarios, buscar strings característicos
                        echo "Contenido (strings):"
                        if strings "$filepath" 2>/dev/null | grep -q "PHONON"; then
                            echo "  ⚠️  CONTIENE: PHONON (Quantum ESPRESSO)"
                        fi
                        if strings "$filepath" 2>/dev/null | grep -q "Microsoft"; then
                            echo "  ✅ CONTIENE: Microsoft"
                        fi
                        if strings "$filepath" 2>/dev/null | grep -q "Fortran"; then
                            echo "  ⚠️  CONTIENE: Fortran"
                        fi
                        echo ""
                        ;;
                esac
                
                # Intentar ejecutarlo brevemente
                echo "Probando ejecución:"
                output=$(timeout 1 "$filepath" 2>&1 | head -n 5)
                if echo "$output" | grep -q "PHONON"; then
                    echo "  ⚠️  EJECUTA: Quantum ESPRESSO PHONON"
                    echo "  Primeras líneas:"
                    echo "$output" | head -n 2 | sed 's/^/    /'
                elif echo "$output" | grep -q -i "microsoft\|compilador"; then
                    echo "  ✅ EJECUTA: Microsoft Compiler"
                    echo "  Primeras líneas:"
                    echo "$output" | head -n 2 | sed 's/^/    /'
                else
                    echo "  ❓ No identificado claramente"
                    if [ -n "$output" ]; then
                        echo "  Primeras líneas:"
                        echo "$output" | head -n 2 | sed 's/^/    /'
                    fi
                fi
                echo ""
            fi
        done
    fi
done

if [ $found_count -eq 0 ]; then
    echo "✅ No se encontraron archivos 'cl' sin extensión en PATH"
else
    echo "================================================================================"
    echo "⚠️  TOTAL ENCONTRADO: $found_count archivo(s) 'cl' sin extensión"
    echo "================================================================================"
fi

echo ""
echo "================================================================================"
echo "🔍 BUSCANDO TAMBIÉN EN UBICACIONES COMUNES DE QUANTUM ESPRESSO"
echo "================================================================================"

QE_COMMON_PATHS=(
    "$HOME/qe"
    "$HOME/quantum-espresso"
    "$HOME/.local/bin"
    "/opt/qe"
    "/opt/quantum-espresso"
    "/usr/local/qe"
    "/usr/local/quantum-espresso"
    "C:/Program Files/Quantum ESPRESSO"
    "C:/qe"
    "D:/qe"
    "/c/qe"
    "/d/qe"
)

for qe_path in "${QE_COMMON_PATHS[@]}"; do
    if [ -d "$qe_path" ]; then
        echo ""
        echo "📂 Buscando en: $qe_path"
        find "$qe_path" -name "cl" -o -name "cl.x" -o -name "ph.x" 2>/dev/null | while read -r file; do
            echo "  ⚠️  Encontrado: $file"
        done
    fi
done

echo ""
echo "================================================================================"
echo "✅ Búsqueda completada"
echo "================================================================================"
