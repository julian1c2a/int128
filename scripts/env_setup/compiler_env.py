#!/usr/bin/env python3
"""
Módulo para detectar y configurar entornos de compiladores
Guarda y carga configuraciones desde archivos de texto
"""

import os
import sys
import subprocess
import json
import shutil
from pathlib import Path
from typing import Dict, Optional, List

# Directorio para guardar configuraciones
CONFIG_DIR = Path(__file__).parent.parent.parent / "build" / "compiler_envs"
CONFIG_DIR.mkdir(parents=True, exist_ok=True)

class CompilerEnvironment:
    """Gestiona entornos de compiladores sin modificar el entorno global"""
    
    def __init__(self, compiler: str):
        self.compiler = compiler.lower()
        self.config_file = CONFIG_DIR / f"{self.compiler}_env.json"
        self.env_cache: Optional[Dict[str, str]] = None
    
    def detect_and_save(self) -> bool:
        """Detecta el compilador y guarda su configuración"""
        print(f"-> Detectando entorno para {self.compiler}...")
        
        if self.compiler == "gcc":
            env = self._detect_gcc()
        elif self.compiler == "clang":
            env = self._detect_clang()
        elif self.compiler == "msvc":
            env = self._detect_msvc()
        elif self.compiler == "intel" or self.compiler == "icx":
            env = self._detect_intel()
        else:
            print(f"[ERROR] Compilador desconocido: {self.compiler}")
            return False
        
        if env:
            self._save_config(env)
            self.env_cache = env
            print(f"[OK] Configuración guardada en {self.config_file}")
            return True
        else:
            print(f"[ERROR] No se pudo detectar {self.compiler}")
            return False
    
    def load(self) -> Optional[Dict[str, str]]:
        """Carga la configuración guardada"""
        if self.env_cache:
            return self.env_cache
        
        if not self.config_file.exists():
            print(f"[INFO] No hay configuración guardada para {self.compiler}")
            print(f"       Ejecuta: python scripts/env_setup/compiler_env.py --detect {self.compiler}")
            return None
        
        try:
            with open(self.config_file, 'r', encoding='utf-8') as f:
                self.env_cache = json.load(f)
            return self.env_cache
        except Exception as e:
            print(f"[ERROR] Error cargando configuración: {e}")
            return None
    
    def get_env(self) -> Dict[str, str]:
        """Obtiene el entorno del compilador (carga si es necesario)"""
        env = self.load()
        if env is None:
            # Intentar detectar automáticamente
            if self.detect_and_save():
                env = self.env_cache
            else:
                # Fallback: usar entorno actual
                env = os.environ.copy()
        
        return env or os.environ.copy()
    
    def _save_config(self, env: Dict[str, str]):
        """Guarda la configuración en JSON"""
        with open(self.config_file, 'w', encoding='utf-8') as f:
            json.dump(env, f, indent=2)
    
    def _run_bash_script(self, script_path: Path) -> Optional[Dict[str, str]]:
        """Ejecuta un script bash y captura las variables de entorno resultantes"""
        if not script_path.exists():
            print(f"[ERROR] Script no encontrado: {script_path}")
            return None
        
        # Ejecutar el script y capturar el entorno
        cmd = f'source "{script_path}" && env'
        
        try:
            result = subprocess.run(
                ['bash', '-c', cmd],
                capture_output=True,
                text=True,
                timeout=30
            )
            
            if result.returncode != 0:
                print(f"[ERROR] Error ejecutando script: {result.stderr}")
                return None
            
            # Parsear la salida
            env = {}
            for line in result.stdout.splitlines():
                if '=' in line:
                    key, value = line.split('=', 1)
                    env[key] = value
            
            return env
        except subprocess.TimeoutExpired:
            print(f"[ERROR] Timeout ejecutando script bash")
            return None
        except Exception as e:
            print(f"[ERROR] Excepción: {e}")
            return None
    
    def _detect_gcc(self) -> Optional[Dict[str, str]]:
        """Detecta GCC"""
        gcc_path = shutil.which("g++")
        if not gcc_path:
            return None
        
        # Usar entorno base del sistema
        env = os.environ.copy()
        
        # Añadir solo las variables específicas de GCC
        env['CC'] = 'gcc'
        env['CXX'] = 'g++'
        env['GCC_PATH'] = gcc_path
        
        # Obtener versión
        try:
            result = subprocess.run(
                ['g++', '--version'],
                capture_output=True,
                text=True
            )
            env['GCC_VERSION'] = result.stdout.split('\n')[0]
        except:
            pass
        
        return env
    
    def _detect_clang(self) -> Optional[Dict[str, str]]:
        """Detecta Clang"""
        clang_path = shutil.which("clang++")
        if not clang_path:
            return None
        
        env = os.environ.copy()
        env['CC'] = 'clang'
        env['CXX'] = 'clang++'
        env['CLANG_PATH'] = clang_path
        
        # Obtener versión
        try:
            result = subprocess.run(
                ['clang++', '--version'],
                capture_output=True,
                text=True
            )
            env['CLANG_VERSION'] = result.stdout.split('\n')[0]
        except:
            pass
        
        return env
    
    def _detect_msvc(self) -> Optional[Dict[str, str]]:
        """Detecta MSVC usando vcvarsall.bat"""
        # Intentar cargar desde el script bash existente
        script_path = Path(__file__).parent / "setup_msvc_env.bash"
        
        if script_path.exists():
            return self._run_bash_script(script_path)
        
        # Fallback: detección directa
        return self._detect_msvc_direct()
    
    def _detect_msvc_direct(self) -> Optional[Dict[str, str]]:
        """Detección directa de MSVC usando vcvarsall.bat"""
        vswhere = Path(r"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe")
        
        if not vswhere.exists():
            return None
        
        try:
            # Buscar la instalación de Visual Studio
            result = subprocess.run(
                [str(vswhere), "-latest", "-products", "*", 
                 "-requires", "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
                 "-property", "installationPath"],
                capture_output=True,
                text=True
            )
            
            install_path = result.stdout.strip()
            if not install_path:
                return None
            
            vcvarsall = Path(install_path) / "VC" / "Auxiliary" / "Build" / "vcvarsall.bat"
            if not vcvarsall.exists():
                return None
            
            # Ejecutar vcvarsall y capturar entorno
            cmd = f'"{vcvarsall}" x64 && set'
            result = subprocess.run(
                cmd,
                shell=True,
                capture_output=True,
                text=True,
                timeout=30
            )
            
            # Parsear variables de entorno
            env = {}
            for line in result.stdout.splitlines():
                if '=' in line:
                    key, value = line.split('=', 1)
                    env[key] = value
            
            return env
        except Exception as e:
            print(f"[ERROR] Error detectando MSVC: {e}")
            return None
    
    def _detect_intel(self) -> Optional[Dict[str, str]]:
        """Detecta Intel oneAPI usando el script bash existente"""
        script_path = Path(__file__).parent / "setup_intel_env.bash"
        
        if script_path.exists():
            return self._run_bash_script(script_path)
        
        return None


