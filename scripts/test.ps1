# Build rápido para desarrollo
# Compila y ejecuta un test específico en modo debug

param(
    [Parameter(Mandatory=$true)]
    [string]$TestName
)

# Llamar al script principal en modo debug
& "$(Split-Path $MyInvocation.MyCommand.Path)\build.ps1" $TestName "debug"