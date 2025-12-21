#!/bin/bash
# Script de ejecución para int128_traits tests y benchmarks
# Ejecuta los tests y benchmarks compilados con los 4 compiladores
# Genera reportes consolidados en benchmark_results/

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

# Crear directorio de resultados con timestamp
TIMESTAMP=$(date +%Y%m%d_%H%M%S)
RESULTS_DIR="benchmark_results"
mkdir -p "$RESULTS_DIR"

echo -e "${BLUE}=============================================================${NC}"
echo -e "${BLUE}  Ejecución de int128_traits Tests y Benchmarks${NC}"
echo -e "${BLUE}=============================================================${NC}\n"

# =============================================================================
# EJECUTAR TESTS
# =============================================================================
echo -e "${YELLOW}============================================================${NC}"
echo -e "${YELLOW}  FASE 1: Ejecutando Tests${NC}"
echo -e "${YELLOW}============================================================${NC}\n"

TESTS_RUN=0
TESTS_PASSED=0

# Test con GCC
if [ -f "build/build_tests/gcc/release/int128_traits_tests_gcc.exe" ]; then
    echo -e "${CYAN}[1/4] Tests con GCC...${NC}"
    if ./build/build_tests/gcc/release/int128_traits_tests_gcc.exe > "$RESULTS_DIR/tests_gcc_$TIMESTAMP.txt" 2>&1; then
        echo -e "${GREEN}  ✅ Tests exitosos (guardado en tests_gcc_$TIMESTAMP.txt)${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}  ❌ Algunos tests fallaron (ver tests_gcc_$TIMESTAMP.txt)${NC}"
    fi
    TESTS_RUN=$((TESTS_RUN + 1))
else
    echo -e "${RED}[1/4] Tests con GCC no disponible${NC}"
fi

# Test con Clang
if [ -f "build/build_tests/clang/release/int128_traits_tests_clang.exe" ]; then
    echo -e "${CYAN}[2/4] Tests con Clang...${NC}"
    if ./build/build_tests/clang/release/int128_traits_tests_clang.exe > "$RESULTS_DIR/tests_clang_$TIMESTAMP.txt" 2>&1; then
        echo -e "${GREEN}  ✅ Tests exitosos (guardado en tests_clang_$TIMESTAMP.txt)${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}  ❌ Algunos tests fallaron (ver tests_clang_$TIMESTAMP.txt)${NC}"
    fi
    TESTS_RUN=$((TESTS_RUN + 1))
else
    echo -e "${RED}[2/4] Tests con Clang no disponible${NC}"
fi

# Test con Intel
if [ -f "build/build_tests/intel/release/int128_traits_tests_intel.exe" ]; then
    echo -e "${CYAN}[3/4] Tests con Intel...${NC}"
    if ./build/build_tests/intel/release/int128_traits_tests_intel.exe > "$RESULTS_DIR/tests_intel_$TIMESTAMP.txt" 2>&1; then
        echo -e "${GREEN}  ✅ Tests exitosos (guardado en tests_intel_$TIMESTAMP.txt)${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}  ❌ Algunos tests fallaron (ver tests_intel_$TIMESTAMP.txt)${NC}"
    fi
    TESTS_RUN=$((TESTS_RUN + 1))
else
    echo -e "${RED}[3/4] Tests con Intel no disponible${NC}"
fi

# Test con MSVC
if [ -f "build/build_tests/msvc/release/int128_traits_tests_msvc.exe" ]; then
    echo -e "${CYAN}[4/4] Tests con MSVC...${NC}"
    if ./build/build_tests/msvc/release/int128_traits_tests_msvc.exe > "$RESULTS_DIR/tests_msvc_$TIMESTAMP.txt" 2>&1; then
        echo -e "${GREEN}  ✅ Tests exitosos (guardado en tests_msvc_$TIMESTAMP.txt)${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}  ❌ Algunos tests fallaron (ver tests_msvc_$TIMESTAMP.txt)${NC}"
    fi
    TESTS_RUN=$((TESTS_RUN + 1))
else
    echo -e "${RED}[4/4] Tests con MSVC no disponible${NC}"
fi

