#!/usr/bin/env bash
# Script para ejecutar los benchmarks de uint128_limits_extracted_benchs

echo "========================================="
echo " EJECUTANDO: uint128_limits_extracted_benchs"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Crear directorio para resultados
RESULTS_DIR="../benchmark_results"
mkdir -p "$RESULTS_DIR"

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
        echo -e "${YELLOW}⚠️  Ejecutable no encontrado: $executable${NC}"
        return 1
    fi
    
    # Crear directorio específico para este compilador/modo
    local output_dir="$RESULTS_DIR/${compiler,,}/${mode,,}"
    mkdir -p "$output_dir"
    
    # Archivo de salida con timestamp
    local timestamp=$(date '+%Y%m%d_%H%M%S')
    local output_file="$output_dir/uint128_limits_benchs_${timestamp}.txt"
    
    # Ejecutar el benchmark y guardar resultado
    if "$executable" | tee "$output_file"; then
        echo ""
        echo -e "${GREEN}✅ Benchmark completado${NC}"
        echo -e "📊 Resultados guardados en: $output_file"
        return 0
    else
        echo -e "${RED}❌ Benchmark falló${NC}"
        return 1
    fi
}

# Contadores
total=0
passed=0

# ---------------------------------------
# 1. GCC
# ---------------------------------------
((total++))
if run_benchmark "GCC" "Debug" "../build/uint128_limits_extracted_benchs/gcc/debug/uint128_limits_extracted_benchs"; then
    ((passed++))
fi

((total++))
if run_benchmark "GCC" "Release" "../build/uint128_limits_extracted_benchs/gcc/release/uint128_limits_extracted_benchs"; then
    ((passed++))
fi

# ---------------------------------------
# 2. Clang
# ---------------------------------------
((total++))
if run_benchmark "Clang" "Debug" "../build/uint128_limits_extracted_benchs/clang/debug/uint128_limits_extracted_benchs"; then
    ((passed++))
fi

((total++))
if run_benchmark "Clang" "Release" "../build/uint128_limits_extracted_benchs/clang/release/uint128_limits_extracted_benchs"; then
    ((passed++))
fi

# ---------------------------------------
# 3. Intel ICX
# ---------------------------------------
if [ -f "../build/uint128_limits_extracted_benchs/intel/debug/uint128_limits_extracted_benchs.exe" ]; then
    ((total++))
    if run_benchmark "Intel ICX" "Debug" "../build/uint128_limits_extracted_benchs/intel/debug/uint128_limits_extracted_benchs.exe"; then
        ((passed++))
    fi
    
    ((total++))
    if run_benchmark "Intel ICX" "Release" "../build/uint128_limits_extracted_benchs/intel/release/uint128_limits_extracted_benchs.exe"; then
        ((passed++))
    fi
fi

# ---------------------------------------
# 4. MSVC
# ---------------------------------------
if [ -f "../build/uint128_limits_extracted_benchs/msvc/debug/uint128_limits_extracted_benchs.exe" ]; then
    ((total++))
    if run_benchmark "MSVC" "Debug" "../build/uint128_limits_extracted_benchs/msvc/debug/uint128_limits_extracted_benchs.exe"; then
        ((passed++))
    fi
    
    ((total++))
    if run_benchmark "MSVC" "Release" "../build/uint128_limits_extracted_benchs/msvc/release/uint128_limits_extracted_benchs.exe"; then
        ((passed++))
    fi
fi

# ---------------------------------------
# Resumen final
# ---------------------------------------
echo ""
echo "========================================="
echo " RESUMEN: $passed/$total benchmarks ejecutados"
echo " $(date '+%Y-%m-%d %H:%M:%S')"
echo "========================================="
echo ""
echo "📁 Resultados guardados en: $RESULTS_DIR"

if [ $passed -eq $total ]; then
    echo -e "${GREEN}✅ TODOS LOS BENCHMARKS COMPLETADOS${NC}"
    exit 0
else
    echo -e "${RED}❌ ALGUNOS BENCHMARKS FALLARON${NC}"
    exit 1
fi
