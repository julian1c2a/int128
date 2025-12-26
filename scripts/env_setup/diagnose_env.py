#!/usr/bin/env python3
"""
Script de diagnóstico para encontrar de dónde viene información extraña en las variables
Uso: python diagnose_env.py [texto_a_buscar]
"""

import os
import sys
import json
from pathlib import Path

def search_in_env(search_text: str = None):
    """Busca un texto específico en todas las variables de entorno"""
    env = dict(os.environ)
    
    if search_text:
        print(f"\n🔍 Buscando '{search_text}' en variables de entorno...")
        print("="*80)
    else:
        print("\n📊 Analizando todas las variables de entorno...")
        print("="*80)
    
    suspicious_vars = []
    
    for var_name, var_value in sorted(env.items()):
        # Si se especifica texto, buscar solo eso
        if search_text:
            if search_text.lower() in var_value.lower():
                suspicious_vars.append((var_name, var_value))
        else:
            # Sin texto específico, buscar patrones sospechosos
            suspicious_patterns = [
                'Program PHONON',
                'quantum-espresso',
                'Error in routine',
                'stopping ...',
                'STOP 1',
                '%%%%%%%%%%%%%%',
                'starts on'
            ]
            
            for pattern in suspicious_patterns:
                if pattern in var_value:
                    suspicious_vars.append((var_name, var_value))
                    break
    
    if not suspicious_vars:
        print("✅ No se encontraron coincidencias")
        return
    
    print(f"\n⚠️  Encontradas {len(suspicious_vars)} variable(s) sospechosa(s):\n")
    
    for var_name, var_value in suspicious_vars:
        print("="*80)
        print(f"📌 Variable: {var_name}")
        print("-"*80)
        print(f"Tipo: {type(var_value).__name__}")
        print(f"Longitud: {len(var_value)} caracteres")
        print(f"Líneas: {var_value.count(chr(10)) + 1}")
        print("-"*80)
        
        # Mostrar primeras líneas
        lines = var_value.split('\n')
        preview_lines = min(20, len(lines))
        
        print(f"Contenido (primeras {preview_lines} líneas):")
        for i, line in enumerate(lines[:preview_lines], 1):
            print(f"  {i:3d}: {line[:100]}")
        
        if len(lines) > preview_lines:
            print(f"  ... y {len(lines) - preview_lines} líneas más")
        
        print()

def analyze_large_vars():
    """Encuentra variables anormalmente grandes"""
    env = dict(os.environ)
    
    print("\n📏 Analizando variables por tamaño...")
    print("="*80)
    
    var_sizes = [(name, len(value)) for name, value in env.items()]
    var_sizes.sort(key=lambda x: x[1], reverse=True)
    
    print(f"\n🔝 Top 15 variables más grandes:\n")
    for i, (name, size) in enumerate(var_sizes[:15], 1):
        lines = env[name].count('\n') + 1
        print(f"  {i:2d}. {name:30s} - {size:6d} chars, {lines:4d} líneas")
    
    # Variables sospechosamente grandes
    suspicious_size = 1000  # Más de 1KB es sospechoso
    large_vars = [(name, size) for name, size in var_sizes if size > suspicious_size]
    
    if large_vars:
        print(f"\n⚠️  Variables sospechosamente grandes (>{suspicious_size} chars): {len(large_vars)}")

def check_snapshots():
    """Verifica si los snapshots guardados tienen el problema"""
    env_dir = Path(__file__).parent.parent.parent / "build" / "env_snapshots"
    before_file = env_dir / "env_before.json"
    after_file = env_dir / "env_after.json"
    
    print("\n📸 Verificando snapshots guardados...")
    print("="*80)
    
    for snapshot_file in [before_file, after_file]:
        if not snapshot_file.exists():
            print(f"⚠️  {snapshot_file.name}: No existe")
            continue
        
        with open(snapshot_file, 'r', encoding='utf-8') as f:
            data = json.load(f)
        
        print(f"\n📄 {snapshot_file.name}:")
        print(f"   Total de variables: {len(data)}")
        
        # Buscar variables con Quantum ESPRESSO
        qe_vars = [name for name, value in data.items() if 'PHONON' in str(value) or 'quantum-espresso' in str(value)]
        
        if qe_vars:
            print(f"   ⚠️  Variables con Quantum ESPRESSO: {len(qe_vars)}")
            for var in qe_vars:
                print(f"      - {var}")
        else:
            print(f"   ✅ No se encontró Quantum ESPRESSO")

def show_usage():
    """Muestra cómo usar el script"""
    print(f"""
Uso: python {Path(__file__).name} [comando] [texto]

Comandos:
  search <texto>  - Busca un texto específico en todas las variables
  large           - Muestra las variables más grandes
  snapshots       - Verifica los snapshots guardados
  all             - Ejecuta todos los análisis (por defecto)

Ejemplos:
  python {Path(__file__).name} search PHONON
  python {Path(__file__).name} large
  python {Path(__file__).name} snapshots
  python {Path(__file__).name}           # Ejecuta todos los análisis
""")

def main():
    if len(sys.argv) > 1:
        command = sys.argv[1].lower()
        
        if command == 'help' or command == '--help' or command == '-h':
            show_usage()
            return
        
        if command == 'search':
            if len(sys.argv) < 3:
                print("❌ Debes especificar un texto a buscar")
                print("   Ejemplo: python diagnose_env.py search PHONON")
                sys.exit(1)
            search_text = ' '.join(sys.argv[2:])
            search_in_env(search_text)
        
        elif command == 'large':
            analyze_large_vars()
        
        elif command == 'snapshots':
            check_snapshots()
        
        elif command == 'all':
            search_in_env()  # Buscar patrones sospechosos
            analyze_large_vars()
            check_snapshots()
        
        else:
            print(f"❌ Comando desconocido: {command}")
            show_usage()
            sys.exit(1)
    else:
        # Por defecto, ejecutar todos los análisis
        search_in_env()  # Buscar patrones sospechosos
        analyze_large_vars()
        check_snapshots()

if __name__ == "__main__":
    main()
