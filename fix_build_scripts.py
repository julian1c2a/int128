#!/usr/bin/env python3
"""
Script para corregir la verificación de errores de compilación en los scripts de build.
Agrega verificación de exit code y filtra warnings de linkado innecesarios.
"""

import re
from pathlib import Path

PROJECT_ROOT = Path(r"c:\msys64\ucrt64\home\julian\CppProjects\int128")
SCRIPTS_DIR = PROJECT_ROOT / "scripts"

# Scripts a modificar
SCRIPTS_TO_FIX = [
    "build_int128_t_extracted_tests.bash",
    "build_uint128_t_extracted_benchs.bash",
    "build_int128_t_extracted_benchs.bash",
]

def fix_clang_section(content):
    """Agrega verificación de exit code para Clang y filtra warnings."""
    
    # Pattern for both Debug and Release
    # Match: clang++ ... -o file.exe"
    #        echo "✅ Clang Debug/Release OK"
    pattern = r'(clang\+\+ "\$SOURCE_FILE" \$INCLUDE_DIR \$COMMON_FLAGS (-g -O0|-O3 -DNDEBUG) \\\n\s+-o "\$PROJECT_ROOT/build/build_\w+/clang/(debug|release)/\w+\.exe")\n\s+echo "   ✅ Clang (Debug|Release) OK"'
    
    def replacement(match):
        compile_cmd = match.group(1)
        mode = match.group(4)  # Debug or Release
        return f'{compile_cmd} 2>&1 | grep -v "LINK : warning LNK" | grep -v "^LINK : "; then\n                echo "   ✅ Clang {mode} OK"\n            else\n                echo "   ❌ Clang {mode} FAILED"\n            fi'
    
    # Need to add "if" before the command
    pattern2 = r'(mkdir -p "\$PROJECT_ROOT/build/build_\w+/clang/(debug|release)")\n\s+(clang\+\+ "\$SOURCE_FILE".*?-o "\$PROJECT_ROOT/build/build_\w+/clang/\2/\w+\.exe" 2>&1.*?fi)'
    
    def replacement2(match):
        mkdir = match.group(1)
        rest = match.group(3)
        return f'{mkdir}\n            if {rest}'
    
    content = re.sub(pattern2, replacement2, content, flags=re.DOTALL)
    
    return content

def fix_intel_section(content):
    """Remueve redirección a /dev/null y agrega verificación de exit code para Intel."""
    
    # Pattern para Debug
    pattern_debug = r'("\$INTEL_CMD" "\$WIN_SOURCE" -I"\$WIN_INCLUDE" \$ICX_FLAGS_BASE /Zi /Od \\\n\s+"/Fe:\$WIN_OUT_DEBUG") > /dev/null\n\s+if \[ \$\? -eq 0 \]; then echo "   ✅ Intel Debug OK"; else echo "   ❌ Intel Debug FAILED"; fi'
    replacement_debug = r'\1 2>&1 | grep -v "LINK : warning LNK"\n            if [ $? -eq 0 ]; then echo "   ✅ Intel Debug OK"; else echo "   ❌ Intel Debug FAILED"; fi'
    content = re.sub(pattern_debug, replacement_debug, content)
    
    # Pattern para Release
    pattern_release = r'("\$INTEL_CMD" "\$WIN_SOURCE" -I"\$WIN_INCLUDE" \$ICX_FLAGS_BASE /O2 /DNDEBUG \\\n\s+"/Fe:\$WIN_OUT_RELEASE") > /dev/null\n\s+if \[ \$\? -eq 0 \]; then echo "   ✅ Intel Release OK"; else echo "   ❌ Intel Release FAILED"; fi'
    replacement_release = r'\1 2>&1 | grep -v "LINK : warning LNK"\n            if [ $? -eq 0 ]; then echo "   ✅ Intel Release OK"; else echo "   ❌ Intel Release FAILED"; fi'
    content = re.sub(pattern_release, replacement_release, content)
    
    return content

def fix_msvc_section(content):
    """Remueve redirección a /dev/null y agrega filtros para MSVC."""
    
    # Pattern para Debug
    pattern_debug = r'(cl\.exe "\$WIN_SOURCE" /I"\$WIN_INCLUDE" \$MSVC_FLAGS_BASE /Zi /Od \\\n\s+"/Fe:\$WIN_OUT_DEBUG") > /dev/null 2>&1\n\s+if \[ \$\? -eq 0 \]; then echo "   ✅ MSVC Debug OK"; else echo "   ❌ MSVC Debug FAILED"; fi'
    replacement_debug = r'\1 2>&1 | grep -v "LINK : warning LNK" | grep -v "\.cpp$"\n            if [ $? -eq 0 ]; then echo "   ✅ MSVC Debug OK"; else echo "   ❌ MSVC Debug FAILED"; fi'
    content = re.sub(pattern_debug, replacement_debug, content)
    
    # Pattern para Release
    pattern_release = r'(cl\.exe "\$WIN_SOURCE" /I"\$WIN_INCLUDE" \$MSVC_FLAGS_BASE /O2 /DNDEBUG \\\n\s+"/Fe:\$WIN_OUT_RELEASE") > /dev/null 2>&1\n\s+if \[ \$\? -eq 0 \]; then echo "   ✅ MSVC Release OK"; else echo "   ❌ MSVC Release FAILED"; fi'
    replacement_release = r'\1 2>&1 | grep -v "LINK : warning LNK" | grep -v "\.cpp$"\n            if [ $? -eq 0 ]; then echo "   ✅ MSVC Release OK"; else echo "   ❌ MSVC Release FAILED"; fi'
    content = re.sub(pattern_release, replacement_release, content)
    
    return content

def fix_script(script_path):
    """Aplica todas las correcciones a un script."""
    print(f"Procesando {script_path.name}...")
    
    with open(script_path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    original_content = content
    
    # Aplicar correcciones
    content = fix_clang_section(content)
    content = fix_intel_section(content)
    content = fix_msvc_section(content)
    
    if content != original_content:
        with open(script_path, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"   ✓ {script_path.name}: Cambios aplicados")
        return True
    else:
        print(f"   ℹ {script_path.name}: Sin cambios")
        return False

def main():
    print("=" * 50)
    print("Corrigiendo scripts de build...")
    print("=" * 50)
    
    fixed_count = 0
    for script_name in SCRIPTS_TO_FIX:
        script_path = SCRIPTS_DIR / script_name
        if script_path.exists():
            if fix_script(script_path):
                fixed_count += 1
        else:
            print(f"   ⚠ {script_name}: No encontrado")
    
    print()
    print("=" * 50)
    print(f"✅ Completado: {fixed_count}/{len(SCRIPTS_TO_FIX)} scripts modificados")
    print("=" * 50)

if __name__ == "__main__":
    main()
