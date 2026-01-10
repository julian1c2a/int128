/**
 * @file demo_int128_thread_safety.cpp
 * @brief Demostración de wrappers thread-safe para int128_t/uint128_t
 *
 * Compilar:
 *   g++ -std=c++20 -O2 -pthread -Iinclude \
 *       demos/examples/demo_int128_thread_safety.cpp -o demo_int128_thread_safety
 *
 * Ejecutar:
 *   ./demo_int128_thread_safety
 */

#include <int128_simple.hpp>
#include <int128_base_thread_safety.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

using namespace nstd;
using namespace nstd::int128_threadsafe;

// ========================= Demo 1: ThreadSafe<signed> (Mutex-based) =========================

void demo_mutex_wrapper()
{
    std::cout << "\n=== Demo 1: ThreadSafeInt128 (Mutex-based) ===\n";

    // Crear contador con valor inicial negativo
    ThreadSafeInt128 counter{int128_t{-1000}};

    std::cout << "Valor inicial: " << counter.get().to_string() << "\n";

    // Verificar signo usando is_negative() del tipo base
    const bool is_neg = counter.get().is_negative();
    std::cout << "Es negativo?: " << (is_neg ? "SI" : "NO") << "\n";

    // Crear 4 hilos que incrementan el contador
    constexpr int NUM_THREADS = 4;
    constexpr int INCREMENTS_PER_THREAD = 500;

    std::vector<std::thread> threads;
    threads.reserve(NUM_THREADS);

    const auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads.emplace_back([&counter]()
                             {
            for (int j = 0; j < INCREMENTS_PER_THREAD; ++j) {
                counter.add(int128_t{1});
            } });
    }

    // Esperar a todos los hilos
    for (auto &t : threads)
    {
        t.join();
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    const int128_t final_value = counter.get();
    const int128_t expected{-1000 + NUM_THREADS * INCREMENTS_PER_THREAD};

    std::cout << "Valor final: " << final_value.to_string() << "\n";
    std::cout << "Esperado: " << expected.to_string() << "\n";
    std::cout << "Tiempo: " << duration.count() << " us\n";
    std::cout << "Es negativo ahora?: " << (final_value.is_negative() ? "SI" : "NO") << "\n";
    std::cout << "Correcto?: " << (final_value == expected ? "SI" : "NO") << "\n";
}

// ========================= Demo 2: ThreadSafeRW (Read-Write Lock) =========================

