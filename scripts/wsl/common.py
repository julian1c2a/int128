#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# =============================================================================
# int128 Library - 128-bit Integer Types for C++20
# =============================================================================
#
# SPDX-License-Identifier: BSL-1.0
# Copyright (c) 2024-2026 Julián Calderón Almendros
#
# WSL Common Functions - Shared by all compiler scripts (Python version)
# =============================================================================

"""
WSL Common Module - Shared functions for all WSL compiler scripts.

This module provides:
- Color output functions
- Compiler detection and validation
- Build and test execution
- Feature validation

Usage:
    from common import (
        VALID_FEATURES, VALID_MODES,
        echo_info, echo_success, echo_error,
        check_compiler, get_source_file, build_and_test
    )
"""

import os
import sys
import subprocess
import shutil
from pathlib import Path
from typing import Optional, List, Tuple, Dict

# =============================================================================
# Force UTF-8 encoding
# =============================================================================
if sys.platform == "win32":
    import io
    sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8', errors='replace')
    sys.stderr = io.TextIOWrapper(sys.stderr.buffer, encoding='utf-8', errors='replace')

# =============================================================================
# Colors (ANSI codes for terminal output)
# =============================================================================
class Colors:
    RED = '\033[0;31m'
    GREEN = '\033[0;32m'
    YELLOW = '\033[1;33m'
    BLUE = '\033[0;34m'
    CYAN = '\033[0;36m'
    NC = '\033[0m'  # No Color

def echo_info(msg: str) -> None:
    """Print info message in cyan."""
    print(f"{Colors.CYAN}[INFO]{Colors.NC} {msg}")

def echo_success(msg: str) -> None:
    """Print success message in green."""
    print(f"{Colors.GREEN}[OK]{Colors.NC}   {msg}")

def echo_error(msg: str) -> None:
    """Print error message in red."""
    print(f"{Colors.RED}[FAIL]{Colors.NC} {msg}")

def echo_warn(msg: str) -> None:
    """Print warning message in yellow."""
    print(f"{Colors.YELLOW}[WARN]{Colors.NC} {msg}")

def echo_header(msg: str) -> None:
    """Print header message in blue."""
    print(f"{Colors.BLUE}{msg}{Colors.NC}")

# =============================================================================
# Paths
# =============================================================================
def get_project_root() -> Path:
    """Get project root directory."""
    # This file is in scripts/wsl/, so project root is ../../
    return Path(__file__).parent.parent.parent.resolve()

PROJECT_ROOT = get_project_root()
INCLUDE_DIR = PROJECT_ROOT / "include"
BUILD_BASE = PROJECT_ROOT / "build" / "build_tests"

# =============================================================================
# Valid values (Template unificado int128_base_t)
# =============================================================================
VALID_FEATURES = [
    "tt", "t", "algorithm", "bits", "cmath", "concepts", 
    "format", "iostreams", "limits", "numeric", "ranges", 
    "safe", "thread_safety", "traits"
]

VALID_MODES = ["debug", "release", "all"]

VALID_COMPILERS = [
    "gcc-13", "gcc-14", "gcc-15",
    "clang-18", "clang-19", "clang-20", "clang-21",
    "icpx", "all"
]

# Compiler command mapping
COMPILER_COMMANDS: Dict[str, str] = {
    "gcc-13": "g++-13",
    "gcc-14": "g++-14", 
    "gcc-15": "g++-15",
    "clang-18": "clang++-18",
    "clang-19": "clang++-19",
    "clang-20": "clang++-20",
    "clang-21": "clang++-21",
    "icpx": "icpx",
}

# =============================================================================
# Validation Functions
# =============================================================================
def validate_feature(feature: str) -> bool:
    """Validate that feature is in the allowed list."""
    return feature in VALID_FEATURES

def validate_mode(mode: str) -> bool:
    """Validate that mode is in the allowed list."""
    return mode in VALID_MODES

def validate_compiler(compiler: str) -> bool:
    """Validate that compiler is in the allowed list."""
    return compiler in VALID_COMPILERS

# =============================================================================
# Compiler Functions
# =============================================================================
def check_compiler(cmd: str, name: str) -> bool:
    """
    Check if compiler exists in PATH.
    
    Args:
        cmd: Compiler command (e.g., 'g++-15')
        name: Compiler name for display (e.g., 'gcc-15')
        
    Returns:
        True if compiler exists, False otherwise
    """
    if shutil.which(cmd) is not None:
        return True
    return False

def get_compiler_version(cmd: str) -> Optional[str]:
    """Get compiler version string."""
    try:
        result = subprocess.run(
            [cmd, "--version"],
            capture_output=True,
            text=True,
            timeout=10
        )
        if result.returncode == 0:
            # Return first line of version output
            return result.stdout.strip().split('\n')[0]
    except (subprocess.TimeoutExpired, FileNotFoundError, OSError):
        pass
    return None

def detect_available_compilers() -> Dict[str, str]:
    """
    Detect all available compilers in the system.
    
    Returns:
        Dict mapping compiler name to command (only available compilers)
    """
    available = {}
    
    for name, cmd in COMPILER_COMMANDS.items():
        if check_compiler(cmd, name):
            version = get_compiler_version(cmd)
            if version:
                echo_success(f"{name}: {version}")
                available[name] = cmd
    
    return available

