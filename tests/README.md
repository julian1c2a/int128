# Tests

Este directorio contiene todas las pruebas unitarias y de funcionalidad para los tipos `uint128_t` e `int128_t`.

## Tests Principales (Solución Final)

### `test_organized_structure.cpp` ⭐
- **Test principal** de la estructura organizada
- Verifica el header unificado `int128.hpp`
- Comprueba los 3 enfoques: constructores + factory + assign

### `test_optimal_balance.cpp` ⭐
- **Test del balance óptimo** logrado
- Verifica `trivially_copyable` + `standard_layout`
- Demuestra flexibilidad de uso

## Tests por Categoría

### Tests de Funcionalidad Core
- `basic_test.cpp` - Funcionalidad básica
- `simple_test.cpp` - Operaciones simples
- `test_int128_basic.cpp` - Funcionalidad básica int128_t
- `test_uint128.cpp` - Funcionalidad básica uint128_t

### Tests de Traits y Propiedades
- `traits_test.cpp` - Tests básicos de traits
- `traits_verification_test.cpp` - Verificación de traits
- `basic_traits_test.cpp` - Traits fundamentales  
- `test_int128_traits_limits.cpp` - Traits y límites específicos

### Tests de Extensiones
- `test_factory.cpp` - Factory functions
- `test_factory_simple.cpp` - Factory functions básicas
- `test_int128_algorithm.cpp` - Funciones de algoritmos
- `test_int128_numeric.cpp` - Funciones numéricas
- `test_min_max.cpp` - std::min/max cross-type

### Tests de Compatibilidad STL
- `test_int128_ecosystem.cpp` - Integración con ecosistema STL
- `test_range_functions.cpp` - Funciones de rango
- `iota_test.cpp` - Tests de std::iota

## Tests Especializados

### Formato y I/O
- `test_formatting_demo.cpp` - Demostración de formateo
- `string_constructor_test.cpp` - Constructores desde string
- `user_literals_test.cpp` - Literales definidos por usuario

### Tests de Conceptos y Seguridad
- `test_int128_concepts.cpp` - C++20 concepts
- `test_int128_safe.cpp` - Operaciones seguras
- `test_data_compatibility.cpp` - Compatibilidad de datos

## Compilación y Ejecución

```bash
# Test principal de la solución final
g++ -I ../include -std=c++20 -O2 test_organized_structure.cpp -o test_main.exe

# Test del balance óptimo
g++ -I ../include -std=c++20 -O2 test_optimal_balance.cpp -o test_balance.exe

# Compilar test específico
g++ -I ../include -std=c++20 -O2 [archivo_test].cpp -o [archivo_test].exe
```

## Estado de los Tests

- ✅ **Tests principales funcionando**: estructura organizada + balance óptimo
- ✅ **Propiedades verificadas**: `trivially_copyable` + `standard_layout`
- ✅ **Tres enfoques validados**: constructores + factory + assign  
- ✅ **Compatibilidad STL**: traits + std::min/max cross-type