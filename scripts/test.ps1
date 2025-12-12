# Build rápido para desarrollo
# Compila y ejecuta un test específico en modo debug

param(
    [Parameter(Mandatory = $true)]
    [string]$TestName
)

# Verificar si es el benchmark normalizado
if ($TestName -eq "normalized_benchmark") {
    Write-Host "=== Building Normalized Benchmark ===" -ForegroundColor Green
    Write-Host "This benchmark compares uint128_t performance vs native uint64_t" -ForegroundColor Yellow
    Write-Host "Results will be saved to documentation/benchmarks/" -ForegroundColor Yellow
}

# Llamar al script principal en modo debug
& "$(Split-Path $MyInvocation.MyCommand.Path)\build.ps1" $TestName "debug"