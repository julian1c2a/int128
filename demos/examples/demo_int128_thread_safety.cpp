/**
 * @file demo_int128_thread_safety.cpp
 * @brief Demostración de wrappers thread-safe para int128_t
 *
 * Compilar:
 *   g++ -std=c++20 -O2 -pthread -I../include \
 *       demos/demo_int128_thread_safety.cpp -o demo_int128_thread_safety
 *
 * Ejecutar:
 *   ./demo_int128_thread_safety
 */

#include <chrono>
#include <int128_simple.hpp>
#include <int128/int128_thread_safety.hpp>
#include <iostream>
#include <thread>
#include <vector>

using namespace nstd;

using namespace int128_threadsafe;

// ========================= Demo 1: ThreadSafeInt128 (Mutex) =========================

void demo_mutex_wrapper()
{
    std::cout << "\n=== Demo 1: ThreadSafeInt128 (Mutex-based) ===\n";

    ThreadSafeInt128 counter(int128_t(-1000));

    std::cout << "Valor inicial: " << counter.get().to_string() << "\n";
    std::cout << "¿Es negativo?: " << (counter.is_negative() ? "SÍ" : "NO") << "\n";

    // Crear 4 hilos que incrementan el contador
    const int NUM_THREADS = 4;
    const int INCREMENTS_PER_THREAD = 500;

    std::vector<std::thread> threads;
    threads.reserve(NUM_THREADS);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < INCREMENTS_PER_THREAD; ++j) {
                counter.add(int128_t(1));
            }
        });
    }

    // Esperar a todos los hilos
    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    int128_t final_value = counter.get();
    std::cout << "Valor final: " << final_value.to_string() << "\n";
    std::cout << "Esperado: " << int128_t(-1000 + NUM_THREADS * INCREMENTS_PER_THREAD).to_string()
              << "\n";
    std::cout << "Tiempo: " << duration.count() << " µs\n";
    std::cout << "¿Es negativo ahora?: " << (counter.is_negative() ? "SÍ" : "NO") << "\n";
}

// ========================= Demo 2: ThreadSafeInt128RW (Read-Write Lock) =========================