def detect_all_compilers():
    """Detecta todos los compiladores disponibles y guarda sus configuraciones"""
    compilers = ["gcc", "clang", "msvc", "intel"]
    results = {}
    
    print("=" * 60)
    print("  DETECCIÓN DE COMPILADORES")
    print("=" * 60)
    
    for compiler in compilers:
        print(f"\n[{compiler.upper()}]")
        env = CompilerEnvironment(compiler)
        success = env.detect_and_save()
        results[compiler] = success
    
    print("\n" + "=" * 60)
    print("  RESUMEN")
    print("=" * 60)
    
    for compiler, success in results.items():
        status = "[OK]" if success else "[FAIL]"
        print(f"{status} {compiler}")
    
    print(f"\nConfiguraciones guardadas en: {CONFIG_DIR}")
    return results


def main():
    """Interfaz de línea de comandos"""
    import argparse
    
    parser = argparse.ArgumentParser(description="Gestión de entornos de compiladores")
    parser.add_argument("--detect", choices=["gcc", "clang", "msvc", "intel", "all"],
                       help="Detectar y guardar configuración de compilador")
    parser.add_argument("--show", choices=["gcc", "clang", "msvc", "intel"],
                       help="Mostrar configuración guardada")
    parser.add_argument("--list", action="store_true",
                       help="Listar configuraciones disponibles")
    
    args = parser.parse_args()
    
    if args.detect:
        if args.detect == "all":
            detect_all_compilers()
        else:
            env = CompilerEnvironment(args.detect)
            env.detect_and_save()
    
    elif args.show:
        env = CompilerEnvironment(args.show)
        config = env.load()
        if config:
            print(json.dumps(config, indent=2))
        else:
            print(f"No hay configuración para {args.show}")
    
    elif args.list:
        print(f"Configuraciones en {CONFIG_DIR}:")
        for config_file in CONFIG_DIR.glob("*_env.json"):
            compiler = config_file.stem.replace("_env", "")
            print(f"  - {compiler}")
    
    else:
        parser.print_help()


if __name__ == "__main__":
    main()
