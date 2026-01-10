#!/usr/bin/env python3
"""
Script para compilar todos los demos y verificar que compilan correctamente.
"""

import os
import subprocess
import sys
from pathlib import Path

DEMOS_DIRS = [
    "demos/tutorials",
    "demos/examples",
    "demos/showcase",
    "demos/general",
]

def compile_demo(filepath, output_dir):
    """Compile a single demo."""
    name = Path(filepath).stem
    output = output_dir / f"{name}.exe"
    
    cmd = [
        "g++",
        "-std=c++20",
        "-O2",
        "-Iinclude",
        "-pthread",
        str(filepath),
        "-o",
        str(output)
    ]
    
    result = subprocess.run(cmd, capture_output=True, text=True)
    
    if result.returncode == 0:
        return True, ""
    else:
        error = result.stderr[:500] if result.stderr else "Unknown error"
        return False, error

def main():
    project_root = Path(__file__).parent
    output_dir = project_root / "build" / "demos"
    output_dir.mkdir(parents=True, exist_ok=True)
    
    print("=" * 70)
    print("COMPILING ALL DEMOS")
    print("=" * 70)
    
    success_count = 0
    fail_count = 0
    failures = []
    
    for demo_dir in DEMOS_DIRS:
        full_path = project_root / demo_dir
        if not full_path.is_dir():
            continue
        
        print(f"\n[{demo_dir}]")
        
        for filename in sorted(full_path.iterdir()):
            if filename.suffix != ".cpp":
                continue
            
            ok, error = compile_demo(filename, output_dir)
            
            if ok:
                print(f"  [OK]   {filename.name}")
                success_count += 1
            else:
                print(f"  [FAIL] {filename.name}")
                fail_count += 1
                failures.append((str(filename), error))
    
    print("\n" + "=" * 70)
    print(f"RESULTS: {success_count} OK, {fail_count} FAILED")
    print("=" * 70)
    
    if failures:
        print("\nFAILURES:")
        for path, err in failures:
            print(f"\n  {path}:")
            # Print first error line only
            first_error = err.split('\n')[0] if err else "?"
            print(f"    {first_error}")
    
    return 0 if fail_count == 0 else 1

if __name__ == "__main__":
    sys.exit(main())
