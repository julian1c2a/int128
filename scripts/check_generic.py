#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
check_generic.py - Script genérico para ejecutar tests
=======================================================

Ejecuta tests compilados y muestra resultados

Uso:
    python check_generic.py <type> <feature> [compiler] [mode]

Argumentos:
    type     : uint128 | int128
    feature  : bits | numeric | algorithm | etc.
    compiler : gcc | clang | intel | msvc | all (default: all)
    mode     : debug | release | all (default: all)

Ejemplos:
    python check_generic.py uint128 bits
    python check_generic.py int128 numeric gcc release
"""

import sys
import os
import subprocess
from pathlib import Path
from typing import Dict, List

# Force UTF-8 encoding for Windows
if sys.platform == "win32":
    import io
    sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8', errors='replace')
    sys.stderr = io.TextIOWrapper(sys.stderr.buffer, encoding='utf-8', errors='replace')

# Add env_setup to path
sys.path.insert(0, str(Path(__file__).parent / "env_setup"))

try:
    from compiler_env import CompilerEnvironment
    USE_COMPILER_ENV = True
except ImportError:
    USE_COMPILER_ENV = False


class Colors:
    GREEN = '\033[0;32m'
    RED = '\033[0;31m'
    YELLOW = '\033[1;33m'
    BLUE = '\033[0;34m'
    NC = '\033[0m'


def echo_success(msg: str):
    print(f"{Colors.GREEN}[OK] {msg}{Colors.NC}")

def echo_error(msg: str):
    print(f"{Colors.RED}[ERROR] {msg}{Colors.NC}")

def echo_info(msg: str):
    print(f"{Colors.YELLOW}-> {msg}{Colors.NC}")

def echo_header(msg: str):
    print(f"{Colors.BLUE}{msg}{Colors.NC}")


def run_test(executable: Path, compiler: str) -> bool:
    """Ejecuta un test y retorna True si pasa"""
    
    if not executable.exists():
        return False
    
    try:
        # Obtener entorno aislado si está disponible
        if USE_COMPILER_ENV:
            comp_env = CompilerEnvironment(compiler)
            env = comp_env.get_env()
        else:
            env = os.environ.copy()
        
        result = subprocess.run(
            [str(executable)],
            capture_output=True,
            text=True,
            timeout=30,
            env=env
        )
        
        return result.returncode == 0
    
    except subprocess.TimeoutExpired:
        echo_error(f"Timeout ejecutando {executable.name}")
        return False
    except Exception as e:
        echo_error(f"Error ejecutando {executable.name}: {e}")
        return False


def main():
    if len(sys.argv) < 3:
        print("Error: Se requieren al menos 2 argumentos")
        print(f"Uso: {sys.argv[0]} <type> <feature> [compiler] [mode]")
        sys.exit(1)
    
    type_name = sys.argv[1]
    feature = sys.argv[2]
    compiler_arg = sys.argv[3] if len(sys.argv) > 3 else "all"
    mode_arg = sys.argv[4] if len(sys.argv) > 4 else "all"
    
    # Validation
    if type_name not in ["uint128", "int128"]:
        print("Error: TYPE debe ser 'uint128' o 'int128'")
        sys.exit(1)
    
    # Configuration
    project_root = Path(__file__).parent.parent
    build_dir = project_root / "build" / "build_tests"
    
    # Determine compilers and modes to test
    compilers = ["gcc", "clang", "intel", "msvc"] if compiler_arg == "all" else [compiler_arg]
    modes = ["debug", "release"] if mode_arg == "all" else [mode_arg]
    
    echo_header("=" * 60)
    echo_header(f"  TEST EXECUTION: {type_name}_{feature}_tests")
    echo_header("=" * 60)
    print()
    
    # Track results
    results: Dict[str, Dict[str, bool]] = {}
    
    for compiler in compilers:
        results[compiler] = {}
        
        for mode in modes:
            exe_name = f"{type_name}_{feature}_tests_{compiler}"
            if compiler == "msvc":
                exe_name += ".exe"
            
            # Build path with forward slashes for compatibility
            exe_path_str = f"build/build_tests/{compiler}/{mode}/{exe_name}"
            exe_path = build_dir / compiler / mode / exe_name
            
            # On Windows, GCC/Clang may generate .exe even if not explicitly specified
            if not exe_path.exists() and sys.platform == "win32":
                exe_path_with_ext = exe_path.with_suffix(".exe")
                if exe_path_with_ext.exists():
                    exe_path = exe_path_with_ext
            
            echo_info(f"Testing {compiler} [{mode}]...")
            
            if exe_path.exists():
                passed = run_test(exe_path, compiler)
                results[compiler][mode] = passed
                
                if passed:
                    echo_success(f"  {compiler} [{mode}]: PASS")
                else:
                    echo_error(f"  {compiler} [{mode}]: FAIL")
            else:
                echo_error(f"  {compiler} [{mode}]: NOT FOUND")
                echo_info(f"     Expected: {exe_path}")
                results[compiler][mode] = False
    
    # Summary table
    print()
    echo_header("=" * 60)
    echo_header("  RESULTS SUMMARY")
    echo_header("=" * 60)
    print()
    print(f"{'Compiler':<12} | {'Debug':<12} | {'Release':<12}")
    print("-" * 42)
    
    for compiler in compilers:
        debug_result = "[OK] PASS" if results[compiler].get("debug", False) else "[ERROR] FAIL"
        release_result = "[OK] PASS" if results[compiler].get("release", False) else "[ERROR] FAIL"
        
        if mode_arg != "all":
            if mode_arg == "debug":
                print(f"{compiler:<12} | {debug_result:<12} | {'- N/A':<12}")
            else:
                print(f"{compiler:<12} | {'- N/A':<12} | {release_result:<12}")
        else:
            print(f"{compiler:<12} | {debug_result:<12} | {release_result:<12}")
    
    print()
    
    # Calculate pass rate
    total_tests = len([r for comp in results.values() for r in comp.values()])
    passed_tests = len([r for comp in results.values() for r in comp.values() if r])
    
    echo_header(f"Pass rate: {passed_tests}/{total_tests}")
    
    return 0 if passed_tests == total_tests else 1


if __name__ == "__main__":
    sys.exit(main())
