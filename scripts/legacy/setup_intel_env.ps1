# Script PowerShell para configurar Intel oneAPI
# Uso: . .\setup_intel_env.ps1  (debe ejecutarse con dot-sourcing)

$IntelOneAPIRoot = "C:\Program Files (x86)\Intel\oneAPI"
$SetvarsScript = Join-Path $IntelOneAPIRoot "setvars.bat"

Write-Host "Configurando Intel oneAPI..." -ForegroundColor Cyan

if (Test-Path $SetvarsScript) {
    Write-Host "Ejecutando setvars.bat..." -ForegroundColor Yellow
    
    # Ejecutar setvars.bat y capturar variables de entorno
    cmd /c "`"$SetvarsScript`" && set" | ForEach-Object {
        if ($_ -match "^(.*?)=(.*)$") {
            $varName = $matches[1]
            $varValue = $matches[2]
            [Environment]::SetEnvironmentVariable($varName, $varValue, "Process")
        }
    }
    
    # Verificar que icpx esté disponible
    $icpx = Get-Command icpx -ErrorAction SilentlyContinue
    $icx = Get-Command icx -ErrorAction SilentlyContinue
    
    if ($icpx) {
        Write-Host "✓ Intel C++ compiler (icpx) configurado correctamente" -ForegroundColor Green
        icpx --version | Select-Object -First 1
    }
    elseif ($icx) {
        Write-Host "✓ Intel C++ compiler (icx) configurado correctamente" -ForegroundColor Green
        icx --version | Select-Object -First 1
    }
    else {
        Write-Host "✗ No se encontró el compilador Intel (icpx/icx)" -ForegroundColor Red
        Write-Host "  Ejecute manualmente: `"$SetvarsScript`"" -ForegroundColor Red
    }
}
else {
    Write-Host "✗ Intel oneAPI no encontrado en: $IntelOneAPIRoot" -ForegroundColor Red
    Write-Host "  Verifique que Intel oneAPI esté instalado correctamente" -ForegroundColor Yellow
}
