#!/usr/bin/env python3
"""
Script para actualizar demos al nuevo formato de headers y constructores explícitos.

Cambios aplicados:
1. #include <int128.hpp> -> #include <int128_simple.hpp>
2. uint128_t var = value -> uint128_t var{value}
3. int128_t var = value -> int128_t var{value}
"""

import os
import re
import sys

DEMOS_DIRS = [
    "demos/tutorials",
    "demos/examples",
    "demos/showcase",
    "demos/general",
]

# Patterns to fix
PATTERNS = [
    # Include replacement - main header
    (r'#include\s*[<"]int128\.hpp[">]', '#include <int128_simple.hpp>'),
    (r'#include\s*[<"]uint128_t\.hpp[">]', '#include <int128_simple.hpp>'),
    (r'#include\s*[<"]int128_t\.hpp[">]', '#include <int128_simple.hpp>'),
    (r'#include\s*[<"]uint128/uint128_t\.hpp[">]', '#include <int128_simple.hpp>'),
    (r'#include\s*[<"]int128/int128_t\.hpp[">]', '#include <int128_simple.hpp>'),
    (r'#include\s*[<"]uint128\.hpp[">]', '#include <int128_simple.hpp>'),
    
    # Remove OLD includes (these are now in int128_simple.hpp or not needed)
    (r'#include\s*[<"]uint128/uint128_iostreams\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]int128/int128_iostreams\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]uint128/uint128_traits\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]int128/int128_traits\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]uint128/uint128_limits\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]int128/int128_limits\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]uint128/uint128_concepts\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]int128/int128_concepts\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]uint128/uint128_bits\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]int128/int128_bits\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]uint128/uint128_format\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]int128/int128_format\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]uint128/uint128_cmath\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]int128/int128_cmath\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]uint128/uint128_numeric\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]int128/int128_numeric\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]uint128/uint128_algorithm\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]int128/int128_algorithm\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]uint128/uint128_safe\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]int128/int128_safe\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]uint128/uint128_thread_safety\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]int128/int128_thread_safety\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]uint128/uint128_ranges\.hpp[">]\s*\n', ''),
    (r'#include\s*[<"]int128/int128_ranges\.hpp[">]\s*\n', ''),
    
    # Namespace literals - update to new namespace
    # OLD: using namespace uint128_literals; -> NEW: using namespace nstd::int128_literals;
    (r'using\s+namespace\s+uint128_literals\s*;', 'using namespace nstd::int128_literals;'),
    (r'using\s+namespace\s+int128_literals\s*;', 'using namespace nstd::int128_literals;'),
    # OLD: uint128_literals::xxx -> NEW: nstd::int128_literals::xxx
    (r'\buint128_literals::', 'nstd::int128_literals::'),
    (r'\bint128_literals::', 'nstd::int128_literals::'),
    # Also nstd::literals works (it imports int128_literals)
    (r'using\s+namespace\s+nstd::uint128_literals\s*;', 'using namespace nstd::int128_literals;'),
    
    # Namespace threadsafe - update to new namespace
    (r'\buint128_threadsafe::', 'nstd::'),
    (r'\bint128_threadsafe::', 'nstd::'),
    (r'using\s+namespace\s+uint128_threadsafe\s*;', 'using namespace nstd;'),
    (r'using\s+namespace\s+int128_threadsafe\s*;', 'using namespace nstd;'),
    
    # Constructor syntax: uint128_t var = value -> uint128_t var{value}
    # Match: type variable_name = numeric_value (but not initializer lists)
    (r'\b(uint128_t\s+)(\w+)\s*=\s*(0[xXbB]?[\da-fA-F\'_]+)\s*;', r'\1\2{\3};'),
    (r'\b(int128_t\s+)(\w+)\s*=\s*(-?0[xXbB]?[\da-fA-F\'_]+)\s*;', r'\1\2{\3};'),
    (r'\b(uint128_t\s+)(\w+)\s*=\s*(\d+)\s*;', r'\1\2{\3};'),
    (r'\b(int128_t\s+)(\w+)\s*=\s*(-?\d+)\s*;', r'\1\2{\3};'),
    
    # Also handle constructor with cast: uint128_t var = uint128_t(value)
    (r'\b(uint128_t\s+)(\w+)\s*=\s*uint128_t\(([^)]+)\)\s*;', r'\1\2{uint128_t{\3}};'),
    (r'\b(int128_t\s+)(\w+)\s*=\s*int128_t\(([^)]+)\)\s*;', r'\1\2{int128_t{\3}};'),
    
    # Handle const declarations: const uint128_t FLAG = value
    (r'\b(const\s+uint128_t\s+)(\w+)\s*=\s*(0[xXbB]?[\da-fA-F\'_]+)\s*;', r'\1\2{\3};'),
    (r'\b(const\s+int128_t\s+)(\w+)\s*=\s*(-?0[xXbB]?[\da-fA-F\'_]+)\s*;', r'\1\2{\3};'),
    (r'\b(const\s+uint128_t\s+)(\w+)\s*=\s*(\d+)\s*;', r'\1\2{\3};'),
    (r'\b(const\s+int128_t\s+)(\w+)\s*=\s*(-?\d+)\s*;', r'\1\2{\3};'),
    
    # Handle bit shift expressions: const uint128_t FLAG = 1 << N
    (r'\b(const\s+uint128_t\s+)(\w+)\s*=\s*(\d+)\s*(<<|>>)\s*(\d+)\s*;', r'\1\2{uint128_t{\3} \4 \5};'),
    (r'\b(uint128_t\s+)(\w+)\s*=\s*(\d+)\s*(<<|>>)\s*(\d+)\s*;', r'\1\2{uint128_t{\3} \4 \5};'),
]

def fix_file(filepath):
    """Fix a single file."""
    try:
        with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
    except Exception as e:
        print(f"  [SKIP] Cannot read: {e}")
        return False
    
    original = content
    
    for pattern, replacement in PATTERNS:
        content = re.sub(pattern, replacement, content)
    
    if content != original:
        try:
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(content)
            return True
        except Exception as e:
            print(f"  [ERROR] Cannot write: {e}")
            return False
    
    return False

def main():
    """Main entry point."""
    project_root = os.path.dirname(os.path.abspath(__file__))
    
    print("=" * 60)
    print("Fixing demos for int128 library")
    print("=" * 60)
    
    fixed_count = 0
    skipped_count = 0
    
    for demo_dir in DEMOS_DIRS:
        full_path = os.path.join(project_root, demo_dir)
        if not os.path.isdir(full_path):
            print(f"\n[SKIP] Directory not found: {demo_dir}")
            continue
        
        print(f"\n[DIR] {demo_dir}/")
        
        for filename in sorted(os.listdir(full_path)):
            if not filename.endswith('.cpp'):
                continue
            
            filepath = os.path.join(full_path, filename)
            print(f"  Processing: {filename}...", end=" ")
            
            if fix_file(filepath):
                print("[FIXED]")
                fixed_count += 1
            else:
                print("[OK/SKIP]")
                skipped_count += 1
    
    print("\n" + "=" * 60)
    print(f"Summary: {fixed_count} files fixed, {skipped_count} unchanged/skipped")
    print("=" * 60)
    
    return 0

if __name__ == "__main__":
    sys.exit(main())
