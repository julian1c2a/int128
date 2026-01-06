#!/usr/bin/env python3
"""
Script para reemplazar caracteres Unicode de caja por ASCII
para mejor compatibilidad con terminales Windows.
"""

import os
import glob

# Mapeo de caracteres Unicode a ASCII
REPLACEMENTS = {
    '╔': '+',
    '╗': '+',
    '╚': '+',
    '╝': '+',
    '║': '|',
    '═': '=',
    '╠': '+',
    '╣': '+',
    '╦': '+',
    '╩': '+',
    '╬': '+',
    '─': '-',
    '│': '|',
    '┌': '+',
    '┐': '+',
    '└': '+',
    '┘': '+',
    '├': '+',
    '┤': '+',
    '┬': '+',
    '┴': '+',
    '┼': '+',
    '✓': 'OK',
    '✗': 'FAIL',
    # Tildes españolas
    'á': 'a',
    'é': 'e',
    'í': 'i',
    'ó': 'o',
    'ú': 'u',
    'Á': 'A',
    'É': 'E',
    'Í': 'I',
    'Ó': 'O',
    'Ú': 'U',
    'ñ': 'n',
    'Ñ': 'N',
    'ü': 'u',
    'Ü': 'U',
}

def fix_file(filepath):
    """Reemplaza caracteres Unicode en un archivo."""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
    except UnicodeDecodeError:
        print(f"  [SKIP] {filepath} - encoding error")
        return False
    
    original = content
    for unicode_char, ascii_char in REPLACEMENTS.items():
        content = content.replace(unicode_char, ascii_char)
    
    if content != original:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        return True
    return False

def main():
    # Buscar archivos de test
    test_files = glob.glob('test_*.cpp')
    
    print(f"Encontrados {len(test_files)} archivos de test")
    print("=" * 50)
    
    modified = 0
    for filepath in sorted(test_files):
        if fix_file(filepath):
            print(f"  [MODIFIED] {filepath}")
            modified += 1
        else:
            print(f"  [OK] {filepath}")
    
    print("=" * 50)
    print(f"Archivos modificados: {modified}/{len(test_files)}")

if __name__ == '__main__':
    main()
