#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
build_generic.py - Script genérico de compilación en Python
============================================================

Compila tests o benchmarks con todos los compiladores (GCC, Clang, Intel, MSVC)

Uso:
    python build_generic.py <type> <feature> <target> [compiler] [mode] [print]

Argumentos:
    type     : uint128 | int128
    feature  : bits | numeric | algorithm | iostreams | cmath | etc.
    target   : tests | benchs
    compiler : gcc | clang | intel | msvc | all (default: all)
    mode     : debug | release | all (default: all)
    print    : yes | no (default: no) - Imprime comandos de compilación

Ejemplos:
    python build_generic.py uint128 bits tests
    python build_generic.py int128 numeric benchs gcc release
    python build_generic.py uint128 algorithm tests all all yes
"""

import sys
import os
import subprocess
import shutil
from pathlib import Path
from typing import List, Optional

# Add env_setup to path for importing compiler_env module
sys.path.insert(0, str(Path(__file__).parent / "env_setup"))

try:
    from compiler_env import CompilerEnvironment
    USE_COMPILER_ENV = True
except ImportError:
    USE_COMPILER_ENV = False
    print("[WARN] compiler_env module not available, using default environment")

# Force UTF-8 encoding for Windows
if sys.platform == "win32":
    import io
    sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8', errors='replace')
    sys.stderr = io.TextIOWrapper(sys.stderr.buffer, encoding='utf-8', errors='replace')


class Colors:
    """ANSI color codes for terminal output"""
    GREEN = '\033[0;32m'
    RED = '\033[0;31m'
    YELLOW = '\033[1;33m'
    BLUE = '\033[0;34m'
    NC = '\033[0m'  # No Color


def echo_success(msg: str) -> None:
    print(f"{Colors.GREEN}OK {msg}{Colors.NC}")


def echo_error(msg: str) -> None:
    print(f"{Colors.RED}ERROR {msg}{Colors.NC}")


def echo_info(msg: str) -> None:
    print(f"{Colors.YELLOW}INFO {msg}{Colors.NC}")


def echo_header(msg: str) -> None:
    print(f"{Colors.BLUE}{msg}{Colors.NC}")


def find_compiler(compiler_cmd: str) -> Optional[str]:
    """Check if compiler exists in PATH"""
    return shutil.which(compiler_cmd)


def compile_with_compiler(
    compiler_name: str,
    compiler_cmd: str,
    source_file: str,
    build_dir: str,
    type_name: str,
    feature: str,
    output_suffix: str,
    modes: List[str],
    print_commands: bool,
    skip_check: bool = False,
    project_root: Path = None  # Add project_root parameter
) -> None:
    """Compile source file with specified compiler"""
    
    if project_root is None:
        project_root = Path.cwd()
    
    echo_info(f"Building with {compiler_name}...")
    
    # Check if compiler exists (unless skip_check is set)
    if not skip_check:
        if not find_compiler(compiler_cmd):
            echo_error(f"{compiler_name} not found ({compiler_cmd}). Skipping...")
            print()
            return
    
    # Get compiler environment (isolated, doesn't modify global environment)
    if USE_COMPILER_ENV:
        comp_env = CompilerEnvironment(compiler_name)
        env = comp_env.get_env()
        echo_info(f"  Using isolated environment for {compiler_name}")
    else:
        env = os.environ.copy()
    
    # Compile for each mode
    for mode in modes:
        output_dir = Path(build_dir) / compiler_name / mode
        output_dir.mkdir(parents=True, exist_ok=True)
        
        output = output_dir / f"{type_name}_{feature}_{output_suffix}_{compiler_name}"
        
        # Add .exe extension for Windows compilers
        if compiler_name in ["msvc", "intel"]:
            output = output.with_suffix(".exe")
        
        # Define compilation flags
        common_flags = ["-std=c++20", "-I./include"]
        
        if mode == "debug":
            mode_flags = ["-O0", "-g", "-DDEBUG"]
        else:
            mode_flags = ["-O3", "-DNDEBUG"]
        
        echo_info(f"  Compiling [{mode}]...")
        
        # Build command - convert paths to forward slashes for compatibility
        source_str = str(source_file).replace("\\", "/")
        output_str = str(output).replace("\\", "/")
        
        cmd = [compiler_cmd] + common_flags + mode_flags + [source_str, "-o", output_str]
        
        # Print command if requested
        if print_commands:
            echo_header(f"    $ {' '.join(cmd)}")
        
        # Execute compilation
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                cwd=project_root,  # Use project root from global scope
                env=env  # Use isolated compiler environment
            )
            
            # Check if compilation succeeded
            output_check = Path(output_str)  # Use the string path for checking
            if result.returncode == 0 or output_check.exists():
                echo_success(f"  {compiler_name} [{mode}]: {output_str}")
            else:
                echo_error(f"  {compiler_name} [{mode}]: compilation failed")
                echo_error(f"    Return code: {result.returncode}")
                if result.stderr:
                    print(f"\nSTDERR:\n{result.stderr}")
                if result.stdout:
                    print(f"\nSTDOUT:\n{result.stdout}")
        except Exception as e:
            echo_error(f"  {compiler_name} [{mode}]: {str(e)}")
    
    print()


def main():
    """Main function"""
    
    # Parse arguments
    if len(sys.argv) < 4:
        print("Error: Se requieren al menos 3 argumentos")
        print(f"Uso: {sys.argv[0]} <type> <feature> <target> [compiler] [mode] [print]")
        print()
        print("Ejemplos:")
        print(f"  python {sys.argv[0]} uint128 bits tests")
        print(f"  python {sys.argv[0]} int128 numeric benchs gcc release")
        print(f"  python {sys.argv[0]} uint128 algorithm tests all all yes")
        sys.exit(1)
    
    type_name = sys.argv[1]
    feature = sys.argv[2]
    target = sys.argv[3]
    compiler = sys.argv[4] if len(sys.argv) > 4 else "all"
    mode = sys.argv[5] if len(sys.argv) > 5 else "all"
    print_commands = (sys.argv[6].lower() == "yes") if len(sys.argv) > 6 else False
    
    # Validation
    if type_name not in ["uint128", "int128"]:
        print("Error: TYPE debe ser 'uint128' o 'int128'")
        sys.exit(1)
    
    if target not in ["tests", "benchs"]:
        print("Error: TARGET debe ser 'tests' o 'benchs'")
        sys.exit(1)
    
    valid_compilers = ["gcc", "clang", "intel", "msvc", "all"]
    if compiler not in valid_compilers:
        print(f"Error: COMPILER debe ser uno de: {', '.join(valid_compilers)}")
        sys.exit(1)
    
    valid_modes = ["debug", "release", "all"]
    if mode not in valid_modes:
        print(f"Error: MODE debe ser uno de: {', '.join(valid_modes)}")
        sys.exit(1)
    
    # Configuration
    project_root = Path(__file__).parent.parent
    os.chdir(project_root)
    
    if target == "tests":
        source_file = f"tests/{type_name}_{feature}_extracted_tests.cpp"
        build_dir = "build/build_tests"
        output_suffix = "tests"
    else:
        source_file = f"benchs/{type_name}_{feature}_extracted_benchs.cpp"
        build_dir = "build/build_benchs"
        output_suffix = "benchs"
    
    # Check source file
    if not Path(source_file).exists():
        echo_error(f"Source file not found: {source_file}")
        sys.exit(1)
    
    echo_info(f"Building {type_name} {feature} {target} for all compilers...")
    print()
    
    # Define modes to compile
    modes_to_compile = ["debug", "release"] if mode == "all" else [mode]
    
    # Get compiler commands from environment or use defaults
    gcc_cmd = os.environ.get("GCC_CXX", "g++")
    clang_cmd = os.environ.get("CLANG_CXX", "clang++")
    intel_cmd = os.environ.get("INTEL_CXX", "icx")
    msvc_cmd = os.environ.get("MSVC_CXX", "cl.exe")
    
    # Compile with each compiler
    if compiler in ["gcc", "all"]:
        compile_with_compiler(
            "gcc", gcc_cmd, source_file, build_dir,
            type_name, feature, output_suffix, modes_to_compile,
            print_commands, skip_check=True, project_root=project_root
        )
    
    if compiler in ["clang", "all"]:
        compile_with_compiler(
            "clang", clang_cmd, source_file, build_dir,
            type_name, feature, output_suffix, modes_to_compile,
            print_commands, project_root=project_root
        )
    
    if compiler in ["intel", "all"]:
        compile_with_compiler(
            "intel", intel_cmd, source_file, build_dir,
            type_name, feature, output_suffix, modes_to_compile,
            print_commands, project_root=project_root
        )
    
    if compiler in ["msvc", "all"]:
        compile_with_compiler(
            "msvc", msvc_cmd, source_file, build_dir,
            type_name, feature, output_suffix, modes_to_compile,
            print_commands, project_root=project_root
        )
    
    # Summary
    echo_success(f"Build complete for {type_name} {feature} {target}!")


if __name__ == "__main__":
    main()
