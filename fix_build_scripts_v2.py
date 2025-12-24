#!/usr/bin/env python3
"""
Script para corregir la lógica de verificación en los scripts de build.
Cambia de usar 'if comando | grep; then' a 'comando | grep || true' + verificación de archivo.
"""

import re
from pathlib import Path

PROJECT_ROOT = Path(r"c:\msys64\ucrt64\home\julian\CppProjects\int128")
SCRIPTS_DIR = PROJECT_ROOT / "scripts"

SCRIPTS_TO_FIX = [
    "build_int128_t_extracted_tests.bash",
    "build_uint128_t_extracted_benchs.bash",
    "build_int128_t_extracted_benchs.bash",
]

def fix_compiler_section(content, compiler, build_type):
    """
    Reemplaza la estructura:
        if COMANDO | grep...; then
            echo OK
        else
            echo FAILED
        fi
    
    Por:
        COMANDO | grep... || true
        if [ -f OUTPUT_FILE ]; then
            echo OK
        else
            echo FAILED
        fi
    """
    
    # Pattern para Clang
    if compiler == "clang":
        pattern = (
            r'if clang\+\+ ("\$SOURCE_FILE" \$INCLUDE_DIR \$COMMON_FLAGS .*? \\\n\s+'
            r'-o "\$PROJECT_ROOT/build/build_(tests|benchs)/clang/(debug|release)/\w+\.exe") '
            r'2>&1 \| grep[^;]+; then\n\s+'
            r'echo "   ✅ Clang (Debug|Release) OK"\n\s+'
            r'else\n\s+'
            r'echo "   ❌ Clang \4 FAILED"\n\s+'
            r'fi'
        )
        
        def replacement(m):
            cmd = m.group(1)
            build_dir = m.group(2)  # tests or benchs
            mode = m.group(3)  # debug or release
            mode_cap = m.group(4)  # Debug or Release
            
            return (
                f'clang++ {cmd} 2>&1 | grep -v "LINK : warning LNK" | grep -v "^LINK : " || true\n'
                f'            if [ -f "$PROJECT_ROOT/build/build_{build_dir}/clang/{mode}/'
                f'{m.group(0).split("/")[-1].split(".")[0]}.exe" ]; then\n'
                f'                echo "   ✅ Clang {mode_cap} OK"\n'
                f'            else\n'
                f'                echo "   ❌ Clang {mode_cap} FAILED"\n'
                f'            fi'
            )
        
        content = re.sub(pattern, replacement, content, flags=re.DOTALL)
    
    # Pattern para Intel
    elif compiler == "intel":
        pattern = (
            r'if "\$INTEL_CMD" "\$WIN_SOURCE" -I"\$WIN_INCLUDE" \$ICX_FLAGS_BASE (.*?) \\\n\s+'
            r'"/Fe:\$WIN_OUT_(DEBUG|RELEASE)" 2>&1 \| grep[^;]+; then\n\s+'
            r'echo "   ✅ Intel (Debug|Release) OK"\n\s+'
            r'else\n\s+'
            r'echo "   ❌ Intel \3 FAILED"\n\s+'
            r'fi'
        )
        
        def replacement(m):
            flags = m.group(1)
            mode_var = m.group(2)  # DEBUG or RELEASE
            mode_cap = m.group(3)  # Debug or Release
            mode_lower = mode_cap.lower()
            
            # Determinar el tipo de build (tests o benchs) desde el contexto
            if "build_tests" in content:
                build_dir = "tests"
            else:
                build_dir = "benchs"
            
            return (
                f'"$INTEL_CMD" "$WIN_SOURCE" -I"$WIN_INCLUDE" $ICX_FLAGS_BASE {flags} \\\n'
                f'                "/Fe:$WIN_OUT_{mode_var}" 2>&1 | grep -v "LINK : warning LNK" | grep -v "^Intel" || true\n'
                f'            if [ -f "$PROJECT_ROOT/build/build_{build_dir}/intel/{mode_lower}/'
                f'{content.split("SOURCE_FILE=")[1].split("/")[-1].split(".")[0].replace("tests","").replace("benchs","")}_extracted_{build_dir}.exe" ]; then\n'
                f'                echo "   ✅ Intel {mode_cap} OK"\n'
                f'            else\n'
                f'                echo "   ❌ Intel {mode_cap} FAILED"\n'
                f'            fi'
            )
        
        content = re.sub(pattern, replacement, content, flags=re.DOTALL)
    
    # Pattern para MSVC
    elif compiler == "msvc":
        pattern = (
            r'if cl\.exe "\$WIN_SOURCE" /I"\$WIN_INCLUDE" \$MSVC_FLAGS_BASE (.*?) \\\n\s+'
            r'"/Fe:\$WIN_OUT_(DEBUG|RELEASE)" 2>&1 \| grep[^;]+; then\n\s+'
            r'echo "   ✅ MSVC (Debug|Release) OK"\n\s+'
            r'else\n\s+'
            r'echo "   ❌ MSVC \3 FAILED"\n\s+'
            r'fi'
        )
        
        def replacement(m):
            flags = m.group(1)
            mode_var = m.group(2)  # DEBUG or RELEASE
            mode_cap = m.group(3)  # Debug or Release
            mode_lower = mode_cap.lower()
            
            # Determinar el tipo de build desde el contexto
            if "build_tests" in content:
                build_dir = "tests"
            else:
                build_dir = "benchs"
            
            return (
                f'cl.exe "$WIN_SOURCE" /I"$WIN_INCLUDE" $MSVC_FLAGS_BASE {flags} \\\n'
                f'                "/Fe:$WIN_OUT_{mode_var}" 2>&1 | grep -v "LINK : warning LNK" | grep -v "\\.cpp$" || true\n'
                f'            if [ -f "$PROJECT_ROOT/build/build_{build_dir}/msvc/{mode_lower}/'
                f'{content.split("SOURCE_FILE=")[1].split("/")[-1].split(".")[0].replace("tests","").replace("benchs","")}_extracted_{build_dir}.exe" ]; then\n'
                f'                echo "   ✅ MSVC {mode_cap} OK"\n'
                f'            else\n'
                f'                echo "   ❌ MSVC {mode_cap} FAILED"\n'
                f'            fi'
            )
        
        content = re.sub(pattern, replacement, content, flags=re.DOTALL)
    
    return content

