#!/bin/bash
# Script para compilar tests y benchmarks de uint128_traits con los 4 compiladores
# Compiladores: g++ (ucrt64), clang (clang64), Intel OneAPI, MSVC

set -e  # Salir en caso de error

PROJECT_DIR="/c/msys64/ucrt64/home/julian/CppProjects/int128"
cd "$PROJECT_DIR"

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}============================================================${NC}"
echo -e "${BLUE}  Compilación de uint128_traits tests y benchmarks${NC}"
echo -e "${BLUE}============================================================${NC}"

# Crear directorios de salida si no existen
mkdir -p build/build_tests/gcc/release
mkdir -p build/build_benchmarks/gcc/release

# =============================================================================
# COMPILADOR 1: G++ (UCRT64)
# =============================================================================
echo -e "\n${YELLOW}[1/4] Compilando con G++ (ucrt64)...${NC}"

GCC_BIN="/c/msys64/ucrt64/bin"
GCC_INCLUDE="/c/msys64/ucrt64/include"
GCC_LIB="/c/msys64/ucrt64/lib"

# Tests
echo -e "${GREEN}  - Compilando tests...${NC}"
"$GCC_BIN/g++.exe" -std=c++20 -O3 -march=native -Wall -Wextra \
    -I"$PROJECT_DIR/include" \
    -I"$GCC_INCLUDE" \
    -L"$GCC_LIB" \
    tests/uint128_traits_extracted_tests.cpp \
    -o build/build_tests/gcc/release/uint128_traits_tests_gcc.exe

# Benchmarks
echo -e "${GREEN}  - Compilando benchmarks...${NC}"
"$GCC_BIN/g++.exe" -std=c++20 -O3 -march=native -Wall -Wextra \
    -I"$PROJECT_DIR/include" \
    -I"$GCC_INCLUDE" \
    -L"$GCC_LIB" \
    benchmarks/uint128_traits_extracted_benchs.cpp \
    -o build/build_benchmarks/gcc/release/uint128_traits_benchs_gcc.exe

echo -e "${GREEN}  ✅ G++ compilación completada${NC}"

# =============================================================================
# COMPILADOR 2: CLANG (CLANG64)
# =============================================================================
echo -e "\n${YELLOW}[2/4] Compilando con Clang (clang64)...${NC}"

# Crear directorios
mkdir -p build/build_tests/clang/release
mkdir -p build/build_benchmarks/clang/release

CLANG_BIN="/c/msys64/clang64/bin"
CLANG_INCLUDE="/c/msys64/clang64/include"
CLANG_LIB="/c/msys64/clang64/lib"

# Tests
echo -e "${GREEN}  - Compilando tests...${NC}"
"$CLANG_BIN/clang++.exe" -std=c++20 -O3 -march=native -Wall -Wextra \
    -I"$PROJECT_DIR/include" \
    -I"$CLANG_INCLUDE" \
    -L"$CLANG_LIB" \
    tests/uint128_traits_extracted_tests.cpp \
    -o build/build_tests/clang/release/uint128_traits_tests_clang.exe

# Benchmarks
echo -e "${GREEN}  - Compilando benchmarks...${NC}"
"$CLANG_BIN/clang++.exe" -std=c++20 -O3 -march=native -Wall -Wextra \
    -I"$PROJECT_DIR/include" \
    -I"$CLANG_INCLUDE" \
    -L"$CLANG_LIB" \
    benchmarks/uint128_traits_extracted_benchs.cpp \
    -o build/build_benchmarks/clang/release/uint128_traits_benchs_clang.exe

echo -e "${GREEN}  ✅ Clang compilación completada${NC}"

# =============================================================================
# COMPILADOR 3: INTEL ONEAPI
# =============================================================================
echo -e "\n${YELLOW}[3/4] Compilando con Intel OneAPI...${NC}"

# Configurar Intel OneAPI
if [ -f "/c/Program Files (x86)/Intel/oneAPI/setvars.sh" ]; then
    source "/c/Program Files (x86)/Intel/oneAPI/setvars.sh" intel64 vs2022
    
    # Crear directorios
    mkdir -p build/build_tests/intel/release
    mkdir -p build/build_benchmarks/intel/release
    
    # Tests
    echo -e "${GREEN}  - Compilando tests...${NC}"
    icx -std=c++20 -O3 -xHost -Wall \
        -I"$PROJECT_DIR/include" \
        tests/uint128_traits_extracted_tests.cpp \
        -o build/build_tests/intel/release/uint128_traits_tests_intel.exe
    
    # Benchmarks
    echo -e "${GREEN}  - Compilando benchmarks...${NC}"
    icx -std=c++20 -O3 -xHost -Wall \
        -I"$PROJECT_DIR/include" \
        benchmarks/uint128_traits_extracted_benchs.cpp \
        -o build/build_benchmarks/intel/release/uint128_traits_benchs_intel.exe
    
    echo -e "${GREEN}  ✅ Intel OneAPI compilación completada${NC}"
