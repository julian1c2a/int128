#!/usr/bin/env python3
"""
Script para comparar variables de entorno antes y después de configurar un compilador
Uso: python compare_env.py <before|after|diff>
"""

import os
import sys
import json
from pathlib import Path

ENV_DIR = Path(__file__).parent.parent.parent / "build" / "env_snapshots"
ENV_DIR.mkdir(parents=True, exist_ok=True)

BEFORE_FILE = ENV_DIR / "env_before.json"
AFTER_FILE = ENV_DIR / "env_after.json"

def capture_env():
    """Captura el entorno actual"""
    return dict(os.environ)

def save_before():
    """Guarda el entorno ANTES de ejecutar el script de configuración"""
    env = capture_env()
    with open(BEFORE_FILE, 'w', encoding='utf-8') as f:
        json.dump(env, f, indent=2, ensure_ascii=False)
    print(f"✓ Entorno guardado en: {BEFORE_FILE}")
    print(f"  Total de variables: {len(env)}")

def save_after():
    """Guarda el entorno DESPUÉS de ejecutar el script de configuración"""
    env = capture_env()
    with open(AFTER_FILE, 'w', encoding='utf-8') as f:
        json.dump(env, f, indent=2, ensure_ascii=False)
    print(f"✓ Entorno guardado en: {AFTER_FILE}")
    print(f"  Total de variables: {len(env)}")

def compare_paths(before_path: str, after_path: str) -> dict:
    """Compara la variable PATH manteniendo el orden"""
    separator = ';' if os.name == 'nt' else ':'
    before_items = [p for p in before_path.split(separator) if p] if before_path else []
    after_items = [p for p in after_path.split(separator) if p] if after_path else []
    
    before_set = set(before_items)
    after_set = set(after_items)
    
    added = [p for p in after_items if p not in before_set]
    removed = [p for p in before_items if p not in after_set]
    common = len(before_set & after_set)
    
    return {
        'added': added,  # Mantiene el orden de aparición
        'removed': removed,
        'common': common,
        'before_items': before_items,
        'after_items': after_items
    }

def is_path_like(var_name: str) -> bool:
    """Detecta si una variable es tipo PATH"""
    path_vars = ['PATH', 'INCLUDE', 'LIB', 'LIBRARY_PATH', 'LD_LIBRARY_PATH', 
                 'CPATH', 'C_INCLUDE_PATH', 'CPLUS_INCLUDE_PATH', 'PKG_CONFIG_PATH']
    return var_name.upper() in path_vars

def is_compiler_related(var_name: str) -> bool:
    """Detecta si una variable está relacionada con compiladores"""
    compiler_prefixes = ['CC', 'CXX', 'FC', 'F77', 'F90', 'AR', 'AS', 'LD', 'NM', 
                         'RANLIB', 'STRIP', 'GCC', 'CLANG', 'MSVC', 'INTEL', 
                         'ONEAPI', 'VCINSTALLDIR', 'VSCMD', 'VS', 'WINDOWSSDKDIR']
    compiler_keywords = ['INCLUDE', 'LIB', 'PATH', 'FLAGS', 'OPTS', 'COMPILER', 
                        'TOOLCHAIN', 'MINGW', 'UCRT', 'MSYSTEM']
    
    var_upper = var_name.upper()
    
    # Verificar prefijos exactos
    for prefix in compiler_prefixes:
        if var_upper.startswith(prefix):
            return True
    
    # Verificar keywords en el nombre
    for keyword in compiler_keywords:
        if keyword in var_upper:
            return True
    
    return False

def is_noise_variable(var_name: str, value: str) -> bool:
    """Detecta variables que son 'ruido' (output de programas, etc.)"""
    # Variables conocidas que suelen tener ruido
    noise_vars = ['_', 'OLDPWD', 'PS1', 'PS2', 'PS3', 'PS4', 'PROMPT', 'RPROMPT']
    
    if var_name in noise_vars:
        return True
    
    # Si el valor contiene indicadores de output de programa
    noise_indicators = [
        'Program PHONON',
        'Error in routine',
        'stopping ...',
        '%%%%%%%%%%%%%%',
        'STOP 1',
        'starts on',
        'quantum-espresso'
    ]
    
    for indicator in noise_indicators:
        if indicator in value:
            return True
    
    # Si el valor es extremadamente largo (>2000 caracteres) y tiene muchas líneas
    if len(value) > 2000 and value.count('\n') > 10:
        return True
    
    return False

