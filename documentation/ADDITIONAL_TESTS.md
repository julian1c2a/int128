# Tests Adicionales para uint128_t

Este documento describe los tests adicionales implementados para validar la robustez de la clase `uint128_t`.

## Archivo de Tests

**Archivo**: `tests/uint128_additional_tests.cpp`

## Cobertura de Tests

Los tests adicionales cubren las siguientes áreas críticas:

### 1. Semántica de Copia y Movimiento

#### test_copy_constructor
- Valida que el constructor de copia funciona correctamente
- Verifica que el objeto copiado es independiente del original

#### test_move_constructor
- Valida el constructor de movimiento
- Verifica que los valores se transfieren correctamente

#### test_copy_assignment
- Valida el operador de asignación por copia
- Verifica la asignación a sí mismo (self-assignment)

#### test_move_assignment
- Valida el operador de asignación por movimiento
- Verifica la asignación a sí mismo

### 2. Compatibilidad con STL

#### test_vector_of_uint128
- Verifica que `uint128_t` funciona correctamente en contenedores STL
- Prueba `std::vector` con push_back, acceso y modificación

### 3. Literales Definidos por el Usuario (UDL)

#### test_udl_integer
- Valida literales enteros: `1000_u128`
- Prueba casos límite como `0_u128`

#### test_udl_string
- Valida literales de string: `"12345"_u128`
- Verifica conversión correcta de cadenas

#### test_udl_specific_formats
- Valida formatos específicos:
  - Hexadecimal: `"0xFF"_u128_hex`
  - Binario: `"1010"_u128_bin`
  - Octal: `"777"_u128_oct`

### 4. Operadores de Flujo

#### test_ostream_operator
- Valida `operator<<` para salida a streams
- Verifica formato decimal correcto

#### test_istream_operator  
- Valida `operator>>` para entrada desde streams
- Prueba lectura de valores decimales

### 5. Casos Extremos y Límites

#### test_overflow_behavior
- Valida comportamiento en desbordamiento (overflow)
- Prueba suma que excede el máximo valor
- Verifica envoltura correcta (wrap-around)

#### test_underflow_behavior
- Valida comportamiento en desbordamiento inferior (underflow)
- Prueba resta que genera valor negativo
- Verifica envoltura desde cero

#### test_boundary_values
- Prueba valores límite:
  - Cero (`uint128_t::zero()`)
  - Máximo (`uint128_t::max()`)
- Verifica operaciones con valores límite

#### test_division_by_zero
- Valida que la división por cero no causa excepciones
- El comportamiento actual no lanza excepciones (como tipos nativos)

### 6. Operaciones Constexpr y Traits

#### test_constexpr_operations
- Valida que las operaciones básicas son `constexpr`
- Verifica evaluación en tiempo de compilación:
  - Construcción
  - Suma
  - Multiplicación

#### test_type_traits
- Valida traits del tipo:
  - `std::is_trivially_copyable`
  - `std::is_standard_layout`
  - `std::is_default_constructible`
  - `std::is_copy_constructible`
  - `std::is_move_constructible`
  - `std::is_copy_assignable`
  - `std::is_move_assignable`

### 7. Casos Extremos de Desplazamiento

#### test_shift_edge_cases
- Valida desplazamientos extremos:
  - Desplazamiento de 64 bits (límite entre high/low)
  - Desplazamiento de 128 bits (debe dar cero)
  - Desplazamiento mayor a 128 bits

### 8. Operaciones con Tipos Mixtos

#### test_mixed_type_operations
- Valida operaciones entre `uint128_t` y tipos nativos:
  - `uint128_t` + `int`
  - `int` + `uint128_t`
  - `uint128_t` - `int`
  - `int` - `uint128_t`
- Verifica conversión implícita correcta

## Scripts de Compilación y Ejecución

### Compilar Tests Adicionales
```bash
bash scripts/build_additional_tests.bash
```

Este script compila los tests con:
- **GCC**: Usando g++ con flags `-std=c++20 -O2 -Wall -Wextra`
- **Clang**: Usando clang++ con flags similares
- **MSVC**: Usando cl.exe con flags `/std:c++20 /O2 /W4 /EHsc`

### Ejecutar Tests Adicionales
```bash
bash scripts/run_uint128_additional_tests.bash
```

Este script:
1. Compila los tests (si es necesario)
2. Ejecuta los tests compilados con cada compilador
3. Muestra resultados con colores

### Compilar y Ejecutar con MSVC (Windows)
```cmd
scripts\test_additional_msvc.bat
```

## Resumen de Resultados

✅ **INTEGRADOS EN `tests/uint128_extracted_tests.cpp`**

Los 18 tests adicionales han sido completamente integrados en el archivo principal de tests. Ahora todos los 84 tests (66 originales + 18 adicionales) se ejecutan juntos con:

```bash
bash scripts/test_extracted.bash
# o
./test.bash
```

Todos los tests pasan exitosamente en:
- ✅ **GCC** (84/84 tests - MSYS2/UCRT64)
- ✅ **Clang** (84/84 tests - MSYS2/UCRT64, 1 warning esperado)  
- ✅ **MSVC** (84/84 tests - con activación automática)

## Integración con CI/CD

Los tests integrados pueden utilizarse fácilmente en pipelines de CI/CD:

```bash
# En tu pipeline - Un solo comando
bash scripts/test_extracted.bash all
```

## Notas de Implementación

### Warnings en Clang
Clang genera un warning esperado en `test_copy_assignment`:
```
warning: explicitly assigning value of variable of type 'uint128_t' to itself [-Wself-assign-overloaded]
```

Este warning es intencional, ya que estamos probando la asignación a sí mismo (self-assignment).

### Constexpr en GCC/Clang vs MSVC
Algunos tests usan `std::is_constant_evaluated()` para validar evaluación en tiempo de compilación. El comportamiento puede variar ligeramente entre compiladores.

### División por Cero
El comportamiento actual de división por cero no lanza excepciones, similar a los tipos nativos de C++. El test valida que la operación se completa sin excepciones.

## Tests Complementarios

Estos tests son complementarios a los tests principales en:
- `tests/uint128_extracted_tests.cpp` (66 tests)

Total de cobertura: **84 tests** entre ambos archivos.
