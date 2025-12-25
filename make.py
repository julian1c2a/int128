#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
make.py - Sistema de build centralizado para int128 project
============================================================

Reemplaza Makefile tradicional con lógica Python y entornos aislados.

COMANDOS PRINCIPALES:
    init                 Detecta y configura compiladores
    build <args>        Compila tests o benchmarks
    check <args>        Ejecuta tests
    run <args>          Ejecuta benchmarks
    clean               Limpia directorios de build
    
TARGETS ESPECIALES:
    all                 Compila todo (tests + benchs) con todos los compiladores
    test                Compila y ejecuta todos los tests
    bench               Compila y ejecuta todos los benchmarks
    list                Lista todas las combinaciones disponibles

EJEMPLOS:
    python make.py init
    python make.py build uint128 bits tests gcc release
    python make.py check uint128 bits
    python make.py test
    python make.py all
    python make.py clean

ARGUMENTOS PARA BUILD/CHECK/RUN:
    <type>      uint128 | int128
    <feature>   bits | numeric | algorithm | iostreams | cmath | etc.
    <target>    tests | benchs (solo para build)
    <compiler>  gcc | clang | intel | msvc | all
    <mode>      debug | release | all
"""

import sys
import os
import subprocess
import shutil
import argparse
from pathlib import Path
from typing import List, Dict, Optional, Tuple
import json
import time

# Force UTF-8 encoding for Windows
if sys.platform == "win32":
    import io
    sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8', errors='replace')
    sys.stderr = io.TextIOWrapper(sys.stderr.buffer, encoding='utf-8', errors='replace')

# Paths
PROJECT_ROOT = Path(__file__).parent
SCRIPTS_DIR = PROJECT_ROOT / "scripts"
BUILD_DIR = PROJECT_ROOT / "build"
TESTS_DIR = PROJECT_ROOT / "tests"
BENCHS_DIR = PROJECT_ROOT / "benchs"
COMPILER_ENVS_DIR = BUILD_DIR / "compiler_envs"

# Colors
class Colors:
    GREEN = '\033[0;32m'
    RED = '\033[0;31m'
    YELLOW = '\033[1;33m'
    BLUE = '\033[0;34m'
    CYAN = '\033[0;36m'
    MAGENTA = '\033[0;35m'
    NC = '\033[0m'

def echo_success(msg: str):
    print(f"{Colors.GREEN}[OK] {msg}{Colors.NC}")

def echo_error(msg: str):
    print(f"{Colors.RED}[ERROR] {msg}{Colors.NC}")

def echo_info(msg: str):
    print(f"{Colors.CYAN}[INFO] {msg}{Colors.NC}")

def echo_header(msg: str):
    print(f"{Colors.BLUE}{msg}{Colors.NC}")

def echo_warning(msg: str):
    print(f"{Colors.YELLOW}[WARN] {msg}{Colors.NC}")


def run_script(script_name: str, args: List[str]) -> int:
    """Ejecuta un script Python de scripts/"""
    script_path = SCRIPTS_DIR / script_name
    
    if not script_path.exists():
        echo_error(f"Script no encontrado: {script_path}")
        return 1
    
    cmd = [sys.executable, str(script_path)] + args
    result = subprocess.run(cmd, cwd=PROJECT_ROOT)
    return result.returncode


def cmd_init(args: argparse.Namespace) -> int:
    """Inicializa el proyecto detectando compiladores"""
    echo_header("=" * 60)
    echo_header("  INICIALIZACIÓN DEL PROYECTO")
    echo_header("=" * 60)
    print()
    
    return run_script("init_project.py", [])


def cmd_build(args: argparse.Namespace) -> int:
    """Compila tests o benchmarks"""
    if not args.type or not args.feature or not args.target:
        echo_error("build requiere: <type> <feature> <target> [compiler] [mode]")
        return 1
    
    build_args = [
        args.type,
        args.feature,
        args.target,
        args.compiler or "all",
        args.mode or "all"
    ]
    
    if args.verbose:
        build_args.append("yes")
    
    return run_script("build_generic.py", build_args)


def cmd_check(args: argparse.Namespace) -> int:
    """Ejecuta tests"""
    if not args.type or not args.feature:
        echo_error("check requiere: <type> <feature> [compiler] [mode]")
        return 1
    
    check_args = [
        args.type,
        args.feature,
        args.compiler or "all",
        args.mode or "all"
    ]
    
    return run_script("check_generic.py", check_args)


def cmd_run(args: argparse.Namespace) -> int:
    """Ejecuta benchmarks"""
    if not args.type or not args.feature:
        echo_error("run requiere: <type> <feature> [compiler] [mode]")
        return 1
    
    run_args = [
        args.type,
        args.feature,
        args.compiler or "all",
        args.mode or "release"  # Default to release for benchmarks
    ]
    
    return run_script("run_generic.py", run_args)


def cmd_clean(args: argparse.Namespace) -> int:
    """Limpia directorios de build"""
    echo_info("Limpiando directorios de build...")
    
    # Limpiar build_tests y build_benchs
    cleaned = []
    
    build_tests = BUILD_DIR / "build_tests"
    if build_tests.exists():
        shutil.rmtree(build_tests)
        cleaned.append("build_tests/")
    
    build_benchs = BUILD_DIR / "build_benchs"
    if build_benchs.exists():
        shutil.rmtree(build_benchs)
        cleaned.append("build_benchs/")
    
    # Opcional: limpiar compiler_envs si se pide
    if args.all:
        if COMPILER_ENVS_DIR.exists():
            shutil.rmtree(COMPILER_ENVS_DIR)
            cleaned.append("compiler_envs/")
    
    if cleaned:
        echo_success(f"Limpiado: {', '.join(cleaned)}")
    else:
        echo_info("Nada que limpiar")
    
    return 0


def get_all_combinations() -> List[Tuple[str, str]]:
    """Obtiene todas las combinaciones type-feature disponibles"""
    combinations = []
    
    for tests_file in TESTS_DIR.glob("*_*_extracted_tests.cpp"):
        name = tests_file.stem
        parts = name.replace("_extracted_tests", "").split("_", 1)
        if len(parts) == 2:
            combinations.append((parts[0], parts[1]))
    
    return sorted(set(combinations))


def cmd_all(args: argparse.Namespace) -> int:
    """Compila todo (tests + benchs) con todos los compiladores"""
    echo_header("=" * 60)
    echo_header("  COMPILACIÓN COMPLETA DEL PROYECTO")
    echo_header("=" * 60)
    print()
    
    combinations = get_all_combinations()
    
    if not combinations:
        echo_error("No se encontraron archivos de tests/benchs")
        return 1
    
    echo_info(f"Compilando {len(combinations)} combinaciones × 2 targets × 4 compiladores × 2 modos")
    echo_info("Esto puede tomar varios minutos...")
    print()
    
    total = len(combinations) * 2  # tests + benchs
    success = 0
    failed = []
    
    start_time = time.time()
    
    for i, (type_name, feature) in enumerate(combinations, 1):
        echo_header(f"\n[{i}/{len(combinations)}] {type_name}_{feature}")
        
        # Build tests
        echo_info(f"  Compilando tests...")
        ret = run_script("build_generic.py", [type_name, feature, "tests", "all", "all"])
        if ret == 0:
            success += 1
        else:
            failed.append(f"{type_name}_{feature}_tests")
        
        # Build benchs
        echo_info(f"  Compilando benchs...")
        ret = run_script("build_generic.py", [type_name, feature, "benchs", "all", "all"])
        if ret == 0:
            success += 1
        else:
            failed.append(f"{type_name}_{feature}_benchs")
    
    elapsed = time.time() - start_time
    
    # Resumen
    print()
    echo_header("=" * 60)
    echo_header("  RESUMEN DE COMPILACIÓN")
    echo_header("=" * 60)
    echo_success(f"Exitosas: {success}/{total}")
    
    if failed:
        echo_error(f"Fallidas: {len(failed)}")
        for item in failed:
            echo_error(f"  - {item}")
    
    echo_info(f"Tiempo total: {elapsed:.1f}s")
    
    return 0 if len(failed) == 0 else 1


def cmd_test(args: argparse.Namespace) -> int:
    """Compila y ejecuta todos los tests"""
    echo_header("=" * 60)
    echo_header("  EJECUCIÓN COMPLETA DE TESTS")
    echo_header("=" * 60)
    print()
    
    combinations = get_all_combinations()
    
    if not combinations:
        echo_error("No se encontraron archivos de tests")
        return 1
    
    total_tests = 0
    passed_tests = 0
    failed = []
    
    start_time = time.time()
    
    for i, (type_name, feature) in enumerate(combinations, 1):
        echo_header(f"\n[{i}/{len(combinations)}] {type_name}_{feature}")
        
        # Build
        echo_info("  Compilando...")
        ret = run_script("build_generic.py", [type_name, feature, "tests", args.compiler or "all", args.mode or "all"])
        
        if ret != 0:
            failed.append(f"{type_name}_{feature} (build failed)")
            continue
        
        # Check
        echo_info("  Ejecutando...")
        ret = run_script("check_generic.py", [type_name, feature, args.compiler or "all", args.mode or "all"])
        
        total_tests += 1
        if ret == 0:
            passed_tests += 1
        else:
            failed.append(f"{type_name}_{feature} (test failed)")
    
    elapsed = time.time() - start_time
    
    # Resumen
    print()
    echo_header("=" * 60)
    echo_header("  RESUMEN DE TESTS")
    echo_header("=" * 60)
    echo_success(f"Pasaron: {passed_tests}/{total_tests}")
    
    if failed:
        echo_error(f"Fallaron: {len(failed)}")
        for item in failed:
            echo_error(f"  - {item}")
    
    echo_info(f"Tiempo total: {elapsed:.1f}s")
    
    return 0 if len(failed) == 0 else 1


def cmd_bench(args: argparse.Namespace) -> int:
    """Compila y ejecuta todos los benchmarks"""
    echo_header("=" * 60)
    echo_header("  EJECUCIÓN COMPLETA DE BENCHMARKS")
    echo_header("=" * 60)
    print()
    
    combinations = get_all_combinations()
    
    if not combinations:
        echo_error("No se encontraron archivos de benchmarks")
        return 1
    
    total_benchs = 0
    success_benchs = 0
    failed = []
    
    start_time = time.time()
    
    for i, (type_name, feature) in enumerate(combinations, 1):
        echo_header(f"\n[{i}/{len(combinations)}] {type_name}_{feature}")
        
        # Build
        echo_info("  Compilando...")
        ret = run_script("build_generic.py", [type_name, feature, "benchs", args.compiler or "all", "release"])
        
        if ret != 0:
            failed.append(f"{type_name}_{feature} (build failed)")
            continue
        
        # Run
        echo_info("  Ejecutando...")
        ret = run_script("run_generic.py", [type_name, feature, args.compiler or "all", "release"])
        
        total_benchs += 1
        if ret == 0:
            success_benchs += 1
        else:
            failed.append(f"{type_name}_{feature} (benchmark failed)")
    
    elapsed = time.time() - start_time
    
    # Resumen
    print()
    echo_header("=" * 60)
    echo_header("  RESUMEN DE BENCHMARKS")
    echo_header("=" * 60)
    echo_success(f"Exitosos: {success_benchs}/{total_benchs}")
    
    if failed:
        echo_error(f"Fallaron: {len(failed)}")
        for item in failed:
            echo_error(f"  - {item}")
    
    echo_info(f"Tiempo total: {elapsed:.1f}s")
    
    return 0 if len(failed) == 0 else 1


def cmd_list(args: argparse.Namespace) -> int:
    """Lista todas las combinaciones disponibles"""
    combinations = get_all_combinations()
    
    if not combinations:
        echo_error("No se encontraron archivos de tests/benchs")
        return 1
    
    echo_header("=" * 60)
    echo_header("  COMBINACIONES DISPONIBLES")
    echo_header("=" * 60)
    print()
    
    # Agrupar por tipo
    by_type: Dict[str, List[str]] = {}
    for type_name, feature in combinations:
        if type_name not in by_type:
            by_type[type_name] = []
        by_type[type_name].append(feature)
    
    for type_name in sorted(by_type.keys()):
        echo_info(f"{type_name}:")
        for feature in sorted(by_type[type_name]):
            print(f"  • {feature}")
        print()
    
    echo_header(f"Total: {len(combinations)} combinaciones")
    
    # Mostrar compiladores disponibles
    if COMPILER_ENVS_DIR.exists():
        configs = list(COMPILER_ENVS_DIR.glob("*_env.json"))
        if configs:
            print()
            echo_info("Compiladores configurados:")
            for config in sorted(configs):
                compiler = config.stem.replace("_env", "")
                print(f"  • {compiler}")
    
    return 0


def main():
    """Main entry point"""
    
    # Parser principal
    parser = argparse.ArgumentParser(
        description="Sistema de build centralizado para int128 project",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Ejemplos:
  python make.py init
  python make.py build uint128 bits tests gcc release
  python make.py check uint128 bits
  python make.py run uint128 cmath all release
  python make.py test
  python make.py all
  python make.py clean
  python make.py list
        """
    )
    
    subparsers = parser.add_subparsers(dest='command', help='Comando a ejecutar')
    
    # init
    subparsers.add_parser('init', help='Detecta y configura compiladores')
    
    # build
    build_parser = subparsers.add_parser('build', help='Compila tests o benchmarks')
    build_parser.add_argument('type', nargs='?', help='uint128 | int128')
    build_parser.add_argument('feature', nargs='?', help='bits | numeric | algorithm | etc.')
    build_parser.add_argument('target', nargs='?', help='tests | benchs')
    build_parser.add_argument('compiler', nargs='?', help='gcc | clang | intel | msvc | all')
    build_parser.add_argument('mode', nargs='?', help='debug | release | all')
    build_parser.add_argument('-v', '--verbose', action='store_true', help='Modo verbose')
    
    # check
    check_parser = subparsers.add_parser('check', help='Ejecuta tests')
    check_parser.add_argument('type', nargs='?', help='uint128 | int128')
    check_parser.add_argument('feature', nargs='?', help='bits | numeric | algorithm | etc.')
    check_parser.add_argument('compiler', nargs='?', help='gcc | clang | intel | msvc | all')
    check_parser.add_argument('mode', nargs='?', help='debug | release | all')
    
    # run
    run_parser = subparsers.add_parser('run', help='Ejecuta benchmarks')
    run_parser.add_argument('type', nargs='?', help='uint128 | int128')
    run_parser.add_argument('feature', nargs='?', help='bits | numeric | algorithm | etc.')
    run_parser.add_argument('compiler', nargs='?', help='gcc | clang | intel | msvc | all')
    run_parser.add_argument('mode', nargs='?', help='debug | release | all')
    
    # clean
    clean_parser = subparsers.add_parser('clean', help='Limpia directorios de build')
    clean_parser.add_argument('--all', action='store_true', help='Limpia también compiler_envs')
    
    # all
    all_parser = subparsers.add_parser('all', help='Compila todo (tests + benchs)')
    
    # test
    test_parser = subparsers.add_parser('test', help='Compila y ejecuta todos los tests')
    test_parser.add_argument('compiler', nargs='?', help='gcc | clang | intel | msvc | all')
    test_parser.add_argument('mode', nargs='?', help='debug | release | all')
    
    # bench
    bench_parser = subparsers.add_parser('bench', help='Compila y ejecuta todos los benchmarks')
    bench_parser.add_argument('compiler', nargs='?', help='gcc | clang | intel | msvc | all')
    
    # list
    subparsers.add_parser('list', help='Lista todas las combinaciones disponibles')
    
    # Parse argumentos
    args = parser.parse_args()
    
    if not args.command:
        parser.print_help()
        return 1
    
    # Ejecutar comando
    commands = {
        'init': cmd_init,
        'build': cmd_build,
        'check': cmd_check,
        'run': cmd_run,
        'clean': cmd_clean,
        'all': cmd_all,
        'test': cmd_test,
        'bench': cmd_bench,
        'list': cmd_list,
    }
    
    return commands[args.command](args)


if __name__ == "__main__":
    try:
        sys.exit(main())
    except KeyboardInterrupt:
        echo_warning("\nInterrumpido por el usuario")
        sys.exit(130)
    except Exception as e:
        echo_error(f"Error inesperado: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)
