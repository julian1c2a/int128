# Script de build PowerShell mejorado para uint128_t
# Uso: .\build_msvc.ps1 [test_name] [debug|release]

param(
    [string]$TestName = "",
    [string]$Configuration = "debug"
)

$ProjectRoot = $PSScriptRoot
$IncludeDir = Join-Path $ProjectRoot "include"
$TestsDir = Join-Path $ProjectRoot "tests"
$BuildDir = Join-Path $ProjectRoot "build\test\$Configuration"

# Compiler path
$MSVC_PATH = "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64"
$CL_EXE = Join-Path $MSVC_PATH "cl.exe"

# Compiler flags
$CommonFlags = @("/std:c++latest", "/EHsc", "/nologo", "/I", $IncludeDir)
if ($Configuration -eq "debug") {
    $Flags = $CommonFlags + @("/Od", "/Zi", "/MDd", "/DDEBUG")
}
else {
    $Flags = $CommonFlags + @("/O2", "/MD", "/DNDEBUG")
}

# Ensure build directory exists
New-Item -ItemType Directory -Path $BuildDir -Force | Out-Null

function Build-Test {
    param(
        [string]$TestFile
    )
    
    $TestBaseName = [System.IO.Path]::GetFileNameWithoutExtension($TestFile)
    $TestSource = Join-Path $TestsDir $TestFile
    $TestExecutable = Join-Path $BuildDir "$TestBaseName.exe"
    
    Write-Host "Building $TestFile ($Configuration)..." -ForegroundColor Cyan
    
    if (-not (Test-Path $TestSource)) {
        Write-Host "✗ Source file not found: $TestSource" -ForegroundColor Red
        return $null
    }
    
    $BuildArgs = $Flags + @($TestSource, "/Fe:$TestExecutable")
    
    try {
        & $CL_EXE $BuildArgs 2>&1 | Out-Null
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✓ $TestBaseName compiled successfully" -ForegroundColor Green
            return $TestExecutable
        }
        else {
            Write-Host "✗ $TestBaseName compilation failed" -ForegroundColor Red
            Write-Host "Debug output:" -ForegroundColor Yellow
            & $CL_EXE $BuildArgs
            return $null
        }
    }
    catch {
        Write-Host "✗ Error executing compiler: $_" -ForegroundColor Red
        return $null
    }
}

function Run-Test {
    param(
        [string]$TestExecutable
    )
    
    if (Test-Path $TestExecutable) {
        Write-Host "Running $(Split-Path $TestExecutable -Leaf)..." -ForegroundColor Yellow
        Write-Host "=" * 60
        & $TestExecutable
        Write-Host "=" * 60
        Write-Host ""
    }
}

# Main execution
Write-Host "uint128_t Build System (MSVC)" -ForegroundColor Magenta
Write-Host "Configuration: $Configuration" -ForegroundColor Cyan
Write-Host "Build Directory: $BuildDir" -ForegroundColor Cyan
Write-Host ""

if ($TestName) {
    # Build specific test
    $TestFile = if ($TestName.EndsWith(".cpp")) { $TestName } else { "$TestName.cpp" }
    
    $TestExecutable = Build-Test -TestFile $TestFile
    if ($TestExecutable) {
        Run-Test -TestExecutable $TestExecutable
    }
}
else {
    Write-Host "Uso: .\build_msvc.ps1 <test_name> [debug|release]" -ForegroundColor Yellow
    Write-Host "Ejemplo: .\build_msvc.ps1 user_literals_test debug" -ForegroundColor Yellow
}