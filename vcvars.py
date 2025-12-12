import os
import subprocess
import json

# Rutas estándar donde suele estar vswhere
VSWHERE_PATH_LEGACY = r"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
VSWHERE_PATH_NEW = r"C:\Program Files\Microsoft Visual Studio\Installer\vswhere.exe"

def find_vswhere():
    """Busca el ejecutable vswhere.exe en el sistema."""
    if os.path.exists(VSWHERE_PATH_NEW):
        return VSWHERE_PATH_NEW
    if os.path.exists(VSWHERE_PATH_LEGACY):
        return VSWHERE_PATH_LEGACY
    return None

def find_visual_studio_installation():
    """
    Utiliza vswhere para encontrar la instalación más reciente de VS que tenga
    las herramientas de C++ (VC).
    Retorna la ruta raíz de la instalación.
    """
    vswhere = find_vswhere()
    if not vswhere:
        # Fallback manual si vswhere no existe (raro en instalaciones modernas)
        candidates = [
            r"C:\Program Files\Microsoft Visual Studio\18\Community",
            r"C:\Program Files\Microsoft Visual Studio\2022\Community",
            r"D:\Program Files\Microsoft Visual Studio\2022\Community",
            r"C:\Program Files\Microsoft Visual Studio\2022\Enterprise",
            r"C:\Program Files\Microsoft Visual Studio\2022\Professional"
        ]
        for c in candidates:
            if os.path.exists(os.path.join(c, "VC", "Auxiliary", "Build", "vcvarsall.bat")):
                return c
        return None

    try:
        # Pedimos formato JSON para ser más robustos al parsear
        cmd = [
            vswhere,
            "-latest",
            "-products", "*",
            "-requires", "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
            "-format", "json"
        ]
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        installations = json.loads(result.stdout)
        
        if not installations:
            return None
            
        # Retornamos la ruta de instalación del primero encontrado (el más reciente)
        return installations[0].get("installationPath")
        
    except (subprocess.CalledProcessError, json.JSONDecodeError):
        return None

def get_vcvarsall_path():
    """Devuelve la ruta completa a vcvarsall.bat."""
    root = find_visual_studio_installation()
    if not root:
        return None
    
    bat_path = os.path.join(root, "VC", "Auxiliary", "Build", "vcvarsall.bat")
    if os.path.exists(bat_path):
        return bat_path
    return None

if __name__ == "__main__":
    # Test simple si se ejecuta directo
    path = get_vcvarsall_path()
    if path:
        print(f"FOUND: {path}")
    else:
        print("NOT FOUND")