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

# Validación (completo según PROMPT.md)
VALID_TYPES := uint128 int128
VALID_FEATURES := t traits limits concepts algorithms iostreams bits cmath numeric ranges format safe thread_safety
VALID_COMPILERS := gcc clang intel msvc all
VALID_MODES := debug release all

# =============================================================================
# VALIDACIÓN
# =============================================================================

define validate_values
	@if [ -z "$(TYPE)" ]; then \
		echo "❌ ERROR: Falta TYPE"; \
		echo "   Valores válidos: $(VALID_TYPES)"; \
		echo "Uso: make $(1) TYPE=[uint128|int128] FEATURE=[feature] COMPILER=[compiler] MODE=[mode]"; \
		exit 1; \
	fi; \
	if [ -z "$(FEATURE)" ]; then \
		echo "❌ ERROR: Falta FEATURE"; \
		echo "   Valores válidos: $(VALID_FEATURES)"; \
		echo "Uso: make $(1) TYPE=[uint128|int128] FEATURE=[feature] COMPILER=[compiler] MODE=[mode]"; \
		exit 1; \
	fi; \
	if ! echo "$(VALID_TYPES)" | grep -wq "$(TYPE)"; then \
		echo "❌ ERROR: TYPE inválido '$(TYPE)'"; \
		echo "   Valores válidos: $(VALID_TYPES)"; \
		exit 1; \
	fi; \
	if ! echo "$(VALID_FEATURES)" | grep -wq "$(FEATURE)"; then \
		echo "❌ ERROR: FEATURE inválido '$(FEATURE)'"; \
		echo "   Valores válidos: $(VALID_FEATURES)"; \
		exit 1; \
	fi; \
	if ! echo "$(VALID_COMPILERS)" | grep -wq "$(COMPILER)"; then \
		echo "❌ ERROR: COMPILER inválido '$(COMPILER)'"; \
		echo "   Valores válidos: $(VALID_COMPILERS)"; \
		exit 1; \
	fi; \
	if ! echo "$(VALID_MODES)" | grep -wq "$(MODE)"; then \
		echo "❌ ERROR: MODE inválido '$(MODE)'"; \
		echo "   Valores válidos: $(VALID_MODES)"; \
		exit 1; \
	fi
endef

# =============================================================================
# TARGETS PRINCIPALES
# =============================================================================

build_tests:
	$(call validate_values,build_tests)
	@echo "🔨 Compilando tests: $(TYPE) $(FEATURE) [$(COMPILER)/$(MODE)]"
	@bash scripts/build_$(TYPE)_$(FEATURE)_extracted_tests.bash $(COMPILER) $(MODE) $(PRINT)

build_benchs:
	$(call validate_values,build_benchs)
	@echo "🔨 Compilando benchmarks: $(TYPE) $(FEATURE) [$(COMPILER)/$(MODE)]"
	@bash scripts/build_$(TYPE)_$(FEATURE)_extracted_benchs.bash $(COMPILER) $(MODE) $(PRINT)

check:
	$(call validate_values,check)
	@echo "🧪 Ejecutando tests: $(TYPE) $(FEATURE) [$(COMPILER)/$(MODE)]"
	@bash scripts/check_$(TYPE)_$(FEATURE)_extracted_tests.bash $(COMPILER) $(MODE) $(PRINT)

run:
	$(call validate_values,run)
	@echo "⚡ Ejecutando benchmarks: $(TYPE) $(FEATURE) [$(COMPILER)/$(MODE)]"
	@bash scripts/run_$(TYPE)_$(FEATURE)_extracted_benchs.bash $(COMPILER) $(MODE) $(PRINT)

# =============================================================================
# SISTEMA DE ATAJOS GENÉRICOS POR FEATURE
# =============================================================================

# Función genérica para crear atajos automáticos para cualquier feature
define feature_shortcuts
.PHONY: build-all-$(1) check-all-$(1) run-all-$(1) $(1)-full

build-all-$(1):
	@echo "========================================="
	@echo " Building ALL $(1) (tests + benchs)"
	@echo "========================================="
	@$$(MAKE) build_tests TYPE=uint128 FEATURE=$(1) COMPILER=all MODE=all
	@$$(MAKE) build_tests TYPE=int128 FEATURE=$(1) COMPILER=all MODE=all
	@$$(MAKE) build_benchs TYPE=uint128 FEATURE=$(1) COMPILER=all MODE=all
	@$$(MAKE) build_benchs TYPE=int128 FEATURE=$(1) COMPILER=all MODE=all

check-all-$(1):
	@echo "========================================="
	@echo " Checking ALL $(1) tests"
	@echo "========================================="
	@$$(MAKE) check TYPE=uint128 FEATURE=$(1) COMPILER=all MODE=all
	@$$(MAKE) check TYPE=int128 FEATURE=$(1) COMPILER=all MODE=all

