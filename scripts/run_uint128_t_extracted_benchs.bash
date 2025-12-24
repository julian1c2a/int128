#!/usr/bin/env bash
# Script para ejecutar los benchmarks de uint128_t_extracted_benchs

set -u

# Detectar directorio del script y directorio raíz del proyecto
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"

# Validar argumentos
if [ $# -lt 2 ]; then
    echo "❌ ERROR: Se requieren al menos 2 argumentos"
    echo "Uso: $0 [compiler] [mode] [print]"
    echo "  compiler: intel, msvc, gcc, clang, all"
    echo "  mode: debug, release, all"
    echo "  print: opcional, cualquier valor para activar"
    exit 1
fi

# Compilador y modo
COMPILER="${1}"
MODE="${2}"
PRINT_MODE="${3:-}"

COMPILER=$(echo "$COMPILER" | tr '[:upper:]' '[:lower:]')
MODE=$(echo "$MODE" | tr '[:upper:]' '[:lower:]')

# Validar compilador
if [[ ! "$COMPILER" =~ ^(intel|msvc|gcc|clang|all)$ ]]; then
    echo "❌ ERROR: Compilador inválido: $COMPILER"
    echo "Compiladores válidos: intel, msvc, gcc, clang, all"
    exit 1
fi

# Validar modo
if [[ ! "$MODE" =~ ^(debug|release|all)$ ]]; then
    echo "❌ ERROR: Modo inválido: $MODE"
    echo "Modos válidos: debug, release, all"
    exit 1
fi

echo "========================================="
echo " EJECUTANDO: uint128_t_extracted_benchs"
echo " Compilador: $COMPILER"
echo " Modo: $MODE"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Directorio para resultados (solo si se activa print)
if [ -n "$PRINT_MODE" ]; then
    RESULTS_DIR="$PROJECT_ROOT/build/build_benchs_results"
fi

# Función para ejecutar un benchmark
run_benchmark() {
    local compiler=$1
    local mode=$2
    local executable=$3
    
    echo ""
    echo "───────────────────────────────────────"
    echo "⚡ $compiler ($mode)"
    echo "───────────────────────────────────────"
    
    if [ ! -f "$executable" ]; then
        echo -e "${YELLOW}⚠️  Ejecutable no encontrado${NC}"
        return 1
    fi
    
    # Ejecutar el benchmark
    if [ -n "$PRINT_MODE" ]; then
        # Crear directorio específico para este compilador/modo
        local output_dir="$RESULTS_DIR/${compiler,,}/${mode,,}"
        mkdir -p "$output_dir"
        
        # Archivo de salida con timestamp
        local timestamp=$(date '+%Y%m%d_%H%M%S')
        local output_file="$output_dir/uint128_t_extracted_benchs_results_${timestamp}.txt"
        
        # Ejecutar el benchmark y guardar resultado
        if "$executable" | tee "$output_file"; then
            echo ""
            echo -e "${GREEN}✅ Benchmark completado${NC}"
            echo "   Resultados guardados en: $output_file"
            return 0
        else
            echo -e "${RED}❌ Benchmark falló${NC}"
            return 1
        fi
    else
        # Ejecutar sin guardar
        if "$executable"; then
            echo ""
            echo -e "${GREEN}✅ Benchmark completado${NC}"
            return 0
        else
            echo -e "${RED}❌ Benchmark falló${NC}"
            return 1
        fi
    fi
}

# Contadores
total=0
passed=0

# ---------------------------------------
# GCC
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "gcc" ]; then
    if [ "$MODE" = "all" ] || [ "$MODE" = "debug" ]; then
        ((total++))
        if run_benchmark "GCC" "Debug" "$PROJECT_ROOT/build/build_benchs/gcc/debug/uint128_t_extracted_benchs.exe"; then
            ((passed++))
        fi
    fi
    
    if [ "$MODE" = "all" ] || [ "$MODE" = "release" ]; then
        ((total++))
        if run_benchmark "GCC" "Release" "$PROJECT_ROOT/build/build_benchs/gcc/release/uint128_t_extracted_benchs.exe"; then
            ((passed++))
        fi
    fi
fi

# ---------------------------------------
# Clang
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "clang" ]; then
    if [ "$MODE" = "all" ] || [ "$MODE" = "debug" ]; then
        ((total++))
        if run_benchmark "Clang" "Debug" "$PROJECT_ROOT/build/build_benchs/clang/debug/uint128_t_extracted_benchs.exe"; then
            ((passed++))
        fi
    fi
    
    if [ "$MODE" = "all" ] || [ "$MODE" = "release" ]; then
        ((total++))
        if run_benchmark "Clang" "Release" "$PROJECT_ROOT/build/build_benchs/clang/release/uint128_t_extracted_benchs.exe"; then
            ((passed++))
        fi
    fi
fi

# ---------------------------------------
# Intel
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "intel" ]; then
    if [ "$MODE" = "all" ] || [ "$MODE" = "debug" ]; then
        if [ -f "$PROJECT_ROOT/build/build_benchs/intel/debug/uint128_t_extracted_benchs.exe" ]; then
            ((total++))
            if run_benchmark "Intel" "Debug" "$PROJECT_ROOT/build/build_benchs/intel/debug/uint128_t_extracted_benchs.exe"; then
                ((passed++))
            fi
        fi
    fi
    
    if [ "$MODE" = "all" ] || [ "$MODE" = "release" ]; then
        if [ -f "$PROJECT_ROOT/build/build_benchs/intel/release/uint128_t_extracted_benchs.exe" ]; then
            ((total++))
            if run_benchmark "Intel" "Release" "$PROJECT_ROOT/build/build_benchs/intel/release/uint128_t_extracted_benchs.exe"; then
                ((passed++))
            fi
        fi
    fi
fi

# ---------------------------------------
# MSVC
# ---------------------------------------
if [ "$COMPILER" = "all" ] || [ "$COMPILER" = "msvc" ]; then
    if [ "$MODE" = "all" ] || [ "$MODE" = "debug" ]; then
        if [ -f "$PROJECT_ROOT/build/build_benchs/msvc/debug/uint128_t_extracted_benchs.exe" ]; then
            ((total++))
            if run_benchmark "MSVC" "Debug" "$PROJECT_ROOT/build/build_benchs/msvc/debug/uint128_t_extracted_benchs.exe"; then
                ((passed++))
            fi
        fi
    fi
    
    if [ "$MODE" = "all" ] || [ "$MODE" = "release" ]; then
        if [ -f "$PROJECT_ROOT/build/build_benchs/msvc/release/uint128_t_extracted_benchs.exe" ]; then
            ((total++))
            if run_benchmark "MSVC" "Release" "$PROJECT_ROOT/build/build_benchs/msvc/release/uint128_t_extracted_benchs.exe"; then
                ((passed++))
            fi
        fi
    fi
fi

# ---------------------------------------
# Resumen final
# ---------------------------------------
echo ""
echo "========================================="
echo " RESUMEN: $passed/$total benchmarks completados"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="

if [ $passed -eq $total ]; then
    echo -e "${GREEN}✅ TODOS LOS BENCHMARKS COMPLETADOS${NC}"
    exit 0
else
    echo -e "${RED}❌ ALGUNOS BENCHMARKS FALLARON${NC}"
    exit 1
fi
