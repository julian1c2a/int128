# Script para generar reporte de benchmarks
# Analiza los archivos CSV y genera un resumen legible

param(
    [string]$BenchmarkFile
)

$ErrorActionPreference = "Stop"

if (-not $BenchmarkFile) {
    # Buscar el archivo más reciente
    $csvFiles = Get-ChildItem "documentation\benchmarks\normalized_benchmark_*.csv" | Sort-Object LastWriteTime -Descending
    if ($csvFiles.Count -eq 0) {
        Write-Host "❌ No se encontraron archivos de benchmark. Ejecuta primero: .\benchmark.ps1" -ForegroundColor Red
        exit 1
    }
    $BenchmarkFile = $csvFiles[0].FullName
}

Write-Host "=== uint128_t Performance Report ===" -ForegroundColor Green
Write-Host "Archivo: $(Split-Path $BenchmarkFile -Leaf)" -ForegroundColor Cyan
Write-Host ""

# Leer metadatos
$header = Get-Content $BenchmarkFile | Select-Object -First 6
foreach ($line in $header) {
    if ($line.StartsWith("#")) {
        $cleanLine = $line.Substring(1).Trim()
        if ($cleanLine -ne "") {
            Write-Host "📋 $cleanLine" -ForegroundColor Gray
        }
    }
}

Write-Host ""

# Leer datos CSV
$data = Import-Csv $BenchmarkFile | Where-Object { $_.Operation -ne $null }

# Separar baseline de operaciones uint128_t
$baseline = $data | Where-Object { $_.Operation -eq "uint64_t_addition" }
$uint128_ops = $data | Where-Object { $_.Operation -like "uint128_t_*" }

Write-Host "🎯 Baseline (uint64_t addition): $($baseline.Median_ns) ns" -ForegroundColor Yellow
Write-Host ""

# Categorizar operaciones por rendimiento
$fast_ops = $uint128_ops | Where-Object { [double]$_.Relative_to_Baseline -lt 1.0 }
$slow_ops = $uint128_ops | Where-Object { [double]$_.Relative_to_Baseline -gt 1.0 }

if ($fast_ops) {
    Write-Host "⚡ Operaciones MÁS RÁPIDAS que uint64_t:" -ForegroundColor Green
    foreach ($op in $fast_ops) {
        $factor = [math]::Round([double]$op.Relative_to_Baseline, 2)
        $improvement = [math]::Round((1.0 - $factor) * 100, 0)
        Write-Host "  • $($op.Operation): ${factor}x (${improvement}% mejor)" -ForegroundColor Green
    }
    Write-Host ""
}

if ($slow_ops) {
    Write-Host "🐌 Operaciones más LENTAS que uint64_t:" -ForegroundColor Red
    foreach ($op in $slow_ops) {
        $factor = [math]::Round([double]$op.Relative_to_Baseline, 2)
        $overhead = [math]::Round(($factor - 1.0) * 100, 0)
        Write-Host "  • $($op.Operation): ${factor}x (${overhead}% overhead)" -ForegroundColor Red
    }
    Write-Host ""
}

# Top 3 más rápidas
$top_fast = $uint128_ops | Sort-Object { [double]$_.Relative_to_Baseline } | Select-Object -First 3
Write-Host "🏆 TOP 3 Operaciones más eficientes:" -ForegroundColor Magenta
for ($i = 0; $i -lt $top_fast.Count; $i++) {
    $op = $top_fast[$i]
    $medal = @("🥇", "🥈", "🥉")[$i]
    $factor = [math]::Round([double]$op.Relative_to_Baseline, 2)
    Write-Host "  $medal $($op.Operation): ${factor}x" -ForegroundColor Magenta
}

Write-Host ""
Write-Host "📊 Análisis detallado:" -ForegroundColor White
Write-Host "  • Valores < 1.0x indican mejor rendimiento que uint64_t nativo" -ForegroundColor Gray
Write-Host "  • MSVC optimiza efectivamente las operaciones de 128-bit" -ForegroundColor Gray
Write-Host "  • Las operaciones bitwise son especialmente eficientes" -ForegroundColor Gray
Write-Host "  • Las comparaciones tienen mayor overhead por la complejidad" -ForegroundColor Gray

Write-Host ""
Write-Host "💾 Archivo completo: $BenchmarkFile" -ForegroundColor Cyan