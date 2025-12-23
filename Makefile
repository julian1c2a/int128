#!/usr/bin/make -f
# Makefile unificado para int128/uint128
# Uso: make [action] TYPE=[type] FEATURE=[feature] COMPILER=[compiler] MODE=[mode] PRINT=[print]

.PHONY: help build_tests build_benchs check run clean
.DEFAULT_GOAL := help

# =============================================================================
# VARIABLES
# =============================================================================

# Valores por defecto
TYPE ?=
FEATURE ?=
COMPILER ?= all
MODE ?= all
PRINT ?=

# Validación
VALID_TYPES := uint128 int128
VALID_FEATURES := concepts limits algorithm cmath traits bits thread_safety
VALID_COMPILERS := gcc clang intel msvc all
VALID_MODES := debug release all

# =============================================================================
# VALIDACIÓN
# =============================================================================

define validate
	@if [ -z "$(TYPE)" ]; then \
		echo "❌ ERROR: Falta TYPE"; \
		echo "Uso: make $(1) TYPE=[uint128|int128] FEATURE=[concepts|limits|...] COMPILER=[gcc|clang|intel|msvc|all] MODE=[debug|release|all]"; \
		exit 1; \
	fi; \
	if [ -z "$(FEATURE)" ]; then \
		echo "❌ ERROR: Falta FEATURE"; \
		echo "Uso: make $(1) TYPE=[uint128|int128] FEATURE=[concepts|limits|...] COMPILER=[gcc|clang|intel|msvc|all] MODE=[debug|release|all]"; \
		exit 1; \
	fi
endef

# =============================================================================
# TARGETS PRINCIPALES
# =============================================================================

build_tests:
	$(call validate,build_tests)
	@echo "🔨 Compilando tests: $(TYPE) $(FEATURE) [$(COMPILER)/$(MODE)]"
	@bash scripts/build_$(TYPE)_$(FEATURE)_extracted_tests.bash $(COMPILER) $(MODE) $(PRINT)

build_benchs:
	$(call validate,build_benchs)
	@echo "🔨 Compilando benchmarks: $(TYPE) $(FEATURE) [$(COMPILER)/$(MODE)]"
	@bash scripts/build_$(TYPE)_$(FEATURE)_extracted_benchs.bash $(COMPILER) $(MODE) $(PRINT)

check:
	$(call validate,check)
	@echo "🧪 Ejecutando tests: $(TYPE) $(FEATURE) [$(COMPILER)/$(MODE)]"
	@bash scripts/check_$(TYPE)_$(FEATURE)_extracted_tests.bash $(COMPILER) $(MODE) $(PRINT)

run:
	$(call validate,run)
	@echo "⚡ Ejecutando benchmarks: $(TYPE) $(FEATURE) [$(COMPILER)/$(MODE)]"
	@bash scripts/run_$(TYPE)_$(FEATURE)_extracted_benchs.bash $(COMPILER) $(MODE)

# =============================================================================
# ATAJOS ÚTILES PARA CONCEPTS
# =============================================================================

.PHONY: build-all-concepts check-all-concepts run-all-concepts concepts-full

build-all-concepts:
	@echo "========================================="
	@echo " Building ALL concepts (tests + benchs)"
	@echo "========================================="
	@$(MAKE) build_tests TYPE=uint128 FEATURE=concepts COMPILER=all MODE=all
	@$(MAKE) build_tests TYPE=int128 FEATURE=concepts COMPILER=all MODE=all
	@$(MAKE) build_benchs TYPE=uint128 FEATURE=concepts COMPILER=all MODE=all
	@$(MAKE) build_benchs TYPE=int128 FEATURE=concepts COMPILER=all MODE=all

check-all-concepts:
	@echo "========================================="
	@echo " Checking ALL concepts tests"
	@echo "========================================="
	@$(MAKE) check TYPE=uint128 FEATURE=concepts COMPILER=all MODE=all
	@$(MAKE) check TYPE=int128 FEATURE=concepts COMPILER=all MODE=all

run-all-concepts:
	@echo "========================================="
	@echo " Running ALL concepts benchmarks"
	@echo "========================================="
	@$(MAKE) run TYPE=uint128 FEATURE=concepts COMPILER=all MODE=all
	@$(MAKE) run TYPE=int128 FEATURE=concepts COMPILER=all MODE=all

concepts-full: build-all-concepts check-all-concepts run-all-concepts
	@echo "========================================="
	@echo " ✅ CONCEPTS: Pipeline completo"
	@echo "========================================="

# =============================================================================
# LIMPIEZA
# =============================================================================

.PHONY: clean clean-build clean-results

clean-build:
	@echo "🧹 Limpiando directorios de build..."
	@rm -rf build/build_tests build/build_benchmarks
	@echo "✅ Limpio"

clean-results:
	@echo "🧹 Limpiando resultados..."
	@rm -rf build/build_tests_results benchmarks_results
	@echo "✅ Limpio"

clean: clean-build clean-results
	@echo "✅ Limpieza completa"

# =============================================================================
# AYUDA
# =============================================================================

.PHONY: help

help:
	@echo "========================================="
	@echo " INT128/UINT128 - Build System"
	@echo "========================================="
	@echo ""
	@echo "USO:"
	@echo "  make [action] TYPE=[type] FEATURE=[feature] COMPILER=[compiler] MODE=[mode] PRINT=[print]"
	@echo ""
	@echo "ACCIONES:"
	@echo "  build_tests   - Compilar tests"
	@echo "  build_benchs  - Compilar benchmarks"
	@echo "  check         - Ejecutar tests"
	@echo "  run           - Ejecutar benchmarks"
	@echo ""
	@echo "ARGUMENTOS:"
	@echo "  TYPE          uint128 | int128 (requerido)"
	@echo "  FEATURE       concepts | limits | algorithm | cmath (requerido)"
	@echo "  COMPILER      gcc | clang | intel | msvc | all (default: all)"
	@echo "  MODE          debug | release | all (default: all)"
	@echo "  PRINT         print (opcional, solo para tests)"
	@echo ""
	@echo "EJEMPLOS:"
	@echo "  make build_tests TYPE=uint128 FEATURE=concepts COMPILER=gcc MODE=debug"
	@echo "  make check TYPE=int128 FEATURE=concepts COMPILER=all MODE=all PRINT=print"
	@echo "  make run TYPE=uint128 FEATURE=concepts COMPILER=intel MODE=release"
	@echo ""
	@echo "ATAJOS CONCEPTS:"
	@echo "  make build-all-concepts  - Build completo concepts (tests+benchs)"
	@echo "  make check-all-concepts  - Ejecutar todos los tests concepts"
	@echo "  make run-all-concepts    - Ejecutar todos los benchs concepts"
	@echo "  make concepts-full       - Pipeline completo concepts"
	@echo ""
	@echo "LIMPIEZA:"
	@echo "  make clean-build         - Limpiar compilaciones"
	@echo "  make clean-results       - Limpiar resultados"
	@echo "  make clean               - Limpiar todo"
	@echo ""
	@echo "========================================="
