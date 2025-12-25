#!/usr/bin/env python3
"""
Script para detectar y configurar el entorno de Intel oneAPI.
Similar a vcvars.py pero para Intel oneAPI.

Uso:
    python scripts/setvars_intel.py
    
El script encuentra y retorna la ruta de setvars.bat de Intel oneAPI.
"""

import os
import sys
from pathlib import Path

# Rutas estándar donde suele estar Intel oneAPI
INTEL_ONEAPI_PATHS = [
    r"C:\Program Files (x86)\Intel\oneAPI",
    r"C:\Program Files\Intel\oneAPI",
    r"D:\Program Files (x86)\Intel\oneAPI",
    r"D:\Program Files\Intel\oneAPI",
]

def find_intel_oneapi_root():
    """
    Busca el directorio raíz de Intel oneAPI.
    Retorna la ruta si existe, None en caso contrario.
    """
    # Primero intentar la variable de entorno si ya está configurada
    if "ONEAPI_ROOT" in os.environ:
        root = os.environ["ONEAPI_ROOT"]
        if os.path.exists(root):
            return root
    
    # Buscar en las rutas estándar
    for path in INTEL_ONEAPI_PATHS:
        if os.path.exists(path):
            return path
    
    return None

def find_setvars_bat():
    """
    Encuentra la ruta completa de setvars.bat de Intel oneAPI.
    Retorna la ruta si existe, None en caso contrario.
    """
    root = find_intel_oneapi_root()
    if not root:
        return None
    
    setvars_path = os.path.join(root, "setvars.bat")
    if os.path.exists(setvars_path):
        return setvars_path
    
    return None

def find_setvars_vcvarsall_bat():
    """
    Encuentra la ruta completa de setvars-vcvarsall.bat de Intel oneAPI.
    Este script configura tanto Intel como MSVC.
    Retorna la ruta si existe, None en caso contrario.
    """
    root = find_intel_oneapi_root()
    if not root:
        return None
    
    setvars_vcvarsall_path = os.path.join(root, "setvars-vcvarsall.bat")
    if os.path.exists(setvars_vcvarsall_path):
        return setvars_vcvarsall_path
    
    return None

def get_compiler_paths():
    """
    Retorna un diccionario con las rutas de los compiladores Intel.
    """
    root = find_intel_oneapi_root()
    if not root:
        return None
    
    compiler_latest = os.path.join(root, "compiler", "latest")
    
    paths = {
        "root": root,
        "compiler": compiler_latest,
        "bin": os.path.join(compiler_latest, "bin"),
        "bin_intel64": os.path.join(compiler_latest, "bin", "intel64"),
        "icpx": os.path.join(compiler_latest, "bin", "icpx.exe"),
        "icx": os.path.join(compiler_latest, "bin", "icx.exe"),
    }
    
    # Verificar que al menos los binarios existan
    if os.path.exists(paths["icpx"]) or os.path.exists(paths["icx"]):
        return paths
    
    return None

if __name__ == "__main__":
    import json
    
    # Si se ejecuta directamente, mostrar información de detección
    print("Intel oneAPI Detection Tool")
    print("=" * 50)
    
    root = find_intel_oneapi_root()
    if root:
        print(f"✓ Intel oneAPI Root: {root}")
    else:
        print("✗ Intel oneAPI Root: NOT FOUND")
        sys.exit(1)
    
    setvars = find_setvars_bat()
    if setvars:
        print(f"✓ setvars.bat: {setvars}")
    else:
        print("✗ setvars.bat: NOT FOUND")
    
    setvars_vcvarsall = find_setvars_vcvarsall_bat()
    if setvars_vcvarsall:
        print(f"✓ setvars-vcvarsall.bat: {setvars_vcvarsall}")
    else:
        print("✗ setvars-vcvarsall.bat: NOT FOUND")
    
    paths = get_compiler_paths()
    if paths:
        print(f"\n✓ Compiler paths:")
        print(json.dumps(paths, indent=2))
    else:
        print("\n✗ Compiler binaries: NOT FOUND")
        sys.exit(1)
