# uint128_t - Implementación de enteros de 128 bits

Una implementación completa y eficiente de enteros sin signo de 128 bits para C++20, compatible con MSVC, GCC y Clang.

## 📁 Estructura del Proyecto

```
int128/
├── include/                    # Headers públicos
│   ├── uint128_t.hpp          # Implementación principal
│   └── uint128_traits.hpp     # Extensiones STL
├── tests/                     # Tests unitarios
│   ├── basic_test.cpp
│   ├── user_literals_test.cpp
│   ├── string_constructor_test.cpp
│   └── ...
├── sources/                   # Código fuente de bibliotecas compiladas
├── build/                     # Archivos compilados
│   ├── test/
│   │   ├── debug/            # Tests compilados en modo debug
│   │   └── release/          # Tests compilados en modo release
│   └── lib/                  # Bibliotecas compiladas (futuro)
├── documentation/            # Documentación
│   ├── benchmarks/          # Resultados de benchmarks (.csv)
│   ├── api.md              # Documentación de API
│   └── examples.md         # Ejemplos de uso
└── scripts/                 # Scripts de build y utilidades
```

## 🛠️ Sistema de Build

### Scripts de Build

- `build_msvc.ps1` - Script PowerShell principal (recomendado para Windows)
- `build.ps1` - Script PowerShell original  
- `build.sh` - Script bash para entornos Unix-like (con cygpath)
- `msys_build.sh` - Script bash optimizado para MSYS2

### Uso del Sistema de Build

#### En PowerShell (Recomendado):
```powershell
# Compilar y ejecutar un test específico (debug)
.\build_msvc.ps1 user_literals_test debug

# Compilar en modo release
.\build_msvc.ps1 user_literals_test release
```

#### En terminal MSYS2/Bash:
```bash
# Compilar y ejecutar un test específico
./msys_build.sh user_literals_test debug

# Compilar en modo release  
./msys_build.sh user_literals_test release
```

### Estructura de Compilación

- **Debug**: `/Od /Zi /MDd /DDEBUG`
- **Release**: `/O2 /MD /DNDEBUG`
- **Output**: `build/test/{debug|release}/test_name.exe`

## 🔧 Características Implementadas

### ✅ Funcionalidades Básicas
- [x] Constructores (default, copia, move, desde integrales)
- [x] Operadores aritméticos (`+`, `-`, `*`, `/`, `%`)
- [x] Operadores de comparación (`==`, `!=`, `<`, `<=`, `>`, `>=`)
- [x] Operadores bit a bit (`&`, `|`, `^`, `<<`, `>>`, `~`)
- [x] Operadores de asignación (`+=`, `-=`, `*=`, etc.)
- [x] Operadores de incremento/decremento (`++`, `--`)

### ✅ Extensiones STL
- [x] `std::numeric_limits<uint128_t>`
- [x] `std::hash<uint128_t>` para contenedores unordered
- [x] Concepts personalizados (`uint128_concepts`)
- [x] Compatibilidad con `std::iota`

### ✅ Funcionalidades Avanzadas
- [x] Constructor desde cadenas: `uint128_t("12345")`
- [x] Método `from_string()` estático
- [x] Literales de usuario: `42_u128`, `"123456789"_U128`
- [x] Operadores simétricos (friend functions)
- [x] Operador de inserción para streams (`operator<<`)

### ✅ Casos de Uso Prácticos
- [x] Cálculos financieros de alta precisión
- [x] Operaciones criptográficas
- [x] Contadores de gran rango
- [x] IDs únicos de 128 bits
- [x] Compatibilidad con algoritmos STL

## 📊 Benchmarks

Los resultados de performance se almacenan en `documentation/benchmarks/` como archivos CSV:

- `arithmetic_operations.csv` - Operaciones aritméticas
- `bitwise_operations.csv` - Operaciones bit a bit
- `string_conversion.csv` - Conversiones string ↔ uint128_t
- `comparison_operations.csv` - Operaciones de comparación

## 🚀 Ejemplos de Uso

```cpp
#include "uint128_t.hpp"

// Construcción básica
uint128_t a = 42;
uint128_t b(100, 200);  // high=100, low=200

// Literales de usuario
auto x = 1234_u128;
auto big = "18446744073709551616"_U128;

// Aritmética
auto result = a + b * x;

// Strings
uint128_t num("999999999999999999");
std::cout << num << std::endl;

// STL algorithms
std::vector<uint128_t> vec(10);
std::iota(vec.begin(), vec.end(), 1000_u128);
```

## ⚡ Performance

- **Tamaño**: Exactamente 16 bytes (2 × uint64_t)
- **Alineación**: Optimizada para arquitecturas de 64 bits
- **Operaciones**: Implementadas con intrínsecos del compilador cuando está disponible
- **Constexpr**: Soporte para evaluación en tiempo de compilación

## 🔗 Compatibilidad

- **Compiladores**: MSVC 14.50+, GCC 10+, Clang 12+
- **Estándares**: C++20 o superior
- **Plataformas**: Windows, Linux, macOS
- **Arquitecturas**: x64, ARM64

## 📝 Licencia

[Especificar licencia aquí]