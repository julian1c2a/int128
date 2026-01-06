#!/usr/bin/env python3
import os
import subprocess
import sys

# --- CONFIGURACIÓN DE RUTAS MANUALES (FALLBACKS) ---
# Incluimos tus rutas personalizadas del disco D:
MSVC_CANDIDATES = [
    r"D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat",
    r"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat",
    r"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat",
    r"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat"
]

INTEL_CANDIDATES = [
    r"C:\Program Files (x86)\Intel\oneAPI\setvars.bat",
    r"D:\Program Files (x86)\Intel\oneAPI\setvars.bat",
    r"C:\Program Files\Intel\oneAPI\setvars.bat",
    r"D:\Program Files\Intel\oneAPI\setvars.bat",
    r"C:\Program Files (x86)\Intel\oneAPI\compiler\latest\env\vars.bat"
]

VSWHERE = r"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"

def find_msvc():
    # 1. Intentar con vswhere
    if os.path.exists(VSWHERE):
        try:
            cmd = [VSWHERE, "-latest", "-products", "*", "-requires", "Microsoft.VisualStudio.Component.VC.Tools.x86.x64", "-property", "installationPath"]
            res = subprocess.run(cmd, capture_output=True, text=True)
            path = res.stdout.strip()
            if path:
                bat = os.path.join(path, "VC", "Auxiliary", "Build", "vcvarsall.bat")
                if os.path.exists(bat):
                    return bat
        except:
            pass
            
    # 2. Intentar rutas manuales
    for path in MSVC_CANDIDATES:
        if os.path.exists(path):
            return path
    return None

def find_intel():
    if "ONEAPI_ROOT" in os.environ:
        bat = os.path.join(os.environ["ONEAPI_ROOT"], "setvars.bat")
        if os.path.exists(bat):
            return bat
    for path in INTEL_CANDIDATES:
        if os.path.exists(path):
            return path
    return None

def main():
    msvc = find_msvc()
    if not msvc:
        sys.stderr.write("ERROR: No se encontró Visual Studio (vcvarsall.bat)\n")
        sys.exit(1)
        
    intel = find_intel()
    if not intel:
        sys.stderr.write("ERROR: No se encontró Intel oneAPI (setvars.bat)\n")
        sys.exit(1)
        
    # COMANDO CRÍTICO: Carga MSVC y LUEGO Intel en la misma sesión
    cmd = f'call "{msvc}" x64 && call "{intel}" intel64 && set'
    
    try:
        res = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    except Exception as e:
        sys.stderr.write(f"ERROR Ejecutando CMD: {e}\n")
        sys.exit(1)
        
    if res.returncode != 0:
        sys.stderr.write("ERROR: Fallaron los scripts batch.\n")
        sys.stderr.write(res.stderr)
        sys.exit(1)
        
    # Filtrar y Exportar
    env_vars = {}
    for line in res.stdout.splitlines():
        if "=" in line:
            k, v = line.split("=", 1)
            k = k.strip().upper()
            v = v.strip()
            
            # Lista blanca de variables importantes
            is_valid = k in ["PATH", "INCLUDE", "LIB", "LIBPATH"] or "INTEL" in k or "ONEAPI" in k or "VC" in k or "SDK" in k
            
            if is_valid:
                env_vars[k] = v
                
    # Imprimir PATH convertido
    if "PATH" in env_vars:
        win_path = env_vars["PATH"]
        unix_parts = []
        for p in win_path.split(";"):
            if not p: continue
            p = p.replace("\\", "/")
            if ":" in p:
                drv, rest = p.split(":", 1)
                p = f"/{drv.lower()}{rest}"
            unix_parts.append(p)
        print(f'export PATH="{":".join(unix_parts)}:$PATH"')
        
    # Imprimir resto
    for k, v in env_vars.items():
        if k != "PATH":
            safe_v = v.replace("\\", "\\\\").replace('"', '\\"')
            print(f'export {k}="{safe_v}"')

    print("export CC=icx")
    print("export CXX=icx")

if __name__ == "__main__":
    main()