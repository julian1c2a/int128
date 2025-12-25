# Script para ejecutar benchmarks normalizados de uint128_t
# Genera resultados comparables entre diferentes sistemas

param(
    [string]$Config = "release"
)

$ErrorActionPreference = "Stop"

$BuildDir = "build"
$TestName = "normalized_benchmark"

Write-Host "=== uint128_t Normalized Benchmark Runner ===" -ForegroundColor Green
Write-Host "Building optimized version for accurate benchmarks..." -ForegroundColor Yellow

# Crear directorios necesarios
if (-not (Test-Path "$BuildDir\$Config")) {
    New-Item -ItemType Directory -Path "$BuildDir\$Config" -Force | Out-Null
}
if (-not (Test-Path "documentation\benchmarks")) {
    New-Item -ItemType Directory -Path "documentation\benchmarks" -Force | Out-Null
}

Write-Host "Compiler: MSVC" -ForegroundColor Cyan
Write-Host "Optimization: Maximum (/O2)" -ForegroundColor Cyan
Write-Host ""

# Compilar con optimizaciones máximas
Write-Host "Compiling $TestName..." -ForegroundColor White
$compileCmd = "cl /std:c++latest /O2 /DNDEBUG /Fe:$BuildDir\$Config\${TestName}.exe tests\$TestName.cpp"

try {
    Invoke-Expression $compileCmd
    if ($LASTEXITCODE -ne 0) { throw "Compilation failed" }
    Write-Host "✅ Compilation successful!" -ForegroundColor Green
}
catch {
    Write-Host "❌ Compilation failed!" -ForegroundColor Red
    Write-Host $_.Exception.Message -ForegroundColor Red
    exit 1
}

Write-Host ""

# Ejecutar benchmark
Write-Host "Running normalized benchmark (this may take a few minutes)..." -ForegroundColor Yellow
Write-Host "📊 Collecting performance statistics..." -ForegroundColor Cyan

try {
    & ".\$BuildDir\$Config\${TestName}.exe"
    
    Write-Host ""
    Write-Host "=== Benchmark Complete ===" -ForegroundColor Green
    Write-Host "📁 Results saved to: documentation\benchmarks\" -ForegroundColor Cyan
    Write-Host "🔍 CSV files contain detailed statistics for cross-machine comparison" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Key metrics explained:" -ForegroundColor White
    Write-Host "  • Median_ns: Middle value (reduces outlier impact)" -ForegroundColor Gray
    Write-Host "  • Relative_to_Baseline: Performance vs native uint64_t (lower is better)" -ForegroundColor Gray
    Write-Host "  • Ops_per_second: Throughput measure" -ForegroundColor Gray
    Write-Host ""
    Write-Host "💡 Tip: Run this on different machines to compare architectures!" -ForegroundColor Yellow
    
}
catch {
    Write-Host "❌ Benchmark execution failed!" -ForegroundColor Red
    Write-Host $_.Exception.Message -ForegroundColor Red
    exit 1
}