else
    echo -e "${RED}  ⚠️  Intel OneAPI no encontrado, saltando...${NC}"
fi

# =============================================================================
# COMPILADOR 4: MSVC
# =============================================================================
echo -e "\n${YELLOW}[4/4] Compilando con MSVC...${NC}"

# Buscar vcvarsall.bat
VCVARSALL="/c/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/vcvarsall.bat"
if [ ! -f "$VCVARSALL" ]; then
    VCVARSALL="/c/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/vcvarsall.bat"
fi

if [ -f "$VCVARSALL" ]; then
    # Crear directorios
    mkdir -p build/build_tests/msvc/release
    mkdir -p build/build_benchmarks/msvc/release
    
    # Crear script temporal para MSVC
    cat > build/compile_msvc.bat <<'EOF'
@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

echo   - Compilando tests...
cl /std:c++20 /O2 /EHsc /W4 ^
    /I"C:\msys64\ucrt64\home\julian\CppProjects\int128\include" ^
    C:\msys64\ucrt64\home\julian\CppProjects\int128\tests\uint128_traits_extracted_tests.cpp ^
    /Fe:C:\msys64\ucrt64\home\julian\CppProjects\int128\build\build_tests\msvc\release\uint128_traits_tests_msvc.exe ^
    /link /MACHINE:X64

echo   - Compilando benchmarks...
cl /std:c++20 /O2 /EHsc /W4 ^
    /I"C:\msys64\ucrt64\home\julian\CppProjects\int128\include" ^
    C:\msys64\ucrt64\home\julian\CppProjects\int128\benchmarks\uint128_traits_extracted_benchs.cpp ^
    /Fe:C:\msys64\ucrt64\home\julian\CppProjects\int128\build\build_benchmarks\msvc\release\uint128_traits_benchs_msvc.exe ^
    /link /MACHINE:X64
EOF
    
    cmd //c "build\\compile_msvc.bat" 2>&1 | grep -v "Microsoft" | grep -v "Copyright" || true
    echo -e "${GREEN}  ✅ MSVC compilación completada${NC}"
else
    echo -e "${RED}  ⚠️  MSVC no encontrado, saltando...${NC}"
fi

# =============================================================================
# RESUMEN
# =============================================================================
echo -e "\n${BLUE}============================================================${NC}"
echo -e "${BLUE}                    RESUMEN DE COMPILACIÓN${NC}"
echo -e "${BLUE}============================================================${NC}"

echo -e "\n${GREEN}Ejecutables generados:${NC}\n"

echo -e "${CYAN}Tests:${NC}"
ls -lh build/build_tests/*/release/*.exe 2>/dev/null || echo "No se encontraron tests"

echo -e "\n${CYAN}Benchmarks:${NC}"
ls -lh build/build_benchmarks/*/release/*.exe 2>/dev/null || echo "No se encontraron benchmarks"

echo -e "\n${GREEN}Para ejecutar los tests:${NC}"
echo -e "  ./build/build_tests/gcc/release/uint128_traits_tests_gcc.exe"
echo -e "  ./build/build_tests/clang/release/uint128_traits_tests_clang.exe"
echo -e "  ./build/build_tests/intel/release/uint128_traits_tests_intel.exe"
echo -e "  ./build/build_tests/msvc/release/uint128_traits_tests_msvc.exe"

echo -e "\n${GREEN}Para ejecutar los benchmarks:${NC}"
echo -e "  ./build/build_benchmarks/gcc/release/uint128_traits_benchs_gcc.exe"
echo -e "  ./build/build_benchmarks/clang/release/uint128_traits_benchs_clang.exe"
echo -e "  ./build/build_benchmarks/intel/release/uint128_traits_benchs_intel.exe"
echo -e "  ./build/build_benchmarks/msvc/release/uint128_traits_benchs_msvc.exe"

echo -e "\n${BLUE}============================================================${NC}"
