#!/usr/bin/env python3
"""
Encuentra todos los cl.exe en el PATH y determina cuál es el correcto
"""

import os
import subprocess
import sys
from pathlib import Path

def find_all_in_path(executable_name: str):
    """Encuentra todas las instancias de un ejecutable en PATH"""
    print(f"\n🔍 Buscando todas las instancias de '{executable_name}' en PATH...")
    print("="*80)
    
    # Obtener PATH
    path_var = os.environ.get('PATH', '')
    separator = ';' if os.name == 'nt' else ':'
    paths = [p for p in path_var.split(separator) if p]
    
    found = []
    
    for path_dir in paths:
        # Buscar con .exe y sin .exe
        for ext in ['', '.exe']:
            full_path = Path(path_dir) / (executable_name + ext)
            if full_path.exists() and full_path.is_file():
                found.append(full_path)
    
    if not found:
        print(f"❌ No se encontró '{executable_name}' en PATH")
        return []
    
    print(f"✅ Encontradas {len(found)} instancia(s):\n")
    
    for i, exe_path in enumerate(found, 1):
        print(f"{i}. {exe_path}")
        
        # Obtener información del archivo
        try:
            size = exe_path.stat().st_size
            print(f"   Tamaño: {size:,} bytes")
            
            # Intentar ejecutarlo con --version o sin argumentos
            try:
                result = subprocess.run(
                    [str(exe_path)],
                    capture_output=True,
                    text=True,
                    timeout=2,
                    stdin=subprocess.DEVNULL
                )
                
                output = (result.stdout + result.stderr)[:200]
                
                # Detectar tipo
                if 'PHONON' in output or 'quantum-espresso' in output:
                    print(f"   ⚠️  TIPO: Quantum ESPRESSO PHONON")
                elif 'Microsoft' in output or 'C/C++' in output:
                    print(f"   ✅ TIPO: Microsoft Visual C++ Compiler")
                elif 'Optimizing Compiler' in output:
                    print(f"   ✅ TIPO: Microsoft C/C++ Optimizing Compiler")
                else:
                    print(f"   ❓ TIPO: Desconocido")
                    print(f"   Output: {output[:100]}")
                
            except subprocess.TimeoutExpired:
                print(f"   ⏱️  Timeout al ejecutar")
            except Exception as e:
                print(f"   ⚠️  Error al ejecutar: {e}")
                
        except Exception as e:
            print(f"   ❌ Error: {e}")
        
        print()
    
    return found

def test_which_cl():
    """Prueba qué cl.exe ejecuta 'which cl.exe'"""
    print("\n🔍 Probando 'which cl.exe'...")
    print("="*80)
    
    try:
        result = subprocess.run(
            ['which', 'cl.exe'],
            capture_output=True,
            text=True,
            timeout=5
        )
        
        if result.returncode == 0:
            cl_path = result.stdout.strip()
            print(f"✅ which encuentra: {cl_path}")
            
            # Verificar si existe
            if Path(cl_path).exists():
                print(f"   Archivo existe: ✅")
                
                # Intentar ejecutarlo
                try:
                    test_result = subprocess.run(
                        [cl_path],
                        capture_output=True,
                        text=True,
                        timeout=2,
                        stdin=subprocess.DEVNULL
                    )
                    
                    output = test_result.stdout + test_result.stderr
                    
                    if 'PHONON' in output:
                        print(f"   ⚠️  ¡PROBLEMA! Este es Quantum ESPRESSO, no MSVC")
                    elif 'Microsoft' in output:
                        print(f"   ✅ Este es el compilador MSVC correcto")
                    else:
                        print(f"   ❓ No está claro qué es")
                        print(f"   Primeras líneas: {output[:150]}")
                        
                except Exception as e:
                    print(f"   ⚠️  Error al ejecutar: {e}")
            else:
                print(f"   ⚠️  El archivo no existe")
        else:
            print(f"❌ which no encontró cl.exe")
            
    except Exception as e:
        print(f"❌ Error: {e}")

def suggest_fix():
    """Sugiere cómo arreglar el problema"""
    print("\n" + "="*80)
    print("💡 SOLUCIONES SUGERIDAS")
    print("="*80)
    print("""
1. Renombrar el ejecutable de Quantum ESPRESSO:
   - Encuentra el directorio de Quantum ESPRESSO
   - Renombra 'cl.exe' a 'phonon.exe' o 'qe_cl.exe'

2. Ajustar el PATH:
   - Asegúrate de que el directorio de MSVC aparezca ANTES del de Quantum ESPRESSO
   - Edita tu .bashrc o variables de entorno

3. Usar ruta completa en los scripts:
   - En lugar de usar 'cl.exe', usa la ruta completa al compilador MSVC
   - Ejemplo: /c/Program Files/.../cl.exe

4. Crear un alias temporal:
   - alias cl='/c/Program\\ Files/.../cl.exe'
""")

def main():
    print("\n" + "="*80)
    print("🔍 DIAGNÓSTICO DE CONFLICTO: cl.exe")
    print("="*80)
    
    # Buscar todos los cl.exe
    all_cl = find_all_in_path('cl')
    
    # Probar which
    test_which_cl()
    
    # Sugerir soluciones
    if any('PHONON' in str(exe) for exe in all_cl):
        suggest_fix()
    
    print("\n" + "="*80)
    print("✅ Diagnóstico completado")
    print("="*80 + "\n")

if __name__ == "__main__":
    main()
