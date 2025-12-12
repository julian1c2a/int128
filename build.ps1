# Script de build para uint128_t
# Uso: .\build.ps1 [test_name] [debug|release]

param(
    [string]$TestName = "",
    [string]$Configuration = "debug"
)

$ProjectRoot = $PSScriptRoot
$IncludeDir = Join-Path $ProjectRoot "include"
$TestsDir = Join-Path $ProjectRoot "tests"
$BuildDir = Join-Path $ProjectRoot "build\test\$Configuration"

# Compiler paths
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
    
    $BuildArgs = $Flags + @($TestSource, "/Fe:$TestExecutable")
    
    & $CL_EXE $BuildArgs
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ $TestBaseName compiled successfully" -ForegroundColor Green
        return $TestExecutable
    }
    else {
        Write-Host "✗ $TestBaseName compilation failed" -ForegroundColor Red
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
Write-Host "uint128_t Build System" -ForegroundColor Magenta
Write-Host "Configuration: $Configuration" -ForegroundColor Cyan
Write-Host "Build Directory: $BuildDir" -ForegroundColor Cyan
Write-Host ""

if ($TestName) {
    # Build specific test
    if ($TestName.EndsWith(".cpp")) {
        $TestFile = $TestName
    }
    else {
        $TestFile = "$TestName.cpp"
    }
    
    $TestExecutable = Build-Test -TestFile $TestFile
    if ($TestExecutable) {
        Run-Test -TestExecutable $TestExecutable
    }
}
else {
    # Build all tests
    $TestFiles = Get-ChildItem -Path $TestsDir -Filter "*.cpp" | Where-Object { $_.Name -ne "test_uint128.cpp" }
    
    $SuccessfulBuilds = @()
    
    foreach ($TestFile in $TestFiles) {
        $TestExecutable = Build-Test -TestFile $TestFile.Name
        if ($TestExecutable) {
            $SuccessfulBuilds += $TestExecutable
        }
        Write-Host ""
    }
    
    Write-Host "Build Summary:" -ForegroundColor Magenta
    Write-Host "Successfully built: $($SuccessfulBuilds.Count) tests" -ForegroundColor Green
    Write-Host "Failed: $($TestFiles.Count - $SuccessfulBuilds.Count) tests" -ForegroundColor Red
    
    if ($SuccessfulBuilds.Count -gt 0) {
        Write-Host ""
        $RunAll = Read-Host "¿Ejecutar todos los tests? (y/N)"
        if ($RunAll -eq "y" -or $RunAll -eq "Y") {
            foreach ($TestExe in $SuccessfulBuilds) {
                Run-Test -TestExecutable $TestExe
            }
        }
    }
}