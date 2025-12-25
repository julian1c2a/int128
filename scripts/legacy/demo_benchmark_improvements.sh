#!/usr/bin/env bash
#
# demo_benchmark_improvements.sh
# Script de demostración de las nuevas mejoras del sistema de benchmarks
#

set -e  # Exit on error

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "${SCRIPT_DIR}")"

echo -e "${MAGENTA}========================================${NC}"
echo -e "${MAGENTA}DEMO: Mejoras del Sistema de Benchmarks${NC}"
echo -e "${MAGENTA}========================================${NC}\n"

# Función para pausar
pause() {
    echo -e "\n${YELLOW}Presiona Enter para continuar...${NC}"
    read -r
}

# 1. Demostración de __uint128_t
echo -e "${CYAN}>>> Demo 1: Benchmark con __uint128_t nativo (GCC/Clang)${NC}"
echo -e "Compilando benchmark con soporte para __uint128_t..."
echo -e "${BLUE}Comando: ./scripts/build_with_backends.bash gcc${NC}"

if command -v g++ &> /dev/null; then
    cd "${PROJECT_ROOT}"
    ./scripts/build_with_backends.bash gcc
    echo -e "${GREEN}✓ Compilación exitosa con __uint128_t${NC}"
    
    echo -e "\nEjecutando benchmark..."
    ./build/benchmarks/benchmark_gcc | head -n 30
    echo -e "${GREEN}✓ Benchmark completado${NC}"
else
    echo -e "${YELLOW}GCC no disponible, saltando demo 1${NC}"
fi

pause

# 2. Demostración de backends de Boost
echo -e "\n${CYAN}>>> Demo 2: Backends de Boost.Multiprecision${NC}"
echo -e "Compilando con todos los backends (cpp_int, GMP, tommath)..."
echo -e "${BLUE}Comando: ./scripts/build_with_backends.bash gcc --all-backends${NC}"

if [ -d "/usr/include/boost" ] || [ -d "/ucrt64/include/boost" ]; then
    cd "${PROJECT_ROOT}"
    ./scripts/build_with_backends.bash gcc --all-backends 2>&1 | head -n 20
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Compilación exitosa con backends de Boost${NC}"
    else
        echo -e "${YELLOW}⚠ Algunos backends no disponibles (falta libgmp o libtommath)${NC}"
        echo -e "${YELLOW}Instalar con: pacman -S mingw-w64-ucrt-x86_64-gmp mingw-w64-ucrt-x86_64-libtommath${NC}"
    fi
else
    echo -e "${YELLOW}Boost no encontrado, saltando demo 2${NC}"
    echo -e "${YELLOW}Instalar con: pacman -S mingw-w64-ucrt-x86_64-boost${NC}"
fi

pause

# 3. Demostración de comparación multi-compilador
echo -e "\n${CYAN}>>> Demo 3: Comparación Multi-Compilador${NC}"
echo -e "Compilando con múltiples compiladores..."

cd "${PROJECT_ROOT}"

# GCC
if command -v g++ &> /dev/null; then
    echo -e "\n${BLUE}Compilando con GCC...${NC}"
    ./scripts/build_benchmarks.bash gcc > /dev/null 2>&1 && echo -e "${GREEN}✓ GCC OK${NC}"
fi

# Clang
if command -v clang++ &> /dev/null; then
    echo -e "${BLUE}Compilando con Clang...${NC}"
    ./scripts/build_benchmarks.bash clang > /dev/null 2>&1 && echo -e "${GREEN}✓ Clang OK${NC}"
fi

# Intel
if command -v icpx &> /dev/null || command -v icx &> /dev/null; then
    echo -e "${BLUE}Compilando con Intel...${NC}"
    ./scripts/build_benchmarks.bash intel > /dev/null 2>&1 && echo -e "${GREEN}✓ Intel OK${NC}"
fi

echo -e "\n${GREEN}Compiladores disponibles verificados${NC}"

pause

# 4. Demostración de ejecución de benchmarks
echo -e "\n${CYAN}>>> Demo 4: Ejecución y Agregación${NC}"
echo -e "Ejecutando benchmark completo..."

cd "${PROJECT_ROOT}"

