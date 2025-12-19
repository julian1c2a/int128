import os
import subprocess
import sys
import platform

# Configuración por defecto
DEFAULT_ARCH = "x64" # Puede ser x86, x64, arm64
VSWHERE_PATH = r"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"

def find_vcvarsall():
    """
    Busca la ubicación de vcvarsall.bat usando vswhere.exe.
    """
    if not os.path.exists(VSWHERE_PATH):
        # Intento fallback en caso de instalación no estándar o antigua
        posibles = [
            r"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat",
            r"C:\Program Files\Microsoft Visual Studio\18\Professional\VC\Auxiliary\Build\vcvarsall.bat",
            r"C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Auxiliary\Build\vcvarsall.bat",
            r"D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" # Tu ruta D:
        ]
        for p in posibles:
            if os.path.exists(p):
                return p
        return None

    # Usar vswhere para obtener la ruta de instalación más reciente
    try:
        cmd = [VSWHERE_PATH, "-latest", "-products", "*", "-requires", "Microsoft.VisualStudio.Component.VC.Tools.x86.x64", "-property", "installationPath"]
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        install_path = result.stdout.strip()
        
        if not install_path:
            return None
            
        vcvars = os.path.join(install_path, "VC", "Auxiliary", "Build", "vcvarsall.bat")
        if os.path.exists(vcvars):
            return vcvars
    except Exception as e:
        sys.stderr.write(f"Error ejecutando vswhere: {e}\n")
        
    return None

def win_to_unix_path(win_path):
    """
    Convierte una ruta de Windows (C:\\Foo) a estilo MSYS2 (/c/Foo).
    Esto es CRÍTICO para la variable PATH en Bash.
    """
    # 1. Reemplazar backslashes
    p = win_path.replace("\\", "/")
    # 2. Manejar unidad (C: -> /c)
    if ":" in p:
        drive, rest = p.split(":", 1)
        return f"/{drive.lower()}{rest}"
    return p

def get_msvc_environment(arch):
    vcvars_path = find_vcvarsall()
    if not vcvars_path:
        sys.stderr.write("Error: No se pudo encontrar vcvarsall.bat\n")
        sys.exit(1)

    # Variables de interés que necesitamos propagar al entorno de Bash
    keys_to_extract = [
        "PATH", "INCLUDE", "LIB", "LIBPATH", 
        "WindowsSdkDir", "WindowsLibPath", "UniversalCRTSdkDir", "UCRTVersion",
        "VCINSTALLDIR", "VisualStudioVersion"
    ]

    # Construimos un comando que:
    # 1. Llame a vcvarsall.bat con la arquitectura
    # 2. Imprima las variables de entorno resultantes con un delimitador seguro
    cmd_list = [f'"{vcvars_path}"', arch, "&&"]
    for key in keys_to_extract:
        cmd_list.append(f'echo {key}=%{key}%')
    
    full_cmd = " ".join(cmd_list)

    # Ejecutamos cmd.exe
    # shell=True es necesario para ejecutar el batch
    result = subprocess.run(full_cmd, shell=True, capture_output=True, text=True)

    if result.returncode != 0:
        sys.stderr.write(f"Error ejecutando vcvarsall.bat:\n{result.stderr}\n")
        sys.exit(1)

    # Procesar la salida
    env_vars = {}
    for line in result.stdout.splitlines():
        if "=" in line:
            # Dividir solo en el primer '='
            key, value = line.split("=", 1)
            key = key.strip()
            value = value.strip()
            
            # Si el valor es %KEY%, significa que la variable no está definida en el batch
            if value == f"%{key}%":
                continue
                
            if key in keys_to_extract:
                env_vars[key] = value

    return env_vars

def main():
    arch = sys.argv[1] if len(sys.argv) > 1 else DEFAULT_ARCH
    
    env_vars = get_msvc_environment(arch)
    
    print(f"# Entorno MSVC generado para arquitectura: {arch}")
    
    for key, value in env_vars.items():
        if key.upper() == "PATH":
            # Para el PATH, necesitamos una cirugía especial.
            # Convertimos las rutas de Windows a MSYS2 y unimos con ':'
            win_paths = value.split(";")
            unix_paths = [win_to_unix_path(p) for p in win_paths if p]
            # No queremos sobrescribir totalmente el PATH de MSYS (ls, grep, etc), 
            # así que lo añadimos al principio.
            new_path = ":".join(unix_paths)
            print(f'export PATH="{new_path}:$PATH"')
        else:
            # Para INCLUDE, LIB, etc., DEJAMOS el formato Windows (backslashes).
            # ¿Por qué? Porque estas variables son leídas por cl.exe y link.exe,
            # que esperan rutas de Windows, no de Bash.
            # Escapamos los backslashes para que Bash no se queje.
            safe_value = value.replace("\\", "\\\\")
            print(f'export {key}="{safe_value}"')

    print(f'export CC=cl')
    print(f'export CXX=cl')
    print("# Configuración completada.")

if __name__ == "__main__":
    main()