void demo_rwlock_wrapper()
{
    std::cout << "\n=== Demo 2: ThreadSafeInt128RW (Read-Write Lock) ===\n";

    ThreadSafeInt128RW statistics(int128_t(-500));

    std::cout << "Valor inicial: " << statistics.get().to_string() << "\n";

    // 8 lectores concurrentes + 2 escritores
    const int NUM_READERS = 8;
    const int NUM_WRITERS = 2;
    const int READS_PER_THREAD = 1000;
    const int WRITES_PER_THREAD = 100;

    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    // Lectores (pueden ejecutarse concurrentemente)
    for (int i = 0; i < NUM_READERS; ++i) {
        threads.emplace_back([&statistics]() {
            for (int j = 0; j < READS_PER_THREAD; ++j) {
                [[maybe_unused]] int128_t val = statistics.get();
                [[maybe_unused]] bool neg = statistics.is_negative();
            }
        });
    }

    // Escritores (acceso exclusivo)
    for (int i = 0; i < NUM_WRITERS; ++i) {
        threads.emplace_back([&statistics]() {
            for (int j = 0; j < WRITES_PER_THREAD; ++j) {
                statistics.add(int128_t(1));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    int128_t final_value = statistics.get();
    std::cout << "Valor final: " << final_value.to_string() << "\n";
    std::cout << "Esperado: " << int128_t(-500 + NUM_WRITERS * WRITES_PER_THREAD).to_string()
              << "\n";
    std::cout << "Tiempo: " << duration.count() << " µs\n";
    std::cout << "Lecturas concurrentes: " << NUM_READERS * READS_PER_THREAD << "\n";
}

// ========================= Demo 3: ThreadSafeInt128Atomic (std::atomic) =========================

void demo_atomic_wrapper()
{
    std::cout << "\n=== Demo 3: ThreadSafeInt128Atomic (std::atomic) ===\n";

    ThreadSafeInt128Atomic atomic_counter(int128_t(-100));

    std::cout << "¿Lock-free?: " << (atomic_counter.is_lock_free() ? "SÍ" : "NO") << "\n";
    std::cout << "Valor inicial: " << atomic_counter.load().to_string() << "\n";

    const int NUM_THREADS = 4;
    const int OPS_PER_THREAD = 250;

    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&atomic_counter]() {
            for (int j = 0; j < OPS_PER_THREAD; ++j) {
                int128_t current = atomic_counter.load();
                int128_t next = current + int128_t(1);
                // Compare-and-swap loop
                while (!atomic_counter.compare_exchange_weak(current, next)) {
                    next = current + int128_t(1);
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    int128_t final_value = atomic_counter.load();
    std::cout << "Valor final: " << final_value.to_string() << "\n";
    std::cout << "Esperado: " << int128_t(-100 + NUM_THREADS * OPS_PER_THREAD).to_string() << "\n";
    std::cout << "Tiempo: " << duration.count() << " µs\n";
}

// ========================= Demo 4: ThreadSafeInt128SpinLock =========================

void demo_spinlock_wrapper()
{
    std::cout << "\n=== Demo 4: ThreadSafeInt128SpinLock (Spin-lock) ===\n";

    ThreadSafeInt128SpinLock fast_counter(int128_t(-200));

    std::cout << "Valor inicial: " << fast_counter.get().to_string() << "\n";

    const int NUM_THREADS = 4;
    const int OPS_PER_THREAD = 500;

    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&fast_counter]() {
            for (int j = 0; j < OPS_PER_THREAD; ++j) {
                fast_counter.add(int128_t(1));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    int128_t final_value = fast_counter.get();
    std::cout << "Valor final: " << final_value.to_string() << "\n";
    std::cout << "Esperado: " << int128_t(-200 + NUM_THREADS * OPS_PER_THREAD).to_string() << "\n";
    std::cout << "Tiempo: " << duration.count() << " µs (más rápido que mutex)\n";
}

// ========================= Demo 5: Operaciones Signed-Specific =========================

void demo_signed_operations()
{
    std::cout << "\n=== Demo 5: Operaciones Específicas de Signed ===\n";

    ThreadSafeInt128 counter(int128_t(-75));

    std::cout << "Valor inicial: " << counter.get().to_string() << "\n";
    std::cout << "¿Es negativo?: " << (counter.is_negative() ? "SÍ" : "NO") << "\n";
    std::cout << "Valor absoluto: " << counter.get_abs().to_string() << "\n";

    // Negar el valor
    counter.negate();
    std::cout << "\nDespués de negate():\n";
    std::cout << "Valor: " << counter.get().to_string() << "\n";
    std::cout << "¿Es negativo?: " << (counter.is_negative() ? "SÍ" : "NO") << "\n";

    // División signed
    counter.divide(int128_t(3));
    std::cout << "\nDespués de divide(3):\n";
    std::cout << "Valor: " << counter.get().to_string() << " (75/3 = 25)\n";

    // Módulo signed
    counter.set(int128_t(-50));
    counter.modulo(int128_t(7));
    std::cout << "\n-50 % 7:\n";
    std::cout << "Valor: " << counter.get().to_string() << " (esperado: -1)\n";

    // Concurrent negate
    counter.set(int128_t(100));
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&counter]() { counter.negate(); });
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout << "\nDespués de 10 negaciones concurrentes:\n";
    std::cout << "Valor final: " << counter.get().to_string() << " (100 o -100)\n";
}

// ========================= Demo 6: Comparación de Performance =========================

void demo_performance_comparison()
{
    std::cout << "\n=== Demo 6: Comparación de Performance ===\n";

    const int NUM_THREADS = 4;
    const int OPS_PER_THREAD = 10000;

    // Test 1: Mutex
    {
        ThreadSafeInt128 counter(int128_t(0));
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        for (int i = 0; i < NUM_THREADS; ++i) {
            threads.emplace_back([&counter]() {
                for (int j = 0; j < OPS_PER_THREAD; ++j) {
                    counter.add(int128_t(1));
                }
            });
        }
        for (auto& t : threads) {
            t.join();
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Mutex wrapper:    " << duration.count() << " µs\n";
    }

    // Test 2: SpinLock
    {
        ThreadSafeInt128SpinLock counter(int128_t(0));
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        for (int i = 0; i < NUM_THREADS; ++i) {
            threads.emplace_back([&counter]() {
                for (int j = 0; j < OPS_PER_THREAD; ++j) {
                    counter.add(int128_t(1));
                }
            });
        }
        for (auto& t : threads) {
            t.join();
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "SpinLock wrapper: " << duration.count() << " µs (más rápido)\n";
    }

    // Test 3: Thread-local (sin sincronización)
    {
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        for (int i = 0; i < NUM_THREADS; ++i) {
            threads.emplace_back([]() {
                int128_t local_counter(0);
                for (int j = 0; j < OPS_PER_THREAD; ++j) {
                    local_counter += int128_t(1);
                }
            });
        }
        for (auto& t : threads) {
            t.join();
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Thread-local:     " << duration.count()
                  << " µs (más rápido - sin sincronización)\n";
    }
}

// ========================= Main =========================

int main()
{
    std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║  Demostración: Thread-Safe Wrappers para int128_t       ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n";

    try {
        demo_mutex_wrapper();
        demo_rwlock_wrapper();
        demo_atomic_wrapper();
        demo_spinlock_wrapper();
        demo_signed_operations();
        demo_performance_comparison();

        std::cout << "\n✅ Todas las demos completadas exitosamente!\n";
        std::cout << "\nResumen:\n";
        std::cout << "- ThreadSafeInt128:         Uso general (recomendado)\n";
        std::cout << "- ThreadSafeInt128RW:       Read-heavy workloads\n";
        std::cout << "- ThreadSafeInt128Atomic:   Interfaz std::atomic\n";
        std::cout << "- ThreadSafeInt128SpinLock: Baja contención (más rápido)\n";
        std::cout << "- Thread-local:             Sin overhead (preferido si es posible)\n";

    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

