#!/bin/bash
# Script maestro para compilar y ejecutar todo el proceso de int128_traits tests y benchmarks
# Este script hace todo el proceso automaticamente

set -e

PROJECT_DIR="/c/msys64/ucrt64/home/julian/CppProjects/int128"
cd "$PROJECT_DIR"

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo -e "${BLUE}============================================================${NC}"
echo -e "${BLUE}  int128_traits.hpp - Proceso Completo${NC}"
echo -e "${BLUE}  Tests + Benchmarks + 4 Compiladores${NC}"
echo -e "${BLUE}============================================================${NC}\n"

# Hacer scripts ejecutables
echo -e "${YELLOW}Preparando scripts...${NC}"
chmod +x scripts/compile_int128_traits_extracted.sh
chmod +x scripts/run_int128_traits_benchmarks.sh
echo -e "${GREEN}✅ Scripts preparados${NC}\n"

# FASE 1: Compilación
echo -e "${BLUE}============================================================${NC}"
echo -e "${BLUE}  FASE 1: Compilación con 4 compiladores${NC}"
echo -e "${BLUE}============================================================${NC}\n"

if ./scripts/compile_int128_traits_extracted.sh; then
    echo -e "\n${GREEN}✅ Compilación completada exitosamente${NC}\n"
else
    echo -e "\n${RED}❌ Error en la compilación${NC}\n"
    exit 1
fi

# FASE 2: Ejecución de tests y benchmarks
echo -e "${BLUE}============================================================${NC}"
echo -e "${BLUE}  FASE 2: Ejecución de tests y benchmarks${NC}"
echo -e "${BLUE}============================================================${NC}\n"

if ./scripts/run_int128_traits_benchmarks.sh; then
    echo -e "\n${GREEN}✅ Ejecución completada exitosamente${NC}\n"
else
    echo -e "\n${RED}❌ Error en la ejecución${NC}\n"
    exit 1
fi

# RESUMEN FINAL
echo -e "${BLUE}============================================================${NC}"
echo -e "${BLUE}                    PROCESO COMPLETADO${NC}"
echo -e "${BLUE}============================================================${NC}\n"

echo -e "${GREEN}✅ Todos los pasos completados exitosamente${NC}\n"
echo -e "${CYAN}Resultados disponibles en:${NC}"
echo -e "  📁 benchmark_results/\n"

echo -e "${CYAN}Para ver el reporte más reciente:${NC}"
LATEST_REPORT=$(ls -t benchmark_results/report_*.txt 2>/dev/null | head -n 1)
if [ -n "$LATEST_REPORT" ]; then
    echo -e "  cat $LATEST_REPORT\n"
    echo -e "${YELLOW}Vista previa del reporte:${NC}"
    echo -e "${CYAN}------------------------------------------------------------${NC}"
    head -n 30 "$LATEST_REPORT"
    echo -e "${CYAN}------------------------------------------------------------${NC}"
    echo -e "${YELLOW}(ver archivo completo para más detalles)${NC}\n"
fi

echo -e "${GREEN}¡Proceso completado!${NC}\n"