def format_path_diff(var_name: str, old_val: str, new_val: str) -> str:
    """Formatea la diferencia de una variable tipo PATH"""
    path_diff = compare_paths(old_val, new_val)
    
    output = [f"\n  {var_name}:"]
    
    if path_diff['added']:
        output.append(f"    ➕ Elementos añadidos ({len(path_diff['added'])}):")
        for i, p in enumerate(path_diff['added'], 1):
            output.append(f"       {i}. {p}")
    
    if path_diff['removed']:
        output.append(f"    ➖ Elementos eliminados ({len(path_diff['removed'])}):")
        for i, p in enumerate(path_diff['removed'], 1):
            output.append(f"       {i}. {p}")
    
    if path_diff['common'] > 0:
        output.append(f"    ↔️  Elementos sin cambios: {path_diff['common']}")
    
    return '\n'.join(output)

def show_diff():
    """Muestra las diferencias entre el entorno antes y después"""
    if not BEFORE_FILE.exists():
        print("❌ No existe snapshot del entorno ANTES")
        print(f"   Ejecuta primero: python {__file__} before")
        return
    
    if not AFTER_FILE.exists():
        print("❌ No existe snapshot del entorno DESPUÉS")
        print(f"   Ejecuta después del script de setup: python {__file__} after")
        return
    
    with open(BEFORE_FILE, 'r', encoding='utf-8') as f:
        before = json.load(f)
    
    with open(AFTER_FILE, 'r', encoding='utf-8') as f:
        after = json.load(f)
    
    # Variables añadidas (filtrar ruido)
    added_vars = {k for k in (set(after.keys()) - set(before.keys())) 
                  if not is_noise_variable(k, after[k])}
    
    # Variables eliminadas (filtrar ruido)
    removed_vars = {k for k in (set(before.keys()) - set(after.keys())) 
                    if not is_noise_variable(k, before[k])}
    
    # Variables modificadas (filtrar ruido)
    all_modified = {k for k in set(before.keys()) & set(after.keys()) 
                    if before[k] != after[k]}
    modified_vars = {k for k in all_modified 
                     if not is_noise_variable(k, after[k])}
    
    # Separar variables relacionadas con compiladores
    compiler_added = {k for k in added_vars if is_compiler_related(k)}
    compiler_modified = {k for k in modified_vars if is_compiler_related(k)}
    
    other_added = added_vars - compiler_added
    other_modified = modified_vars - compiler_modified
    
    # Contar ruido filtrado
    noise_filtered = len(all_modified) - len(modified_vars)
    
    print("\n" + "="*80)
    print("COMPARACIÓN DE ENTORNO - CAMBIOS DEL COMPILADOR")
    print("="*80)
    
    # === VARIABLES RELACIONADAS CON COMPILADORES ===
    if compiler_added or compiler_modified:
        print(f"\n🔧 CAMBIOS RELACIONADOS CON COMPILADORES:")
        print("=" * 80)
    
    # Variables de compilador añadidas
    if compiler_added:
        print(f"\n📌 Variables añadidas ({len(compiler_added)}):")
        print("-" * 80)
        for var in sorted(compiler_added):
            value = after[var]
            # Si es tipo PATH, formatear bonito
            if is_path_like(var):
                separator = ';' if os.name == 'nt' else ':'
                paths = [p for p in value.split(separator) if p]
                print(f"\n  {var}: ({len(paths)} elementos)")
                for i, p in enumerate(paths, 1):
                    print(f"     {i}. {p}")
            else:
                if len(value) > 120:
                    value = value[:120] + "..."
                print(f"  {var} = {value}")
    
    # Variables de compilador modificadas
    if compiler_modified:
        # Separar PATH y otras
        path_modified = {k for k in compiler_modified if is_path_like(k)}
        other_compiler_modified = compiler_modified - path_modified
        
        print(f"\n✏️  Variables modificadas ({len(compiler_modified)}):")
        print("-" * 80)
        
        # Mostrar PATH y variables similares primero
        if path_modified:
            print("\n  🔸 Variables tipo PATH:")
            print("  " + "-" * 78)
            for var in sorted(path_modified):
                old_val = before[var]
                new_val = after[var]
                print(format_path_diff(var, old_val, new_val))
        
        # Mostrar otras variables de compilador modificadas
        if other_compiler_modified:
            print("\n  🔸 Otras variables de compilador:")
            print("  " + "-" * 78)
            for var in sorted(other_compiler_modified):
                old_val = before[var]
                new_val = after[var]
                print(f"\n  {var}:")
                
                # Truncar valores muy largos
                if len(old_val) > 120:
                    old_val = old_val[:120] + "..."
                if len(new_val) > 120:
                    new_val = new_val[:120] + "..."
                
                print(f"    ANTES: {old_val}")
                print(f"    AHORA: {new_val}")
    
    # === OTROS CAMBIOS (NO RELACIONADOS CON COMPILADORES) ===
    if other_added or other_modified:
        print(f"\n\n📋 OTROS CAMBIOS (no relacionados con compiladores):")
        print("=" * 80)
    
    if other_added:
        print(f"\n📌 Otras variables añadidas ({len(other_added)}):")
        print("-" * 80)
        for var in sorted(other_added):
            value = after[var]
            if len(value) > 120:
                value = value[:120] + "..."
            print(f"  {var} = {value}")
    
    if other_modified:
        print(f"\n✏️  Otras variables modificadas ({len(other_modified)}):")
        print("-" * 80)
        for var in sorted(other_modified):
            old_val = before[var]
            new_val = after[var]
            print(f"\n  {var}:")
            
            if len(old_val) > 120:
                old_val = old_val[:120] + "..."
            if len(new_val) > 120:
                new_val = new_val[:120] + "..."
            
            print(f"    ANTES: {old_val}")
            print(f"    AHORA: {new_val}")
    
    # Variables eliminadas
    if removed_vars:
        print(f"\n🗑️  VARIABLES ELIMINADAS ({len(removed_vars)}):")
        print("-" * 80)
        for var in sorted(removed_vars):
            value = before[var]
            if len(value) > 120:
                value = value[:120] + "..."
            print(f"  {var} = {value}")
    
    # Resumen
    print("\n" + "="*80)
    print("RESUMEN:")
    print(f"  🔧 Variables de compilador añadidas:       {len(compiler_added)}")
    print(f"  🔧 Variables de compilador modificadas:    {len(compiler_modified)}")
    print(f"  📋 Otras variables añadidas:               {len(other_added)}")
    print(f"  📋 Otras variables modificadas:            {len(other_modified)}")
    print(f"  🗑️  Variables eliminadas:                   {len(removed_vars)}")
    if noise_filtered > 0:
        print(f"  🔇 Variables con ruido filtradas:          {noise_filtered}")
    print(f"  ✓  Variables sin cambios:                  {len(set(before.keys()) & set(after.keys())) - len(all_modified)}")
    print("="*80 + "\n")

def show_usage():
    """Muestra cómo usar el script"""
    print(f"""
Uso: python {Path(__file__).name} <comando>

Comandos:
  before   - Guarda el entorno actual ANTES de configurar el compilador
  after    - Guarda el entorno actual DESPUÉS de configurar el compilador
  diff     - Muestra las diferencias entre ambos entornos

Ejemplo de uso:
  1. Capturar entorno inicial:
     python {Path(__file__).name} before
  
  2. Ejecutar tu script de configuración del compilador:
     source scripts/env_setup/setup_gcc_env.bash
  
  3. Capturar entorno modificado:
     python {Path(__file__).name} after
  
  4. Ver los cambios:
     python {Path(__file__).name} diff
""")

def main():
    if len(sys.argv) < 2:
        show_usage()
        sys.exit(1)
    
    command = sys.argv[1].lower()
    
    if command == 'before':
        save_before()
    elif command == 'after':
        save_after()
    elif command == 'diff':
        show_diff()
    else:
        print(f"❌ Comando desconocido: {command}")
        show_usage()
        sys.exit(1)

if __name__ == "__main__":
    main()
