# Benchmark Multiplataforma para uint128_t
param(
    [int]$iterations = 10000000,
    [string]$outputDir = "benchmark_results"
)

Write-Host "🚀 BENCHMARK MULTIPLATAFORMA UINT128_T" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host "Iteraciones por operación: $iterations" -ForegroundColor Yellow

Set-Location "c:\msys64\ucrt64\home\julian\CppProjects\int128"

# Crear directorio para resultados
if (!(Test-Path $outputDir)) {
    New-Item -ItemType Directory -Path $outputDir | Out-Null
}

function Run-Benchmark {
    param(
        [string]$CompilerName,
        [string]$CompilerCmd,
        [string]$CompilerArgs,
        [string]$OutputSuffix
    )
    
    Write-Host ""
    Write-Host "🔧 Compilando con $CompilerName..." -ForegroundColor Cyan
    
    # Preparar comando de compilación
    $compileCmd = "$CompilerCmd $CompilerArgs benchmark_complete.cpp"
    
    try {
        # Compilar
        Invoke-Expression $compileCmd
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✅ Compilación exitosa con $CompilerName" -ForegroundColor Green
            
            Write-Host "🏃 Ejecutando benchmark con $CompilerName..." -ForegroundColor Yellow
            Write-Host "==================================" -ForegroundColor Yellow
            
            # Ejecutar benchmark
            $outputFile = "$outputDir\benchmark_output_$OutputSuffix.txt"
            & ".\benchmark_complete_$OutputSuffix.exe" | Tee-Object -FilePath $outputFile
            
            # Mover archivo CSV si existe
            $csvFiles = Get-ChildItem -Name "benchmark_results_*.csv" -ErrorAction SilentlyContinue
            if ($csvFiles) {
                foreach ($csvFile in $csvFiles) {
                    Move-Item $csvFile "$outputDir\benchmark_$OutputSuffix.csv" -Force
                }
            }
            
            Write-Host ""
            Write-Host "✅ Benchmark completado para $CompilerName" -ForegroundColor Green
            Write-Host "📁 Resultados guardados en $outputDir\" -ForegroundColor Cyan
            
        } else {
            Write-Host "❌ Error en compilación con $CompilerName" -ForegroundColor Red
        }
    }
    catch {
        Write-Host "❌ Error ejecutando $CompilerName : $_" -ForegroundColor Red
    }
}

# Test con MSVC
if (Get-Command cl.exe -ErrorAction SilentlyContinue) {
    Run-Benchmark "MSVC" "cl.exe" "/std:c++17 /O2 /DNDEBUG /EHsc /I. /Fe:benchmark_complete_msvc.exe" "msvc"
}

# Test con GCC
$gccPath = "c:\msys64\ucrt64\bin\g++.exe"
if (Test-Path $gccPath) {
    Run-Benchmark "GCC" $gccPath "-std=c++17 -O3 -DNDEBUG -I. -o benchmark_complete_gcc.exe" "gcc"
}

# Test con Clang
$clangPath = "c:\msys64\ucrt64\bin\clang++.exe"
if (Test-Path $clangPath) {
    Run-Benchmark "Clang" $clangPath "-std=c++17 -O3 -DNDEBUG -I. -o benchmark_complete_clang.exe" "clang"
}

Write-Host ""
Write-Host "🎉 TODOS LOS BENCHMARKS COMPLETADOS!" -ForegroundColor Green
Write-Host "📊 Compara los resultados en el directorio $outputDir\" -ForegroundColor Cyan

# Crear resumen comparativo
$summaryFile = "$outputDir\summary.txt"
@"
📈 RESUMEN COMPARATIVO DE COMPILADORES
=======================================

"@ | Out-File -FilePath $summaryFile -Encoding UTF8

$csvFiles = Get-ChildItem -Path $outputDir -Name "benchmark_*.csv" -ErrorAction SilentlyContinue
if ($csvFiles) {
    foreach ($csvFile in $csvFiles) {
        $compiler = $csvFile -replace "benchmark_", "" -replace ".csv", ""
        "🔹 $($compiler.ToUpper()): $csvFile" | Add-Content -Path $summaryFile -Encoding UTF8
    }
}

@"

💡 Abre los archivos CSV en Excel o LibreOffice para análisis detallado
🎯 Usa PowerBI o similar para crear gráficos comparativos
📊 Los archivos .txt contienen la salida completa de cada benchmark
"@ | Add-Content -Path $summaryFile -Encoding UTF8

Write-Host "📋 Resumen guardado en $summaryFile" -ForegroundColor Cyan

# Mostrar estadísticas básicas si tenemos los CSVs
$csvFiles = Get-ChildItem -Path $outputDir -Name "benchmark_*.csv" -ErrorAction SilentlyContinue
if ($csvFiles.Count -gt 1) {
    Write-Host ""
    Write-Host "📊 COMPARACIÓN RÁPIDA (Suma - uint128_t vs uint64_t):" -ForegroundColor Yellow
    
    foreach ($csvFile in $csvFiles) {
        $compiler = $csvFile -replace "benchmark_", "" -replace ".csv", ""
        $csvPath = Join-Path $outputDir $csvFile
        
        if (Test-Path $csvPath) {
            $data = Import-Csv $csvPath
            $uint128Addition = $data | Where-Object { $_.Operacion -eq "Addition" -and $_.Tipo -eq "uint128_t" }
            $uint64Addition = $data | Where-Object { $_.Operacion -eq "Addition" -and $_.Tipo -eq "uint64_t" }
            
            if ($uint128Addition -and $uint64Addition) {
                $ratio = [math]::Round([double]$uint128Addition.Media_ns / [double]$uint64Addition.Media_ns, 2)
                Write-Host "$($compiler.ToUpper()): uint128_t es $ratio veces más lento que uint64_t en suma" -ForegroundColor White
            }
        }
    }
}