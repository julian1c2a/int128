# Examples - Real-World Use Cases

Esta carpeta contiene ejemplos de **casos de uso reales** de la biblioteca int128.

## 📁 Ejemplos Disponibles

### 1. Thread Safety Examples

| Archivo | Descripción |
|---------|-------------|
| [demo_int128_thread_safety.cpp](demo_int128_thread_safety.cpp) | Demostración completa de thread safety |
| [example_thread_safety.cpp](example_thread_safety.cpp) | Ejemplos prácticos de multithreading |

**Qué aprenderás:**
- Uso de std::atomic<uint128_t>
- Protección con std::mutex
- Race conditions y cómo evitarlas
- Operaciones concurrentes seguras

### 2. Mathematical Library

| Archivo | Descripción |
|---------|-------------|
| [demo_mathematical_library.cpp](demo_mathematical_library.cpp) | Suite completa de funciones matemáticas |

**Qué aprenderás:**
- GCD, LCM de números grandes
- Raíz cuadrada entera
- Exponenciación modular
- Funciones trigonométricas (aproximadas)

### 3. General Demonstrations

| Archivo | Descripción |
|---------|-------------|
| [int128.cpp](int128.cpp) | Demo general de capacidades |

## 📚 Ejemplos por Implementar

Los siguientes son ejemplos planeados que puedes implementar:

### IPv6 Address Handling
```cpp
// demos/examples/ipv6_address.cpp
class IPv6Address {
    uint128_t address;
public:
    IPv6Address(const std::string& str);  // Parse "::1"
    std::string to_string() const;
    bool in_subnet(const IPv6Address& network, int prefix_len) const;
};
```

### UUID Generation
```cpp
// demos/examples/uuid_generation.cpp
uint128_t generate_uuid();
std::string uuid_to_string(uint128_t uuid);
uint128_t uuid_from_string(const std::string& str);
```

### Big Integer Calculator
```cpp
// demos/examples/big_integer_calculator.cpp
int main() {
    std::string expression;
    while (std::getline(std::cin, expression)) {
        auto result = evaluate(expression);
        std::cout << result << "\n";
    }
}
```

### Prime Factorization
```cpp
// demos/examples/prime_factorization.cpp
std::vector<uint128_t> factorize(uint128_t n);
bool is_prime(uint128_t n);
uint128_t next_prime(uint128_t n);
```

### Mersenne Primes
```cpp
// demos/examples/mersenne_primes.cpp
bool is_mersenne_prime(int exponent);
std::vector<uint128_t> find_mersenne_primes(int max_exponent);
```

## 🚀 Cómo Usar

### Compilar un ejemplo

#### Con g++:
```bash
g++ -std=c++20 -I../../include -pthread demo_int128_thread_safety.cpp -o thread_demo
./thread_demo
```

#### Con CMake:
```bash
cmake --build . --target demo_int128_thread_safety
./demos/examples/demo_int128_thread_safety
```

### Crear tu propio ejemplo

1. Crea un archivo `.cpp` en esta carpeta
2. Incluye `int128.hpp`
3. Implementa tu caso de uso
4. Compila y prueba
5. (Opcional) Envía un PR para compartir!

## 💡 Ideas de Ejemplos

¿Qué casos de uso te gustaría ver? Algunas ideas:

- **Finanzas**: Cálculos de precisión arbitraria
- **Criptografía**: Aritmética modular grande
- **Ciencia**: Constantes y cálculos astronómicos
- **Juegos**: Puntajes extremadamente grandes
- **Simulación**: Contadores de eventos masivos
- **Blockchain**: Hashes y direcciones de 128 bits
- **Networking**: Direcciones IPv6, UUIDs
- **Base de datos**: Índices únicos gigantes
- **Machine Learning**: IDs de dataset masivos

## 📊 Plantilla de Ejemplo

```cpp
/**
 * Example: [NOMBRE DEL EJEMPLO]
 * 
 * Este ejemplo demuestra:
 * - [Característica 1]
 * - [Característica 2]
 * - [Característica 3]
 * 
 * Caso de uso real:
 * [Descripción del problema que resuelve]
 */

#include <int128.hpp>
#include <iostream>

using namespace uint128_literals;

int main() {
    // Tu código aquí
    
    std::cout << "Resultado: " << resultado << "\n";
    
    return 0;
}
```

## 🔗 Enlaces Útiles

- [Tutoriales](../tutorials/) - Aprende los básicos primero
- [Showcase](../showcase/) - Demos impresionantes
- [Documentación](../../documentation/generated/html/) - API reference

## 🐛 Reportar Issues

Si encuentras problemas en los ejemplos:
1. Verifica que tu compilador soporta C++20
2. Asegúrate de compilar con `-pthread` si usas threads
3. Revisa la documentación de la función que usas

## 🤝 Contribuir

¿Tienes un caso de uso interesante? ¡Compártelo!

1. Fork el repositorio
2. Crea tu ejemplo en `demos/examples/`
3. Documenta bien el código
4. Incluye output esperado
5. Envía un Pull Request

---

**¡Explora, aprende y crea! 💻**
