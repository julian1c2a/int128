/**
 * Tutorial 11: Thread Safety (Seguridad en Multithreading)
 *
 * Aprenderás:
 * - Operaciones thread-safe
 * - std::atomic con uint128_t
 * - Protección con mutex
 * - Operaciones concurrentes
 */

#include <atomic>
#include <int128.hpp>
#include <uint128/uint128_iostreams.hpp>
#include <int128/int128_iostreams.hpp>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace nstd;

using namespace uint128_literals;

int main()
{
    std::cout << "========================================\n";
    std::cout << "Tutorial 11: Thread Safety\n";
    std::cout << "========================================\n\n";

    // ============================================================
    // 1. PROBLEMA: CARRERAS DE DATOS (RACE CONDITION)
    // ============================================================
    std::cout << "--- 1. ⚠️  Sin protección (INCORRECTO) ---\n";

    uint128_t counter = 0;

    auto increment_unsafe = [&]() {
        for (int i = 0; i < 1000; ++i) {
            ++counter; // ¡No es thread-safe!
        }
    };

    // No ejecutar esto realmente, solo demostración
    std::cout << "⚠️  ++counter en múltiples threads sin protección\n";
    std::cout << "   puede causar race conditions y resultados incorrectos.\n\n";

    // ============================================================
    // 2. SOLUCIÓN 1: MUTEX
    // ============================================================
    std::cout << "--- 2. ✅ Protección con std::mutex ---\n";

    uint128_t safe_counter = 0;
    std::mutex mtx;

    auto increment_safe = [&]() {
        for (int i = 0; i < 1000; ++i) {
            std::lock_guard<std::mutex> lock(mtx);
            ++safe_counter;
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(increment_safe);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Resultado con mutex: " << safe_counter << "\n";
    std::cout << "Esperado: 4000\n\n";

    // ============================================================
    // 3. SOLUCIÓN 2: ATOMIC (si está disponible)
    // ============================================================
    std::cout << "--- 3. std::atomic<uint128_t> ---\n";

    // Nota: std::atomic<uint128_t> puede no estar disponible en todas las plataformas
    // Depende del soporte de instrucciones como CMPXCHG16B en x86_64

#ifdef __SIZEOF_INT128__
    std::cout << "✅ Platform soporta operaciones atómicas de 128 bits\n";
#else
    std::cout << "⚠️  Platform puede requerir mutex para uint128_t\n";
#endif
    std::cout << "\n";

    // ============================================================
    // 4. OPERACIONES DE LECTURA THREAD-SAFE
    // ============================================================
    std::cout << "--- 4. Lectura thread-safe ---\n";

    uint128_t shared_value = 12345_u128;
    std::mutex read_mtx;

    auto read_value = [&]() -> uint128_t {
        std::lock_guard<std::mutex> lock(read_mtx);
        return shared_value;
    };

    std::cout << "Lectura protegida: " << read_value() << "\n\n";

    // ============================================================
    // 5. BEST PRACTICES
    // ============================================================
    std::cout << "--- 5. 📘 Best Practices ---\n";
    std::cout << "1. Usa std::atomic si tu plataforma lo soporta\n";
    std::cout << "2. Protege con std::mutex si no hay soporte atómico\n";
    std::cout << "3. Minimiza el tiempo dentro del lock\n";
    std::cout << "4. Considera usar lock-free algorithms si es crítico\n";
    std::cout << "5. Siempre testea código multithreaded\n\n";

    std::cout << "========================================\n";
    std::cout << "✅ Tutorial 11 completado\n";
    std::cout << "========================================\n";
    std::cout << "\nPróximo tutorial: 12_safe_operations.cpp\n";

    return 0;
}

