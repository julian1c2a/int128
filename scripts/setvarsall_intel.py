#!/usr/bin/env python3
"""
Script para ejecutar setvars.bat de Intel oneAPI y capturar las variables de entorno.
Similar a vcvarsall.py pero para Intel oneAPI.

Uso básico:
    python scripts/setvarsall_intel.py
    
Uso avanzado:
    python scripts/setvarsall_intel.py [args]
    
Donde [args] son argumentos opcionales para setvars.bat:
    - intel64 (default)
    - ia32
    - --config=<config_file>
    
El script ejecuta setvars.bat y retorna comandos de exportación para bash/MSYS2.
"""

import os
import subprocess
import sys
from setvars_intel import find_setvars_bat, find_intel_oneapi_root

def win_to_unix_path(win_path):
    """
    Convierte una ruta de Windows (C:\\Foo) a estilo MSYS2 (/c/Foo).
    Crítico para la variable PATH en Bash.
    """
    # Reemplazar backslashes
    p = win_path.replace("\\", "/")
    # Manejar unidad (C: -> /c)
    if ":" in p:
        drive, rest = p.split(":", 1)
        return f"/{drive.lower()}{rest}"
    return p

def get_intel_environment(args=None):
    """
    Ejecuta setvars.bat y captura TODAS las variables de entorno resultantes.
    
    Args:
        args: Lista de argumentos adicionales para setvars.bat
    
    Returns:
        Diccionario con las variables de entorno
    """
    setvars_path = find_setvars_bat()
    if not setvars_path:
        sys.stderr.write("Error: No se pudo encontrar setvars.bat de Intel oneAPI\n")
        sys.stderr.write("Buscar en: C:\\Program Files (x86)\\Intel\\oneAPI\n")
        sys.exit(1)
    
    # Construir comando para ejecutar setvars.bat y luego imprimir TODAS las variables
    cmd_parts = [f'"{setvars_path}"']
    
    # Añadir argumentos adicionales si existen
    if args:
        cmd_parts.extend(args)
    else:
        # Por defecto, configurar para intel64
        cmd_parts.append("intel64")
    
    # Añadir comando para imprimir TODAS las variables de entorno
    cmd_parts.append("&& set")
    
    full_cmd = " ".join(cmd_parts)
    
    # Ejecutar comando
    result = subprocess.run(full_cmd, shell=True, capture_output=True, text=True)
    
    if result.returncode != 0:
        sys.stderr.write(f"Error ejecutando setvars.bat:\n{result.stderr}\n")
        sys.exit(1)
    
    # Procesar la salida - capturar TODAS las variables después del '&& set'
    env_vars = {}
    capture = False
    
    for line in result.stdout.splitlines():
        # Detectar cuando comienza la salida de 'set'
        # Las líneas de set tienen el formato: VARIABLE=valor
        if "=" in line and not line.startswith("::"):
            # Dividir solo en el primer '='
            parts = line.split("=", 1)
            if len(parts) == 2:
                key, value = parts
                key = key.strip()
                value = value.strip()
                
                # Solo capturar variables relevantes para compilación
                # o que contengan Intel/oneAPI
                if (key in ["PATH", "INCLUDE", "LIB", "LIBPATH", "CPATH", 
                           "LIBRARY_PATH", "LD_LIBRARY_PATH", "CMAKE_PREFIX_PATH", 
                           "PKG_CONFIG_PATH", "TEMP", "TMP"] or
                    "INTEL" in key.upper() or 
                    "ONEAPI" in key.upper() or
                    "SETVARS" in key.upper() or
                    "VCINSTALLDIR" in key.upper() or
                    "VSCMD" in key.upper() or
                    "WINDOWSSDKDIR" in key.upper()):
                    
                    env_vars[key] = value
    
    return env_vars

def print_bash_exports(env_vars):
    """
    Imprime comandos de exportación para bash/MSYS2.
    """
    for key, value in env_vars.items():
        # Convertir PATH a formato UNIX
        if key == "PATH":
            # Dividir por ; y convertir cada ruta
            paths = value.split(";")
            unix_paths = [win_to_unix_path(p) for p in paths if p]
            value = ":".join(unix_paths)
        
        # Escapar comillas y otros caracteres especiales
        value = value.replace('"', '\\"')
        
        print(f'export {key}="{value}"')

def print_cmd_sets(env_vars):
    """
    Imprime comandos SET para CMD/PowerShell.
    """
    for key, value in env_vars.items():
        print(f'set {key}={value}')

if __name__ == "__main__":
    # Determinar el formato de salida
    output_format = "bash"  # default
    args_for_setvars = []
    
    # Procesar argumentos
    i = 1
    while i < len(sys.argv):
        arg = sys.argv[i]
        if arg == "--format=cmd" or arg == "-f=cmd":
            output_format = "cmd"
        elif arg == "--format=bash" or arg == "-f=bash":
            output_format = "bash"
        else:
            # Pasar el argumento a setvars.bat
            args_for_setvars.append(arg)
        i += 1
    
    # Obtener variables de entorno
    env_vars = get_intel_environment(args_for_setvars if args_for_setvars else None)
    
    if not env_vars:
        sys.stderr.write("Error: No se pudieron obtener las variables de entorno\n")
        sys.exit(1)
    
    # Imprimir en el formato solicitado
    if output_format == "cmd":
        print_cmd_sets(env_vars)
    else:
        print_bash_exports(env_vars)