echo -e "\n${CYAN}Resumen de Tests: $TESTS_PASSED/$TESTS_RUN exitosos${NC}"

if [ $TESTS_PASSED -lt $TESTS_RUN ]; then
    echo -e "${RED}ADVERTENCIA: Algunos tests fallaron. Continuando con benchmarks...${NC}\n"
fi

# =============================================================================
# EJECUTAR BENCHMARKS
# =============================================================================
echo -e "\n${YELLOW}============================================================${NC}"
echo -e "${YELLOW}  FASE 2: Ejecutando Benchmarks${NC}"
echo -e "${YELLOW}============================================================${NC}\n"

BENCHS_RUN=0

# Benchmark con GCC
if [ -f "build/build_benchmarks/gcc/release/int128_traits_benchs_gcc.exe" ]; then
    echo -e "${GREEN}[1/4] Benchmarks con GCC...${NC}"
    ./build/build_benchmarks/gcc/release/int128_traits_benchs_gcc.exe > "$RESULTS_DIR/benchmarks_gcc_$TIMESTAMP.txt" 2>&1
    echo -e "${GREEN}  ✅ Completado (guardado en benchmarks_gcc_$TIMESTAMP.txt)${NC}"
    BENCHS_RUN=$((BENCHS_RUN + 1))
else
    echo -e "${RED}[1/4] Benchmarks con GCC no disponible${NC}"
fi

# Benchmark con Clang
if [ -f "build/build_benchmarks/clang/release/int128_traits_benchs_clang.exe" ]; then
    echo -e "${GREEN}[2/4] Benchmarks con Clang...${NC}"
    ./build/build_benchmarks/clang/release/int128_traits_benchs_clang.exe > "$RESULTS_DIR/benchmarks_clang_$TIMESTAMP.txt" 2>&1
    echo -e "${GREEN}  ✅ Completado (guardado en benchmarks_clang_$TIMESTAMP.txt)${NC}"
    BENCHS_RUN=$((BENCHS_RUN + 1))
else
    echo -e "${RED}[2/4] Benchmarks con Clang no disponible${NC}"
fi

# Benchmark con Intel
if [ -f "build/build_benchmarks/intel/release/int128_traits_benchs_intel.exe" ]; then
    echo -e "${GREEN}[3/4] Benchmarks con Intel...${NC}"
    ./build/build_benchmarks/intel/release/int128_traits_benchs_intel.exe > "$RESULTS_DIR/benchmarks_intel_$TIMESTAMP.txt" 2>&1
    echo -e "${GREEN}  ✅ Completado (guardado en benchmarks_intel_$TIMESTAMP.txt)${NC}"
    BENCHS_RUN=$((BENCHS_RUN + 1))
else
    echo -e "${RED}[3/4] Benchmarks con Intel no disponible${NC}"
fi

# Benchmark con MSVC
if [ -f "build/build_benchmarks/msvc/release/int128_traits_benchs_msvc.exe" ]; then
    echo -e "${GREEN}[4/4] Benchmarks con MSVC...${NC}"
    ./build/build_benchmarks/msvc/release/int128_traits_benchs_msvc.exe > "$RESULTS_DIR/benchmarks_msvc_$TIMESTAMP.txt" 2>&1
    echo -e "${GREEN}  ✅ Completado (guardado en benchmarks_msvc_$TIMESTAMP.txt)${NC}"
    BENCHS_RUN=$((BENCHS_RUN + 1))
else
    echo -e "${RED}[4/4] Benchmarks con MSVC no disponible${NC}"
fi

# =============================================================================
# EXTRAER Y CONSOLIDAR RESULTADOS CSV
# =============================================================================
echo -e "\n${YELLOW}============================================================${NC}"
echo -e "${YELLOW}  FASE 3: Consolidando Resultados${NC}"
echo -e "${YELLOW}============================================================${NC}\n"

SUMMARY_CSV="$RESULTS_DIR/summary_$TIMESTAMP.csv"
REPORT_TXT="$RESULTS_DIR/report_$TIMESTAMP.txt"

# Crear encabezado CSV
echo "Compilador,Benchmark,Tiempo_ns,Ciclos" > "$SUMMARY_CSV"

