#!/usr/bin/env python3
"""
Script para corregir warnings de variables no usadas agregando [[maybe_unused]]
"""
import re
import sys

def fix_file(filepath, patterns):
    """Agrega [[maybe_unused]] a las líneas que coinciden con los patrones"""
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()
    
    original_content = content
    changes_made = 0
    
    for pattern in patterns:
        # Buscar líneas que coincidan y que NO tengan ya [[maybe_unused]]
        matches = re.finditer(pattern, content, re.MULTILINE)
        replacements = []
        
        for match in matches:
            full_match = match.group(0)
            # Si ya tiene [[maybe_unused]], skip
            if '[[maybe_unused]]' in full_match:
                continue
            
            # Extraer la indentación
            indent_match = re.match(r'^(\s*)', full_match)
            indent = indent_match.group(1) if indent_match else ''
            
            # Agregar [[maybe_unused]] después de la indentación
            rest_of_line = full_match[len(indent):]
            new_line = f'{indent}[[maybe_unused]] {rest_of_line}'
            
            replacements.append((full_match, new_line))
        
        # Aplicar reemplazos
        for old, new in replacements:
            if old in content:
                content = content.replace(old, new, 1)
                changes_made += 1
    
    if content != original_content:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"✓ {filepath}: {changes_made} cambios aplicados")
        return True
    else:
        print(f"- {filepath}: Sin cambios")
        return False

def main():
    # Patrones para uint128_t_extracted_tests.cpp
    uint128_patterns = [
        # Variables 'val' no usadas
        r'^\s+uint128_t val(_\w+)?;$',
        r'^\s+uint128_t val(_\w+)?\([^)]+\);$',
        r'^\s+uint128_t val\d? = .+;$',
        r'^\s+auto val\d* = .+;$',
        
        # Variables 'res' no usadas
        r'^\s+auto res\d* = .+\.divrem\(.+\);$',
        r'^\s+auto res_[iu](64|32|16|8)(_\d+)? = .+\.divrem\(.+\);$',
        
        # Variables 'check' no usadas
        r'^\s+auto check = .+\.divrem\(.+\);$',
        r'^\s+auto check\d+ = .+\.divrem\(.+\);$',
        r'^\s+auto check[123]_[iu](64|32|16|8) = .+\.divrem\(.+\);$',
        
        # Variables 'div_result' no usadas
        r'^\s+auto div_result_[ab] = .+\.divrem\(.+\);$',
        
        # Otras variables
        r'^\s+uint128_t (result\d*|left|right|shifted|mult_result|shift\d+r?|power\d+|almost_power\d+|moved|from_temp) = .+;$',
        r'^\s+bool (real_less|real_eq) = .+;$',
        r'^\s+uint64_t (mult|orig_high|orig_low) = .+;$',
        r'^\s+uint128_t (zero_copy|max_copy|moved|from_temp|zero|one|max_val|max64|large|val2) ?(\([^)]+\))?;$',
        r'^\s+uint128_t (val_hex|val_large|val_dec|val_oct|val_bin|hex|dec|bin|oct|max|power64|almost_power64|almost_max) ?(\([^)]+\))?;$',
        r'^\s+auto (hex|dec|bin|oct|zero|max64|val|val2|res\d*|b2_\w+) = .+;$',
        r'^\s+uint128_t (a|b|zero|one|max_val|h1|h2|hl1|hl2|m1|m2|max_low|min_high|max_u128|almost_max) ?(\([^)]+\))?;$',
        r'^\s+uint128_t (pos_small|pos_large|neg_small|neg_large|min_val|max_val) ?(\([^)]+\))?;$',
        
        # Variables native
        r'^\s+__uint128_t native(_u)? = .+;$',
    ]
    
    # Patrones para int128_t_extracted_tests.cpp
    int128_patterns = [
        # Variables result y expected
        r'^\s+int128_t result = .+;$',
        r'^\s+int64_t expected = .+;$',
        r'^\s+int128_t val;$',
        r'^\s+int128_t& (result|old) = .+;$',
        r'^\s+int128_t old = .+;$',
        
        # Variables en tests de comparación
        r'^\s+int128_t ([a-z]|[a-z]\d+|pos_\w+|neg_\w+|min_val|max_val)(\([^)]*\))?;$',
    ]
    
    # Aplicar correcciones
    files_fixed = 0
    
    print("Corrigiendo uint128_t_extracted_tests.cpp...")
    if fix_file('tests/uint128_t_extracted_tests.cpp', uint128_patterns):
        files_fixed += 1
    
    print("\nCorrigiendo int128_t_extracted_tests.cpp...")
    if fix_file('tests/int128_t_extracted_tests.cpp', int128_patterns):
        files_fixed += 1
    
    print(f"\n✓ Corrección completada: {files_fixed} archivos modificados")

if __name__ == '__main__':
    main()
