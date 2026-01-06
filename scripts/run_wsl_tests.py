import subprocess
import sys
import os
import shutil

# Forzar codificación UTF-8 para la salida en Windows (evita UnicodeEncodeError)
if sys.platform == "win32":
    import io
    sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8', errors='replace')
    sys.stderr = io.TextIOWrapper(sys.stderr.buffer, encoding='utf-8', errors='replace')

def fix_line_endings(directory):
    """
    Convierte recursivamente CRLF a LF en scripts de shell y python
    para evitar errores de sintaxis en Linux/WSL ($'\r': command not found).
    """
    print(f"🔧 Normalizando finales de línea (CRLF -> LF) en {directory}...")
    count = 0
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith(('.bash', '.sh', '.py')):
                filepath = os.path.join(root, file)
                try:
                    with open(filepath, 'rb') as f:
                        content = f.read()
                    
                    if b'\r\n' in content:
                        content = content.replace(b'\r\n', b'\n')
                        with open(filepath, 'wb') as f:
                            f.write(content)
                        count += 1
                except Exception as e:
                    print(f"  ⚠️ Error procesando {file}: {e}")
    
    if count > 0:
        print(f"  ✅ Se corrigieron {count} archivos.")
    else:
        print("  ✅ Todos los archivos ya tienen formato LF.")

def run_wsl_tests():
    """
    Ejecuta los tests del proyecto dentro de WSL (Windows Subsystem for Linux).
    Puente automático entre el entorno de desarrollo Windows y la validación en Linux.
    """
    print("🐧 Iniciando puente de pruebas Windows -> WSL (Ubuntu)...")
    
    # 1. Verificar que WSL está disponible
    if shutil.which("wsl") is None:
        print("Error: No se encontró el comando 'wsl'. Asegúrate de tener WSL instalado.")
        sys.exit(1)

    # 2. Convertir ruta actual de Windows a ruta WSL (/mnt/c/...)
    # Esto permite ejecutar los tests sobre los mismos archivos que estás editando
    cwd = os.getcwd()
    drive, path = os.path.splitdrive(cwd)
    
    if drive:
        # Ruta Windows estándar (C:\...)
        drive_letter = drive[0].lower()
        wsl_path = f"/mnt/{drive_letter}{path.replace(os.sep, '/')}"
    else:
        # Ruta tipo POSIX/MSYS2 (/c/...)
        # Heurística: si empieza por /x/ asumimos que es una unidad montada
        if cwd.startswith('/') and len(cwd) >= 3 and cwd[2] == '/' and cwd[1].isalpha():
            wsl_path = f"/mnt/{cwd[1].lower()}{cwd[2:]}"
        else:
            # Fallback: usar la ruta tal cual (puede fallar si no coincide con WSL)
            wsl_path = cwd
    
    print(f"📂 Directorio del proyecto en WSL: {wsl_path}")

    # Corregir finales de línea antes de ejecutar en WSL
    scripts_dir = os.path.join(cwd, "scripts")
    if os.path.exists(scripts_dir):
        fix_line_endings(scripts_dir)

    # 3. Construir el comando compuesto para bash
    # - cd al directorio
    # - Limpiar builds previos (importante: los binarios de Windows y Linux son incompatibles en la misma carpeta build/)
    # - Inicializar entorno (detectar g++/clang en Linux)
    # - Ejecutar tests
    
    # Intentar cargar entorno Intel oneAPI si existe (ubicación estándar)
    intel_env_cmd = "source /opt/intel/oneapi/setvars.sh > /dev/null 2>&1 || true"
    
    bash_cmd = (
        f"{intel_env_cmd} && cd '{wsl_path}' && "
        "echo '🔄 [WSL] Limpiando artefactos de compilación previos...' && "
        "rm -rf build/build_tests build/build_benchs && "
        "echo '⚙️ [WSL] Detectando compiladores en Linux...' && "
        "python3 make.py init && "
        "echo '🧪 [WSL] Ejecutando batería de tests...' && "
        "python3 make.py test"
    )

    # 4. Ejecutar
    try:
        # check=True lanzará una excepción si el comando falla (exit code != 0)
        subprocess.run(["wsl", "bash", "-c", bash_cmd], check=True)
        print("\n✅ [Windows] Tests en entorno Linux completados exitosamente.")
    except subprocess.CalledProcessError as e:
        print(f"\n❌ [Windows] Fallo en los tests de Linux (Código de salida: {e.returncode}).")
        sys.exit(e.returncode)

if __name__ == "__main__":
    run_wsl_tests()