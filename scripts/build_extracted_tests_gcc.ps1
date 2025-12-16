# Script para compilar uint128_extracted_tests.cpp con G++

$projectRoot = Resolve-Path "$PSScriptRoot/.."
$sourceFile = Join-Path $projectRoot "tests/uint128_extracted_tests.cpp"
$buildDir = Join-Path $projectRoot "build/build_tests/gcc/release"
$outputExe = Join-Path $buildDir "test_uint128_extracted.exe"

# Verificar si el archivo fuente existe
if (-not (Test-Path $sourceFile)) {
    Write-Error "No se encuentra el archivo fuente: $sourceFile"
    exit 1
}

# Crear directorio de salida si no existe
if (-not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir -Force | Out-Null
    Write-Host "Directorio creado: $buildDir" -ForegroundColor Gray
}

Write-Host "Compilando $sourceFile con g++..." -ForegroundColor Cyan

# Nota: No usamos -DNDEBUG porque estos tests dependen de assert() para verificar la lógica.
# Si se define NDEBUG, los asserts se eliminan y los tests no verifican nada.
$compileCmd = "g++ -std=c++17 -O3 -I`"$projectRoot/include`" `"$sourceFile`" -o `"$outputExe`""

Invoke-Expression $compileCmd

if ($LASTEXITCODE -eq 0) {
    Write-Host "✅ Compilación exitosa." -ForegroundColor Green
    Write-Host "Ejecutable generado en: $outputExe" -ForegroundColor Gray
} else {
    Write-Host "❌ Error de compilación." -ForegroundColor Red
    exit $LASTEXITCODE
}
