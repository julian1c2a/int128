# General Demos - int128 Library

Demostraciones generales, experimentales y ad-hoc que no encajan en otras categorías.

## 📂 Contenido

### Demos de Simetría de Operadores
- **`demo_symmetry_fixed.cpp`** - Demostración de operadores simétricos corregidos
- **`demo_symmetry_complete.cpp`** - Suite completa de pruebas de simetría

### Demos de Conversiones
- **`demo_bytes_bitset.cpp`** - Conversiones entre bytes, bitset y uint128_t/int128_t
- **`demo_constexpr_cstr.cpp`** - Funciones constexpr con C-strings

### Análisis
- **`bit_analysis.cpp`** - Análisis de operaciones bitwise
- **`analysis_summary.cpp`** - Resumen de análisis de rendimiento

## 🚀 Compilación

```bash
# Desde la raíz del proyecto
g++ -std=c++20 -O2 -I include demos/general/demo_bytes_bitset.cpp -o build/demos/general/demo_bytes_bitset

# O con el sistema de scripts (cuando esté implementado)
make build_demo CATEGORY=general DEMO=demo_bytes_bitset
```

## 📝 Notas

Esta categoría contiene demos que:
- Prueban características específicas
- Son experimentales o prototipo
- No tienen un caso de uso claro en otras categorías
- Están en desarrollo o investigación
