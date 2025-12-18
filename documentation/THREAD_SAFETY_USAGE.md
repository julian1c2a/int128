# Thread-Safe uint128_t - Guía de Uso

## Descripción

El header `uint128_thread_safety.hpp` proporciona 4 wrappers thread-safe para la clase `uint128_t`:

1. **ThreadSafeUint128** - Mutex-based (recomendado para uso general)
2. **ThreadSafeUint128RW** - Read-Write lock (optimizado para lecturas)
3. **ThreadSafeUint128Atomic** - Interfaz std::atomic estándar
4. **ThreadSafeUint128SpinLock** - Spin-lock (baja contención)

## Instalación

```cpp
#include "uint128/uint128_thread_safety.hpp"
using namespace uint128_threadsafe;
```

## Uso Rápido

### Opción 1: Mutex (Recomendado)

```cpp
// Contador global compartido
ThreadSafeUint128 counter(uint128_t(0));

// Desde múltiples hilos:
void worker() {
    counter.add(uint128_t(1));      // Thread-safe
    uint128_t val = counter.get();  // Thread-safe
}
```

### Opción 2: Read-Write Lock (Para Lecturas Frecuentes)

```cpp
// Estadísticas con muchas lecturas
ThreadSafeUint128RW statistics(uint128_t(0));

// Múltiples lectores concurrentes
void reader() {
    uint128_t val = statistics.get();  // Shared lock
}

// Escritor exclusivo
void writer() {
    statistics.add(uint128_t(1));  // Unique lock
}
```

### Opción 3: Atomic (Interfaz Estándar)

```cpp
ThreadSafeUint128Atomic atomic_val(uint128_t(0));

// Operaciones atómicas estándar
atomic_val.store(uint128_t(100));
uint128_t val = atomic_val.load();

// Compare-and-swap
uint128_t expected(100), desired(200);
bool success = atomic_val.compare_exchange_strong(expected, desired);
```

### Opción 4: SpinLock (Baja Contención)

```cpp
ThreadSafeUint128SpinLock fast_counter(uint128_t(0));

void worker() {
    fast_counter.add(uint128_t(1));  // Spin-lock
}
```

## Comparación de Performance

Resultados de 100,000 operaciones:

| Método | Tiempo (µs) | Lock-Free | Caso de Uso |
|--------|-------------|-----------|-------------|
| Thread-local (sin wrapper) | 0 | N/A | Cada hilo su copia |
| SpinLock | 3,000 | Sí | Baja contención |
| RW-Lock | 5,000 | No | 90%+ lecturas |
| Mutex | 7,000 | No | Uso general |
| Atomic | 7,000 | No* | *Usa locks internamente |

## API Completa

### ThreadSafeUint128 (Mutex)

```cpp
ThreadSafeUint128 counter(uint128_t(0));

// Operaciones básicas
counter.get();                    // Obtener valor
counter.set(uint128_t(100));      // Establecer valor
counter.add(uint128_t(10));       // Sumar
counter.subtract(uint128_t(5));   // Restar
counter.multiply(uint128_t(2));   // Multiplicar
counter.divide(uint128_t(2));     // Dividir

// Fetch-and-modify (retorna valor anterior)
uint128_t old = counter.fetch_add(uint128_t(1));

// Incremento/decremento
uint128_t new_val = ++counter;    // Pre-incremento
uint128_t old_val = counter++;    // Post-incremento

// Compare-and-swap
uint128_t expected(100), desired(200);
bool success = counter.compare_exchange(expected, desired);

// Operación personalizada atómica
counter.apply([](uint128_t v) {
    return v * uint128_t(2) + uint128_t(5);
});
```

### ThreadSafeUint128RW (Read-Write Lock)

```cpp
ThreadSafeUint128RW stats(uint128_t(0));

// Lecturas (shared lock - concurrentes)
uint128_t val = stats.get();

// Escrituras (unique lock - exclusivas)
stats.set(uint128_t(100));
stats.add(uint128_t(10));

// Operaciones personalizadas
stats.read([](const uint128_t& v) {
    return v.low();  // Lectura con shared lock
});

stats.modify([](uint128_t& v) {
    v += uint128_t(10);  // Escritura con unique lock
});
```

### ThreadSafeUint128Atomic (std::atomic)

```cpp
ThreadSafeUint128Atomic atomic_val(uint128_t(0));

// Verificar si es lock-free
bool lock_free = atomic_val.is_lock_free();  // Usualmente false

// Operaciones atómicas
atomic_val.store(uint128_t(100));
uint128_t val = atomic_val.load();
uint128_t old = atomic_val.exchange(uint128_t(200));

// Compare-and-swap
uint128_t expected(100), desired(200);
atomic_val.compare_exchange_weak(expected, desired);
atomic_val.compare_exchange_strong(expected, desired);

// Memory ordering
atomic_val.store(uint128_t(100), std::memory_order_relaxed);
uint128_t val = atomic_val.load(std::memory_order_acquire);
```

### ThreadSafeUint128SpinLock (Spin-lock)

```cpp
ThreadSafeUint128SpinLock counter(uint128_t(0));

// API similar a ThreadSafeUint128
counter.get();
counter.set(uint128_t(100));
counter.add(uint128_t(10));
counter.fetch_add(uint128_t(1));
```

