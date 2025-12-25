#!/bin/bash

# Script para ejecutar tests adicionales de uint128_t con todos los compiladores

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo "==============================================="
echo "Ejecutando tests adicionales de uint128_t con todos los compiladores"
echo "==============================================="
echo ""

# Compilar primero si es necesario
bash scripts/build_additional_tests.bash

echo ""
echo "==============================================="
echo "Ejecutando tests compilados"
echo "==============================================="
echo ""

# GCC
if [ -f "build/build_tests/gcc/release/test_uint128_additional.exe" ]; then
    echo -e "${BLUE}Ejecutando tests con GCC...${NC}"
    ./build/build_tests/gcc/release/test_uint128_additional.exe
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK] Tests GCC pasaron exitosamente${NC}"
    else
        echo -e "${RED}[ERROR] Tests GCC fallaron${NC}"
    fi
    echo ""
fi

# Clang
if [ -f "build/build_tests/clang/release/uint128_additional_tests.exe" ]; then
    echo -e "${BLUE}Ejecutando tests con Clang...${NC}"
    ./build/build_tests/clang/release/uint128_additional_tests.exe
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK] Tests Clang pasaron exitosamente${NC}"
    else
        echo -e "${RED}[ERROR] Tests Clang fallaron${NC}"
    fi
    echo ""
fi

# MSVC
if [ -f "build/build_tests/msvc/release/uint128_additional_tests.exe" ]; then
    echo -e "${BLUE}Ejecutando tests con MSVC...${NC}"
    ./build/build_tests/msvc/release/uint128_additional_tests.exe
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK] Tests MSVC pasaron exitosamente${NC}"
    else
        echo -e "${RED}[ERROR] Tests MSVC fallaron${NC}"
    fi
    echo ""
fi

echo "==============================================="
echo "Ejecución de tests completada"
echo "==============================================="