void demo_rwlock_wrapper()
{
    std::cout << "\n=== Demo 2: ThreadSafeInt128RW (Read-Write Lock) ===\n";

    ThreadSafeInt128RW statistics{int128_t{-500}};

    std::cout << "Valor inicial: " << statistics.get().to_string() << "\n";

    // 8 lectores concurrentes + 2 escritores
    constexpr int NUM_READERS = 8;
    constexpr int NUM_WRITERS = 2;
    constexpr int READS_PER_THREAD = 1000;
    constexpr int WRITES_PER_THREAD = 100;

    std::vector<std::thread> threads;

    const auto start = std::chrono::high_resolution_clock::now();

    // Lectores (pueden ejecutarse concurrentemente)
    for (int i = 0; i < NUM_READERS; ++i)
    {
        threads.emplace_back([&statistics]()
                             {
            for (int j = 0; j < READS_PER_THREAD; ++j) {
                [[maybe_unused]] const int128_t val = statistics.get();
                // Verificar si es negativo del valor leído
                [[maybe_unused]] const bool neg = val.is_negative();
            } });
    }

    // Escritores (acceso exclusivo)
    for (int i = 0; i < NUM_WRITERS; ++i)
    {
        threads.emplace_back([&statistics]()
                             {
            for (int j = 0; j < WRITES_PER_THREAD; ++j) {
                statistics.add(int128_t{5});
            } });
    }

    for (auto &t : threads)
    {
        t.join();
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    const int128_t final_value = statistics.get();
    const int128_t expected{-500 + NUM_WRITERS * WRITES_PER_THREAD * 5};

    std::cout << "Valor final: " << final_value.to_string() << "\n";
    std::cout << "Esperado: " << expected.to_string() << "\n";
    std::cout << "Tiempo: " << duration.count() << " us\n";
    std::cout << "Correcto?: " << (final_value == expected ? "SI" : "NO") << "\n";
}

// ========================= Demo 3: ThreadSafe<unsigned> =========================

void demo_unsigned_wrapper()
{
    std::cout << "\n=== Demo 3: ThreadSafeUint128 (Unsigned) ===\n";

    ThreadSafeUint128 counter{uint128_t{0}};

    std::cout << "Valor inicial: " << counter.get().to_string() << "\n";

    // Crear 4 hilos que incrementan el contador
    constexpr int NUM_THREADS = 4;
    constexpr int INCREMENTS_PER_THREAD = 1000;

    std::vector<std::thread> threads;
    threads.reserve(NUM_THREADS);

    const auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads.emplace_back([&counter]()
                             {
            for (int j = 0; j < INCREMENTS_PER_THREAD; ++j) {
                ++counter;  // Usar operador de incremento thread-safe
            } });
    }

    for (auto &t : threads)
    {
        t.join();
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    const uint128_t final_value = counter.get();
    const uint128_t expected{static_cast<uint64_t>(NUM_THREADS * INCREMENTS_PER_THREAD)};

    std::cout << "Valor final: " << final_value.to_string() << "\n";
    std::cout << "Esperado: " << expected.to_string() << "\n";
    std::cout << "Tiempo: " << duration.count() << " us\n";
    std::cout << "Correcto?: " << (final_value == expected ? "SI" : "NO") << "\n";
}

// ========================= Demo 4: Fetch-and-Add =========================

void demo_fetch_operations()
{
    std::cout << "\n=== Demo 4: Fetch-and-Add Operations ===\n";

    ThreadSafeUint128 counter{uint128_t{100}};

    std::cout << "Valor inicial: " << counter.get().to_string() << "\n";

    // fetch_add retorna el valor ANTERIOR
    const uint128_t old1 = counter.fetch_add(uint128_t{50});
    std::cout << "fetch_add(50): valor anterior = " << old1.to_string()
              << ", nuevo = " << counter.get().to_string() << "\n";

    // fetch_sub retorna el valor ANTERIOR
    const uint128_t old2 = counter.fetch_sub(uint128_t{30});
    std::cout << "fetch_sub(30): valor anterior = " << old2.to_string()
              << ", nuevo = " << counter.get().to_string() << "\n";

    // Verificar resultado final
    const uint128_t expected{120}; // 100 + 50 - 30 = 120
    std::cout << "Esperado: " << expected.to_string() << "\n";
    std::cout << "Correcto?: " << (counter.get() == expected ? "SI" : "NO") << "\n";
}

// ========================= Demo 5: Compare-and-Swap =========================

void demo_compare_exchange()
{
    std::cout << "\n=== Demo 5: Compare-and-Swap ===\n";

    ThreadSafeUint128 counter{uint128_t{42}};

    std::cout << "Valor inicial: " << counter.get().to_string() << "\n";

    // CAS exitoso: expected == actual
    uint128_t expected{42};
    const uint128_t desired{100};
    const bool success1 = counter.compare_exchange(expected, desired);
    std::cout << "CAS(42 -> 100): " << (success1 ? "EXITO" : "FALLO")
              << ", valor = " << counter.get().to_string() << "\n";

    // CAS fallido: expected != actual
    uint128_t wrong_expected{42}; // ya no es 42, es 100
    const uint128_t desired2{200};
    const bool success2 = counter.compare_exchange(wrong_expected, desired2);
    std::cout << "CAS(42 -> 200): " << (success2 ? "EXITO" : "FALLO")
              << ", expected actualizado = " << wrong_expected.to_string() << "\n";

    // CAS exitoso con valor correcto
    uint128_t correct_expected{100};
    const uint128_t desired3{200};
    const bool success3 = counter.compare_exchange(correct_expected, desired3);
    std::cout << "CAS(100 -> 200): " << (success3 ? "EXITO" : "FALLO")
              << ", valor final = " << counter.get().to_string() << "\n";
}

// ========================= Demo 6: Operaciones Aritmeticas =========================

void demo_arithmetic_operations()
{
    std::cout << "\n=== Demo 6: Operaciones Aritmeticas Thread-Safe ===\n";

    ThreadSafeInt128 counter{int128_t{1000}};

    std::cout << "Valor inicial: " << counter.get().to_string() << "\n";

    // Operaciones basicas
    counter.add(int128_t{500});
    std::cout << "Despues de add(500): " << counter.get().to_string() << "\n";

    counter.subtract(int128_t{200});
    std::cout << "Despues de subtract(200): " << counter.get().to_string() << "\n";

    counter.multiply(int128_t{2});
    std::cout << "Despues de multiply(2): " << counter.get().to_string() << "\n";

    counter.divide(int128_t{13});
    std::cout << "Despues de divide(13): " << counter.get().to_string() << "\n";

    // Verificar resultado: (1000 + 500 - 200) * 2 / 13 = 2600 / 13 = 200
    const int128_t expected{200};
    std::cout << "Esperado: " << expected.to_string() << "\n";
    std::cout << "Correcto?: " << (counter.get() == expected ? "SI" : "NO") << "\n";
}

// ========================= Demo 7: Stress Test =========================

void demo_stress_test()
{
    std::cout << "\n=== Demo 7: Stress Test (Alta Concurrencia) ===\n";

    ThreadSafeUint128 counter{uint128_t{0}};

    constexpr int NUM_THREADS = 16;
    constexpr int OPS_PER_THREAD = 10000;

    std::vector<std::thread> threads;
    threads.reserve(NUM_THREADS);

    const auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads.emplace_back([&counter, i]()
                             {
            for (int j = 0; j < OPS_PER_THREAD; ++j) {
                if ((i + j) % 2 == 0) {
                    counter.add(uint128_t{1});
                } else {
                    counter.add(uint128_t{1});
                }
            } });
    }

    for (auto &t : threads)
    {
        t.join();
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    const uint128_t final_value = counter.get();
    const uint128_t expected{static_cast<uint64_t>(NUM_THREADS * OPS_PER_THREAD)};

    std::cout << "Operaciones totales: " << (NUM_THREADS * OPS_PER_THREAD) << "\n";
    std::cout << "Valor final: " << final_value.to_string() << "\n";
    std::cout << "Esperado: " << expected.to_string() << "\n";
    std::cout << "Tiempo total: " << duration.count() << " ms\n";
    std::cout << "Ops/segundo: " << (NUM_THREADS * OPS_PER_THREAD * 1000 / duration.count()) << "\n";
    std::cout << "Correcto?: " << (final_value == expected ? "SI" : "NO") << "\n";
}

// ========================= Main =========================

int main()
{
    std::cout << "+=========================================+\n";
    std::cout << "|  int128 Thread-Safety Demo             |\n";
    std::cout << "+=========================================+\n";

    demo_mutex_wrapper();
    demo_rwlock_wrapper();
    demo_unsigned_wrapper();
    demo_fetch_operations();
    demo_compare_exchange();
    demo_arithmetic_operations();
    demo_stress_test();

    std::cout << "\n+=========================================+\n";
    std::cout << "|  Todos los demos completados OK        |\n";
    std::cout << "+=========================================+\n";

    return 0;
}