## Ejemplos Completos

### Ejemplo 1: Contador Global

```cpp
#include "uint128/uint128_thread_safety.hpp"
#include <thread>
#include <vector>

using namespace uint128_threadsafe;

ThreadSafeUint128 global_counter(uint128_t(0));

void worker() {
    for (int i = 0; i < 1000; ++i) {
        global_counter.add(uint128_t(1));
    }
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(worker);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "Final: " << global_counter.get().low() << std::endl;
    // Output: Final: 10000
    return 0;
}
```

### Ejemplo 2: Estadísticas con RW-Lock

```cpp
ThreadSafeUint128RW stats(uint128_t(0));

// 8 hilos leyendo concurrentemente
void reader() {
    for (int i = 0; i < 1000; ++i) {
        uint128_t val = stats.get();  // Múltiples lectores OK
        // Procesar valor...
    }
}

// 2 hilos escribiendo exclusivamente
void writer() {
    for (int i = 0; i < 100; ++i) {
        stats.add(uint128_t(1));  // Acceso exclusivo
    }
}
```

### Ejemplo 3: Compare-and-Swap

```cpp
ThreadSafeUint128 shared_value(uint128_t(0, 100));

void try_update() {
    uint128_t expected(0, 100);
    uint128_t desired(0, 200);
    
    if (shared_value.compare_exchange(expected, desired)) {
        std::cout << "Actualización exitosa" << std::endl;
    } else {
        std::cout << "Valor cambió a: " << expected.low() << std::endl;
    }
}
```

## Recomendaciones

### ¿Cuándo usar cada opción?

1. **ThreadSafeUint128 (Mutex)**
   - ✅ Uso general
   - ✅ Código simple y correcto
   - ✅ Performance suficiente para la mayoría de casos
   - ❌ No optimizado para lecturas

2. **ThreadSafeUint128RW (RW-Lock)**
   - ✅ 90%+ operaciones de lectura
   - ✅ Múltiples lectores concurrentes
   - ✅ Mejor performance para read-heavy
   - ❌ Más complejo que mutex simple

3. **ThreadSafeUint128Atomic (std::atomic)**
   - ✅ Interfaz estándar familiar
   - ✅ Compatible con código existente
   - ❌ No es lock-free en la mayoría de plataformas
   - ❌ Performance similar a mutex

4. **ThreadSafeUint128SpinLock (Spin-lock)**
   - ✅ Baja contención
   - ✅ Secciones críticas muy cortas
   - ✅ Menor overhead
   - ❌ Desperdicia CPU en alta contención

### Patrón Recomendado: Thread-Local

Para máxima performance, usa thread-local:

```cpp
// Sin sincronización - máxima performance
thread_local uint128_t local_counter(0);

void worker() {
    local_counter += uint128_t(1);  // Sin locks!
}

uint128_t get_total() {
    // Agregar contadores thread-local al final
    // (implementación específica del contexto)
}
```

## Importante: ¿Cuándo NO necesitas estos wrappers?

`uint128_t` **YA ES thread-safe** para:

```cpp
// ✅ Lecturas concurrentes (const)
const uint128_t shared(100);
void thread1() { auto val = shared.high(); }  // SEGURO
void thread2() { auto val = shared.low(); }   // SEGURO

// ✅ Objetos thread-local
void thread_func() {
    uint128_t local(0);
    local += 100;  // SEGURO (cada hilo su copia)
}

// ✅ Paso por valor o const&
void process(const uint128_t& value) {  // SEGURO
    auto result = value * 2;
}
```

**Solo necesitas wrappers** para:
- ❌ Modificaciones concurrentes del mismo objeto compartido
- ❌ Contadores globales/static modificados por múltiples hilos

## Compilación

```bash
# Compilar con soporte thread
g++ -std=c++20 -pthread -I include your_file.cpp -o output -latomic

# Clang
clang++ -std=c++20 -pthread -I include your_file.cpp -o output -latomic
```

## Tests

```bash
# Compilar y ejecutar tests
g++ -std=c++20 -pthread -I include tests/test_thread_safety_header.cpp -o test -latomic
./test

# Ejecutar ejemplos completos
g++ -std=c++20 -pthread -I include demos/example_thread_safety.cpp -o examples -latomic
./examples
```

## Notas de Implementación

- **std::atomic<uint128_t>**: Disponible pero NO es lock-free en la mayoría de plataformas (usa locks internamente)
- **SpinLock**: Usa `std::atomic_flag` para implementación lock-free real
- **RW-Lock**: Permite múltiples lectores simultáneos, escritores exclusivos
- **Mutex**: Simple y correcto, suficiente para la mayoría de casos

## Documentación Relacionada

- `THREAD_SAFETY_SUMMARY.md` - Resumen ejecutivo
- `documentation/THREAD_SAFETY_ANALYSIS.md` - Análisis técnico completo
- `tests/uint128_thread_safety_tests.cpp` - Tests exhaustivos
- `demos/example_thread_safety.cpp` - Ejemplos completos

## Licencia

Mismo que uint128_t.hpp