if [ -f "build/benchmarks/benchmark_gcc" ]; then
    echo -e "${BLUE}Ejecutando benchmark_gcc...${NC}"
    ./build/benchmarks/benchmark_gcc > /dev/null 2>&1
    echo -e "${GREEN}✓ Benchmark ejecutado${NC}"
    
    echo -e "\nResultados guardados en:"
    ls -lh benchmark_results/*.csv 2>/dev/null | tail -n 3
fi

pause

# 5. Demostración de agregación
echo -e "\n${CYAN}>>> Demo 5: Agregación de Resultados${NC}"
echo -e "Consolidando resultados de múltiples compiladores..."

cd "${PROJECT_ROOT}"

if [ -f "scripts/aggregate_benchmark_results.py" ]; then
    echo -e "${BLUE}Ejecutando aggregate_benchmark_results.py...${NC}"
    python scripts/aggregate_benchmark_results.py 2>&1 | tail -n 20
    
    if [ -f "benchmark_results/benchmark_summary.md" ]; then
        echo -e "\n${GREEN}✓ Reporte Markdown generado${NC}"
        echo -e "${CYAN}Vista previa del reporte:${NC}\n"
        head -n 30 benchmark_results/benchmark_summary.md
    fi
else
    echo -e "${YELLOW}Script de agregación no encontrado${NC}"
fi

pause

# 6. Demostración de gráficos
echo -e "\n${CYAN}>>> Demo 6: Generación de Gráficos con matplotlib${NC}"

# Verificar matplotlib
python -c "import matplotlib" 2>/dev/null
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ matplotlib disponible${NC}"
    
    cd "${PROJECT_ROOT}"
    
    if [ -f "scripts/plot_benchmark_results.py" ]; then
        echo -e "\n${BLUE}Generando gráficos...${NC}"
        python scripts/plot_benchmark_results.py 2>&1 | tail -n 15
        
        if [ -d "benchmark_results/plots" ]; then
            echo -e "\n${GREEN}✓ Gráficos generados:${NC}"
            ls -lh benchmark_results/plots/*.png 2>/dev/null | head -n 10
            
            echo -e "\n${CYAN}Tipos de gráficos generados:${NC}"
            echo -e "  - time_*.png: Comparación de tiempos por operación"
            echo -e "  - cycles_*.png: Comparación de ciclos de CPU"
            echo -e "  - compiler_*.png: Comparación entre compiladores"
            echo -e "  - speedup_heatmap.png: Mapa de calor de speedup"
            echo -e "  - operations_overview.png: Vista general de operaciones"
        fi
    fi
else
    echo -e "${YELLOW}matplotlib no disponible${NC}"
    echo -e "${YELLOW}Instalar con: pip install matplotlib numpy${NC}"
fi

pause

# 7. Resumen de mejoras
echo -e "\n${MAGENTA}========================================${NC}"
echo -e "${MAGENTA}RESUMEN DE MEJORAS IMPLEMENTADAS${NC}"
echo -e "${MAGENTA}========================================${NC}\n"

echo -e "${CYAN}1. Soporte para __uint128_t${NC}"
echo -e "   ✓ Tipo nativo de 128 bits en GCC/Clang"
echo -e "   ✓ Comparación directa con implementación hardware"
echo -e "   ✓ Incluido en todas las operaciones (aritmética, bitwise, shifts, etc.)\n"

echo -e "${CYAN}2. Backends de Boost.Multiprecision${NC}"
echo -e "   ✓ cpp_int: Implementación pura C++"
echo -e "   ✓ GMP backend: Optimización con GNU MP"
echo -e "   ✓ tommath backend: Alternativa ligera"
echo -e "   ✓ Comparación de rendimiento entre backends\n"

echo -e "${CYAN}3. Visualización con matplotlib${NC}"
echo -e "   ✓ Gráficos de tiempo por operación"
echo -e "   ✓ Gráficos de ciclos de CPU"
echo -e "   ✓ Comparación entre compiladores"
echo -e "   ✓ Mapa de calor de speedup"
echo -e "   ✓ Vista general de todas las operaciones\n"

echo -e "${CYAN}4. Soporte para Compilador Intel${NC}"
echo -e "   ✓ Intel oneAPI (icpx)"
echo -e "   ✓ Intel Classic (icx/icc)"
echo -e "   ✓ Optimizaciones específicas Intel"
echo -e "   ✓ Integrado en build y run scripts\n"

# 8. Próximos pasos
echo -e "${YELLOW}========================================${NC}"
echo -e "${YELLOW}PRÓXIMOS PASOS${NC}"
echo -e "${YELLOW}========================================${NC}\n"

echo -e "Para ejecutar benchmarks completos:"
echo -e "  ${BLUE}./scripts/build_benchmarks.bash all${NC}"
echo -e "  ${BLUE}./scripts/run_benchmarks.bash all${NC}"
echo -e "  ${BLUE}python scripts/plot_benchmark_results.py${NC}\n"

echo -e "Para compilar con backends opcionales:"
echo -e "  ${BLUE}./scripts/build_with_backends.bash gcc --all-backends${NC}\n"

echo -e "Para ver documentación completa:"
echo -e "  ${BLUE}cat documentation/BENCHMARK_IMPROVEMENTS.md${NC}"
echo -e "  ${BLUE}cat documentation/BENCHMARK_GUIDE.md${NC}\n"

echo -e "Próxima fase: ${CYAN}CI/CD Integration${NC}"
echo -e "  - GitHub Actions para benchmarks automáticos"
echo -e "  - Detección de regresiones de performance"
echo -e "  - Tracking histórico de resultados\n"

echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}DEMO COMPLETADA${NC}"
echo -e "${GREEN}========================================${NC}\n"