def fix_script_simple(script_path):
    """Aplica correcciones de forma más simple y directa."""
    print(f"Procesando {script_path.name}...")
    
    with open(script_path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    original_content = content
    
    # Aplicar correcciones para cada compilador
    for compiler in ['clang', 'intel', 'msvc']:
        content = fix_compiler_section(content, compiler, "tests" if "tests" in script_path.name else "benchs")
    
    # Si no funcionó el regex complejo, usar simple replace
    if content == original_content:
        # Fallback: simple string replacements
        replacements = [
            ('2>&1 | grep -v "LINK : warning LNK"\n            if [ $? -eq 0 ];', 
             '2>&1 | grep -v "LINK : warning LNK" | grep -v "^LINK : " || true\n            if [ -f'),
        ]
        
        for old, new in replacements:
            if old in content:
                print(f"   ℹ Usando reemplazo simple")
                # Esto requeriría más lógica específica...
                pass
    
    if content != original_content:
        with open(script_path, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"   ✓ {script_path.name}: Cambios aplicados")
        return True
    else:
        print(f"   ℹ {script_path.name}: Sin cambios (puede requerir corrección manual)")
        return False

def main():
    print("=" * 50)
    print("Corrigiendo lógica de verificación en scripts...")
    print("=" * 50)
    
    for script_name in SCRIPTS_TO_FIX:
        script_path = SCRIPTS_DIR / script_name
        if script_path.exists():
            fix_script_simple(script_path)
        else:
            print(f"   ⚠ {script_name}: No encontrado")
    
    print()
    print("=" * 50)
    print("✅ Proceso completado")
    print("=" * 50)

if __name__ == "__main__":
    main()
