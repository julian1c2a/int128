#!/bin/bash
echo "🚀 BENCHMARK MULTIPLATAFORMA UINT128_T"
echo "========================================"

cd "/c/msys64/ucrt64/home/julian/CppProjects/int128"

# Crear directorio para resultados si no existe
mkdir -p benchmark_results

# Función para compilar y ejecutar benchmark
run_benchmark() {
    local compiler=$1
    local compiler_cmd=$2
    local output_suffix=$3
    
    echo ""
    echo "🔧 Compilando con $compiler..."
    
    # Compilar
    if $compiler_cmd -std=c++17 -O3 -DNDEBUG -I. benchmark_complete.cpp -o "benchmark_complete_${output_suffix}.exe"; then
        echo "✅ Compilación exitosa con $compiler"
        
        echo "🏃 Ejecutando benchmark con $compiler..."
        echo "=================================="
        
        # Ejecutar y guardar salida
        "./benchmark_complete_${output_suffix}.exe" | tee "benchmark_results/benchmark_output_${output_suffix}.txt"
        
        # Mover archivo CSV generado
        if [ -f benchmark_results_*.csv ]; then
            mv benchmark_results_*.csv "benchmark_results/benchmark_${output_suffix}.csv"
        fi
        
        echo ""
        echo "✅ Benchmark completado para $compiler"
        echo "📁 Resultados guardados en benchmark_results/"
        
    else
        echo "❌ Error en compilación con $compiler"
    fi
}

# Test con MSVC
if command -v cl.exe &> /dev/null; then
    echo "🎯 Preparando benchmark con Microsoft Visual C++..."
    # Para MSVC necesitamos configurar el entorno
    run_benchmark "MSVC" "cl.exe /std:c++17 /O2 /DNDEBUG /EHsc /I." "msvc"
fi

# Test con GCC
if [ -f "/c/msys64/ucrt64/bin/g++.exe" ]; then
    run_benchmark "GCC" "/c/msys64/ucrt64/bin/g++.exe" "gcc"
fi

# Test con Clang
if [ -f "/c/msys64/ucrt64/bin/clang++.exe" ]; then
    run_benchmark "Clang" "/c/msys64/ucrt64/bin/clang++.exe" "clang"
fi

echo ""
echo "🎉 TODOS LOS BENCHMARKS COMPLETADOS!"
echo "📊 Compara los resultados en el directorio benchmark_results/"
echo ""

# Crear un resumen comparativo
echo "📈 RESUMEN COMPARATIVO DE COMPILADORES" > benchmark_results/summary.txt
echo "=======================================" >> benchmark_results/summary.txt
echo "" >> benchmark_results/summary.txt

if [ -f "benchmark_results/benchmark_msvc.csv" ]; then
    echo "🔹 MSVC: benchmark_msvc.csv" >> benchmark_results/summary.txt
fi

if [ -f "benchmark_results/benchmark_gcc.csv" ]; then
    echo "🔹 GCC: benchmark_gcc.csv" >> benchmark_results/summary.txt
fi

if [ -f "benchmark_results/benchmark_clang.csv" ]; then
    echo "🔹 Clang: benchmark_clang.csv" >> benchmark_results/summary.txt
fi

echo "" >> benchmark_results/summary.txt
echo "💡 Abre los archivos CSV en Excel o LibreOffice para análisis detallado" >> benchmark_results/summary.txt

echo "📋 Resumen guardado en benchmark_results/summary.txt"