# Función para extraer datos de benchmarks
extract_benchmark_data() {
    local compiler=$1
    local file=$2

    if [ -f "$file" ]; then
        grep -A 2 "^\[" "$file" | grep -E "(Tiempo promedio|Ciclos promedio)" | \
        awk -v comp="$compiler" '
        /^\[/ { benchmark=$0; gsub(/^\[|\]$/, "", benchmark) }
        /Tiempo promedio:/ { time=$3 }
        /Ciclos promedio:/ { cycles=$3; print comp","benchmark","time","cycles }
        ' >> "$SUMMARY_CSV" 2>/dev/null
    fi
}

# Extraer datos de cada compilador
extract_benchmark_data "GCC" "$RESULTS_DIR/benchmarks_gcc_$TIMESTAMP.txt"
extract_benchmark_data "Clang" "$RESULTS_DIR/benchmarks_clang_$TIMESTAMP.txt"
extract_benchmark_data "Intel" "$RESULTS_DIR/benchmarks_intel_$TIMESTAMP.txt"
extract_benchmark_data "MSVC" "$RESULTS_DIR/benchmarks_msvc_$TIMESTAMP.txt"

echo -e "${GREEN}✅ Datos consolidados en $SUMMARY_CSV${NC}"

# =============================================================================
# GENERAR REPORTE DE TEXTO
# =============================================================================
cat > "$REPORT_TXT" << EOF
=============================================================================
  REPORTE DE int128_traits TESTS Y BENCHMARKS
=============================================================================
Fecha: $(date '+%Y-%m-%d %H:%M:%S')
Directorio: $PROJECT_DIR

-----------------------------------------------------------------------------
RESUMEN DE EJECUCIÓN
-----------------------------------------------------------------------------
Tests ejecutados: $TESTS_RUN
Tests exitosos: $TESTS_PASSED
Benchmarks ejecutados: $BENCHS_RUN

-----------------------------------------------------------------------------
ARCHIVOS GENERADOS
-----------------------------------------------------------------------------
EOF

ls -lh "$RESULTS_DIR"/*_$TIMESTAMP.* >> "$REPORT_TXT"

cat >> "$REPORT_TXT" << EOF

-----------------------------------------------------------------------------
RESULTADOS DE BENCHMARKS (ver $SUMMARY_CSV para datos completos)
-----------------------------------------------------------------------------
EOF

# Mostrar top 5 mejores resultados por tiempo
if [ -s "$SUMMARY_CSV" ]; then
    echo -e "\nTop 5 mejores tiempos (nanosegundos):" >> "$REPORT_TXT"
    tail -n +2 "$SUMMARY_CSV" | sort -t',' -k3 -n | head -5 >> "$REPORT_TXT"

    echo -e "\nTop 5 mejores ciclos:" >> "$REPORT_TXT"
    tail -n +2 "$SUMMARY_CSV" | sort -t',' -k4 -n | head -5 >> "$REPORT_TXT"
fi

echo -e "${GREEN}✅ Reporte generado en $REPORT_TXT${NC}"

# =============================================================================
# RESUMEN FINAL
# =============================================================================
echo -e "\n${BLUE}=============================================================${NC}"
echo -e "${BLUE}                    PROCESO COMPLETADO${NC}"
echo -e "${BLUE}=============================================================${NC}\n"

echo -e "${CYAN}📁 Resultados guardados en:${NC} $RESULTS_DIR/"
echo -e "${CYAN}📊 Resumen CSV:${NC} $SUMMARY_CSV"
echo -e "${CYAN}📝 Reporte:${NC} $REPORT_TXT\n"

if [ $TESTS_PASSED -eq $TESTS_RUN ] && [ $BENCHS_RUN -gt 0 ]; then
    echo -e "${GREEN}✅ PROCESO EXITOSO${NC}\n"
    exit 0
elif [ $BENCHS_RUN -gt 0 ]; then
    echo -e "${YELLOW}⚠️  Proceso completado con advertencias${NC}\n"
    exit 0
else
    echo -e "${RED}❌ No se ejecutaron benchmarks${NC}\n"
    exit 1
fi