# =============================================================================
# Source File Functions
# =============================================================================
def get_source_file(feature: str) -> Path:
    """
    Get source file path for a feature.
    
    With the unified template, all tests use the int128_ prefix.
    
    Args:
        feature: Feature name (e.g., 'tt', 'bits', 'cmath')
        
    Returns:
        Path to source file
    """
    return PROJECT_ROOT / "tests" / f"int128_{feature}_extracted_tests.cpp"

# =============================================================================
# Build and Test Function
# =============================================================================
def build_and_test(
    compiler_name: str,
    compiler_cmd: str,
    feature: str,
    mode: str = "all"
) -> Tuple[int, int]:
    """
    Build and test with specified compiler.
    
    Args:
        compiler_name: Compiler name (e.g., 'gcc-15')
        compiler_cmd: Compiler command (e.g., 'g++-15')
        feature: Feature to test (e.g., 'tt', 'bits')
        mode: Build mode ('debug', 'release', or 'all')
        
    Returns:
        Tuple of (passed_count, failed_count)
    """
    source_file = get_source_file(feature)
    
    if not source_file.exists():
        echo_error(f"Source file not found: {source_file}")
        return (0, 1)
    
    # Determine modes to run
    if mode == "all":
        modes_to_run = ["debug", "release"]
    else:
        modes_to_run = [mode]
    
    passed = 0
    failed = 0
    
    for m in modes_to_run:
        echo_header(f"--- {compiler_name} [{m}] int128_{feature} ---")
        
        # Create output directory
        output_dir = BUILD_BASE / compiler_name / m
        output_dir.mkdir(parents=True, exist_ok=True)
        output = output_dir / f"int128_{feature}_tests_{compiler_name}"
        
        # Base flags
        common_flags = [
            "-std=c++20",
            f"-I{INCLUDE_DIR}",
            "-Wall", "-Wextra", "-Wpedantic"
        ]
        
        # Mode-specific flags
        if m == "debug":
            mode_flags = ["-O0", "-g"]
        else:  # release
            mode_flags = ["-O2", "-DNDEBUG"]
        
        # Extra flags for thread_safety
        extra_flags = []
        if feature == "thread_safety":
            extra_flags.append("-pthread")
            # GCC/Clang may need -latomic
            if compiler_cmd.startswith("g++") or compiler_cmd.startswith("clang++"):
                extra_flags.append("-latomic")
        
        # Build command
        cmd = [
            compiler_cmd,
            *common_flags,
            *mode_flags,
            str(source_file),
            "-o", str(output),
            *extra_flags
        ]
        
        echo_info("Compiling...")
        
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=120
            )
            
            if result.returncode != 0:
                echo_error(f"Compilation failed")
                print(result.stderr)
                failed += 1
                continue
            
            echo_success("Compilation OK")
            
        except subprocess.TimeoutExpired:
            echo_error("Compilation timeout (120s)")
            failed += 1
            continue
        except Exception as e:
            echo_error(f"Compilation error: {e}")
            failed += 1
            continue
        
        # Run tests
        echo_info("Running tests...")
        
        try:
            result = subprocess.run(
                [str(output)],
                capture_output=True,
                text=True,
                timeout=60
            )
            
            # Print test output
            print(result.stdout)
            
            if result.returncode == 0:
                echo_success("Tests PASSED")
                passed += 1
            else:
                echo_error("Tests FAILED")
                if result.stderr:
                    print(result.stderr)
                failed += 1
                
        except subprocess.TimeoutExpired:
            echo_error("Test execution timeout (60s)")
            failed += 1
        except Exception as e:
            echo_error(f"Test execution error: {e}")
            failed += 1
    
    return (passed, failed)

# =============================================================================
# Utility Functions
# =============================================================================
def print_summary(results: Dict[str, Dict[str, str]], feature: str) -> Tuple[int, int]:
    """
    Print summary table of results.
    
    Args:
        results: Dict mapping "compiler_mode" to "PASS"/"FAIL"
        feature: Feature name for title
        
    Returns:
        Tuple of (total_passed, total_count)
    """
    echo_header("=" * 60)
    echo_header(f"  SUMMARY - int128_{feature}")
    echo_header("=" * 60)
    
    # Group by compiler
    compilers = {}
    for key, status in results.items():
        parts = key.rsplit('_', 1)
        if len(parts) == 2:
            compiler, mode = parts
            if compiler not in compilers:
                compilers[compiler] = {}
            compilers[compiler][mode] = status
    
    # Print header
    print(f"\n{'Compiler':<15} {'debug':<12} {'release':<12}")
    print("-" * 15 + " " + "-" * 12 + " " + "-" * 12)
    
    passed = 0
    total = 0
    
    # Print each compiler row
    for compiler in sorted(compilers.keys()):
        modes = compilers[compiler]
        debug_status = modes.get('debug', '-')
        release_status = modes.get('release', '-')
        
        print(f"{compiler:<15} {debug_status:<12} {release_status:<12}")
        
        for status in modes.values():
            total += 1
            if status == "PASS":
                passed += 1
    
    print(f"\nTotal: {passed}/{total} passed")
    
    return (passed, total)


if __name__ == "__main__":
    # Self-test: detect compilers
    print("WSL Common Module - Self Test")
    print("=" * 40)
    print(f"Project root: {PROJECT_ROOT}")
    print(f"Include dir: {INCLUDE_DIR}")
    print(f"Valid features: {VALID_FEATURES}")
    print()
    echo_info("Detecting available compilers...")
    available = detect_available_compilers()
    print(f"\nFound {len(available)} compilers")
