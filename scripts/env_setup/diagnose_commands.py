#!/usr/bin/env python3
"""
Script de diagnóstico avanzado para rastrear output extraño
Captura STDOUT y STDERR de comandos del shell
"""

import subprocess
import sys
from pathlib import Path

def test_command(cmd: str, description: str):
    """Prueba un comando y captura toda su salida"""
    print(f"\n{'='*80}")
    print(f"🧪 Probando: {description}")
    print(f"   Comando: {cmd}")
    print(f"{'-'*80}")
    
    try:
        result = subprocess.run(
            cmd,
            shell=True,
            capture_output=True,
            text=True,
            timeout=5
        )
        
        if result.stdout:
            print(f"📤 STDOUT ({len(result.stdout)} chars):")
            lines = result.stdout.split('\n')[:10]
            for line in lines:
                print(f"   {line}")
            if len(result.stdout.split('\n')) > 10:
                print(f"   ... y {len(result.stdout.split('\n')) - 10} líneas más")
        
        if result.stderr:
            print(f"📥 STDERR ({len(result.stderr)} chars):")
            lines = result.stderr.split('\n')[:10]
            for line in lines:
                print(f"   {line}")
            if len(result.stderr.split('\n')) > 10:
                print(f"   ... y {len(result.stderr.split('\n')) - 10} líneas más")
        
        # Buscar texto sospechoso
        suspicious = ['PHONON', 'quantum-espresso', 'Error in routine', 'stopping']
        full_output = result.stdout + result.stderr
        
        found = [word for word in suspicious if word in full_output]
        if found:
            print(f"   ⚠️  ENCONTRADO: {', '.join(found)}")
        else:
            print(f"   ✅ Sin texto sospechoso")
        
        print(f"   Return code: {result.returncode}")
        
    except subprocess.TimeoutExpired:
        print(f"   ⏱️  Timeout")
    except Exception as e:
        print(f"   ❌ Error: {e}")

def main():
    print("\n" + "="*80)
    print("🔍 DIAGNÓSTICO DE COMANDOS - Buscando origen de output extraño")
    print("="*80)
    
    # Comandos comunes que podrían estar generando salida
    commands = [
        ("gcc --version", "GCC versión"),
        ("g++ --version", "G++ versión"),
        ("clang --version", "Clang versión"),
        ("clang++ --version", "Clang++ versión"),
        ("cl.exe 2>&1 | head -n 1", "MSVC versión"),
        ("icx --version", "Intel ICX versión"),
        ("which g++", "Ubicación de g++"),
        ("which clang++", "Ubicación de clang++"),
        ("echo $CC", "Variable CC"),
        ("echo $CXX", "Variable CXX"),
    ]
    
    for cmd, desc in commands:
        test_command(cmd, desc)
    
    print(f"\n{'='*80}")
    print("🔍 REVISANDO ARCHIVOS DE CONFIGURACIÓN DEL SHELL")
    print(f"{'='*80}")
    
    # Buscar en archivos de configuración
    config_files = [
        Path.home() / ".bashrc",
        Path.home() / ".bash_profile",
        Path.home() / ".profile",
        Path.home() / ".zshrc",
    ]
    
    for config_file in config_files:
        if config_file.exists():
            print(f"\n📄 {config_file}")
            content = config_file.read_text(encoding='utf-8', errors='ignore')
            
            suspicious = ['PHONON', 'quantum-espresso', 'qe', 'espresso']
            found = [word for word in suspicious if word.lower() in content.lower()]
            
            if found:
                print(f"   ⚠️  ENCONTRADO: {', '.join(found)}")
                lines = content.split('\n')
                for i, line in enumerate(lines, 1):
                    if any(word.lower() in line.lower() for word in suspicious):
                        print(f"   Línea {i}: {line.strip()}")
            else:
                print(f"   ✅ Sin referencias a Quantum ESPRESSO")
        else:
            print(f"\n📄 {config_file} - No existe")
    
    print(f"\n{'='*80}")
    print("✅ Diagnóstico completado")
    print(f"{'='*80}\n")

if __name__ == "__main__":
    main()
