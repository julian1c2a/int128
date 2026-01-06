#!/usr/bin/env python3
"""
Refactoriza data[0] → data[LSULL] y data[1] → data[MSULL]
en las secciones restantes de int128_base.hpp.
"""

import re
from pathlib import Path

def refactor_file(filepath: Path):
    content = filepath.read_text(encoding='utf-8')
    
    # Pattern 1: data[0] → data[LSULL]
    content = re.sub(r'\bdata\[0\]', 'data[LSULL]', content)
    
    # Pattern 2: data[1] → data[MSULL]
    content = re.sub(r'\bdata\[1\]', 'data[MSULL]', content)
    
    # Pattern 3: other.data[0] → other.data[LSULL]
    content = re.sub(r'\bother\.data\[0\]', 'other.data[LSULL]', content)
    
    # Pattern 4: other.data[1] → other.data[MSULL]
    content = re.sub(r'\bother\.data\[1\]', 'other.data[MSULL]', content)
    
    filepath.write_text(content, encoding='utf-8')
    print(f"✓ Refactored: {filepath}")

if __name__ == "__main__":
    target = Path("include_new/int128_base.hpp")
    if target.exists():
        refactor_file(target)
    else:
        print(f"Error: File not found: {target}")
EO