run-all-$(1):
	@echo "========================================="
	@echo " Running ALL $(1) benchmarks"
	@echo "========================================="
	@$$(MAKE) run TYPE=uint128 FEATURE=$(1) COMPILER=all MODE=all
	@$$(MAKE) run TYPE=int128 FEATURE=$(1) COMPILER=all MODE=all

$(1)-full: build-all-$(1) check-all-$(1) run-all-$(1)
	@echo "========================================="
	@echo " ✅ $(1): Pipeline completo"
	@echo "========================================="
endef

# Generar atajos automáticamente para TODAS las features
$(foreach feature,$(VALID_FEATURES),$(eval $(call feature_shortcuts,$(feature))))

# =============================================================================
# LIMPIEZA
# =============================================================================

.PHONY: clean clean-build clean-results clean-logs

clean-build:
	@echo "🧹 Limpiando directorios de build..."
	@rm -rf build/build_tests build/build_benchs
	@echo "✅ Build limpiado"

clean-results:
	@echo "🧹 Limpiando resultados..."
	@rm -rf build/build_tests_results build/build_benchs_results
	@echo "✅ Resultados limpiados"

clean-logs:
	@echo "🧹 Limpiando logs de compilación..."
	@rm -rf build_log
	@echo "✅ Logs limpiados"

clean: clean-build clean-results clean-logs
	@echo "✅ Limpieza completa"

# =============================================================================
# TARGETS ÚTILES
# =============================================================================

.PHONY: list-features list-compilers list-modes list-all build-all-features

list-features:
	@echo "Features disponibles:"
	@echo "$(VALID_FEATURES)" | tr ' ' '\n' | sed 's/^/  - /'

list-compilers:
	@echo "Compiladores disponibles:"
	@echo "$(VALID_COMPILERS)" | tr ' ' '\n' | sed 's/^/  - /'

list-modes:
	@echo "Modos disponibles:"
	@echo "$(VALID_MODES)" | tr ' ' '\n' | sed 's/^/  - /'

list-all: list-features list-compilers list-modes

# Compilar todas las features (uint128 e int128)
build-all-features:
	@echo "========================================="
	@echo " Building TODAS las features"
	@echo "========================================="
	@for feature in $(VALID_FEATURES); do \
		echo ""; \
		echo "▶ Building $$feature..."; \
		$(MAKE) build-all-$$feature || exit 1; \
	done
	@echo ""
	@echo "========================================="
	@echo " ✅ TODAS las features compiladas"
	@echo "========================================="

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
	@echo "  FEATURE       t | traits | limits | concepts | algorithms | iostreams"
	@echo "                bits | cmath | numeric | ranges | format | safe | thread_safety (requerido)"
	@echo "  COMPILER      gcc | clang | intel | msvc | all (default: all)"
	@echo "  MODE          debug | release | all (default: all)"
	@echo "  PRINT         print (opcional, genera logs/resultados en archivos)"
	@echo ""
	@echo "EJEMPLOS:"
	@echo "  make build_tests TYPE=uint128 FEATURE=concepts COMPILER=gcc MODE=debug"
	@echo "  make check TYPE=int128 FEATURE=concepts COMPILER=all MODE=all PRINT=print"
	@echo "  make run TYPE=uint128 FEATURE=cmath COMPILER=intel MODE=release PRINT=print"
	@echo ""
	@echo "ATAJOS POR FEATURE (disponibles para TODAS las features):"
	@echo "  make [feature]-full         - Pipeline completo (build+check+run)"
	@echo "  make build-all-[feature]    - Build tests+benchs de una feature"
	@echo "  make check-all-[feature]    - Ejecutar todos los tests de una feature"
	@echo "  make run-all-[feature]      - Ejecutar todos los benchs de una feature"
	@echo ""
	@echo "  Ejemplos:"
	@echo "    make concepts-full        make limits-full        make algorithms-full"
	@echo "    make cmath-full           make traits-full        make bits-full"
	@echo ""
	@echo "ATAJOS GLOBALES:"
	@echo "  make build-all-features     - Compilar TODAS las features"
	@echo "  make list-features          - Listar features disponibles"
	@echo "  make list-compilers         - Listar compiladores disponibles"
	@echo "  make list-modes             - Listar modos disponibles"
	@echo "  make list-all               - Listar todas las opciones"
	@echo ""
	@echo "LIMPIEZA:"
	@echo "  make clean-build            - Limpiar ejecutables compilados"
	@echo "  make clean-results          - Limpiar resultados de ejecución"
	@echo "  make clean-logs             - Limpiar logs de compilación"
	@echo "  make clean                  - Limpiar todo (build+results+logs)"
	@echo ""
	@echo "========================================="
