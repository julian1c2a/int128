/**
 * @file example_thread_safety.cpp
 * @brief Comprehensive examples of using uint128_thread_safety.hpp
 */

#include <chrono>
#include <int128_simple.hpp>
#include <iostream>
#include <thread>
#include <vector>

using namespace nstd;

using namespace nstd;

// ========================= Example 1: Global Counter =========================

void example_global_counter()
{
    std::cout << "Example 1: Global Counter with Mutex" << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    ThreadSafeUint128 global_counter(uint128_t(0));

    // Simulate 10 threads incrementing a counter
    std::vector<std::thread> threads;
    constexpr int NUM_THREADS = 10;
    constexpr int INCREMENTS = 1000;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&global_counter]() {
            for (int j = 0; j < INCREMENTS; ++j) {
                global_counter.add(uint128_t(0, 1));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    uint128_t final_value = global_counter.get();
    std::cout << "Final counter value: " << final_value.low() << std::endl;
    std::cout << "Expected: " << NUM_THREADS * INCREMENTS << std::endl;
    std::cout << "Time: " << duration.count() << " µs" << std::endl;
    std::cout << "✓ Success!" << std::endl;
    std::cout << std::endl;
}

// ========================= Example 2: Read-Heavy Statistics =========================

void example_statistics()
{
    std::cout << "Example 2: Read-Heavy Statistics with RW-Lock" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;

    ThreadSafeUint128RW statistics(uint128_t(0, 1000));

    std::vector<std::thread> threads;
    constexpr int NUM_READERS = 8;
    constexpr int NUM_WRITERS = 2;
    constexpr int OPERATIONS = 500;

    auto start = std::chrono::high_resolution_clock::now();

    // Many readers
    for (int i = 0; i < NUM_READERS; ++i) {
        threads.emplace_back([&statistics]() {
            for (int j = 0; j < OPERATIONS; ++j) {
                uint128_t val = statistics.get();
                // Simulate some read operation
                (void)val;
            }
        });
    }

    // Few writers
    for (int i = 0; i < NUM_WRITERS; ++i) {
        threads.emplace_back([&statistics]() {
            for (int j = 0; j < OPERATIONS; ++j) {
                statistics.add(uint128_t(0, 1));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    uint128_t final_value = statistics.get();
    std::cout << "Final statistics value: " << final_value.low() << std::endl;
    std::cout << "Time: " << duration.count() << " µs" << std::endl;
    std::cout << "✓ Success! (RW-lock allows concurrent reads)" << std::endl;
    std::cout << std::endl;
}

// ========================= Example 3: Atomic Operations =========================

void example_atomic()
{
    std::cout << "Example 3: Atomic Operations" << std::endl;
    std::cout << "-----------------------------" << std::endl;

    ThreadSafeUint128Atomic atomic_value(uint128_t(0, 100));

    std::cout << "Lock-free: " << (atomic_value.is_lock_free() ? "Yes" : "No") << std::endl;

    // Store and load
    atomic_value.store(uint128_t(10, 20));
    uint128_t loaded = atomic_value.load();
    std::cout << "Stored (10, 20), Loaded: (" << loaded.high() << ", " << loaded.low() << ")"
              << std::endl;

    // Exchange
    uint128_t old = atomic_value.exchange(uint128_t(30, 40));
    std::cout << "Exchanged: old = (" << old.high() << ", " << old.low() << ")" << std::endl;

    // Compare-and-swap
    uint128_t expected(30, 40);
    uint128_t desired(50, 60);
    bool success = atomic_value.compare_exchange_strong(expected, desired);
    std::cout << "CAS: " << (success ? "Success" : "Failed") << std::endl;

    loaded = atomic_value.load();
    std::cout << "Final value: (" << loaded.high() << ", " << loaded.low() << ")" << std::endl;
    std::cout << "✓ Success!" << std::endl;
    std::cout << std::endl;
}

// ========================= Example 4: SpinLock for Low Contention =========================

void example_spinlock()
{
    std::cout << "Example 4: SpinLock for Low Contention" << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    ThreadSafeUint128SpinLock fast_counter(uint128_t(0));

    std::vector<std::thread> threads;
    constexpr int NUM_THREADS = 4;
    constexpr int INCREMENTS = 250;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&fast_counter]() {
            for (int j = 0; j < INCREMENTS; ++j) {
                fast_counter.add(uint128_t(0, 1));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    uint128_t final_value = fast_counter.get();
    std::cout << "Final counter value: " << final_value.low() << std::endl;
    std::cout << "Time: " << duration.count() << " µs" << std::endl;
    std::cout << "✓ Success! (SpinLock has lowest overhead for low contention)" << std::endl;
    std::cout << std::endl;
}

// ========================= Example 5: Compare-and-Swap Pattern =========================

void example_cas()
{
    std::cout << "Example 5: Compare-and-Swap Pattern" << std::endl;
    std::cout << "------------------------------------" << std::endl;

    ThreadSafeUint128 shared_value(uint128_t(0, 100));

    // Try to update value only if it's still 100
    uint128_t expected(0, 100);
    uint128_t desired(0, 200);

    bool success = shared_value.compare_exchange(expected, desired);
    std::cout << "First CAS (100 -> 200): " << (success ? "Success" : "Failed") << std::endl;

    // Try again with wrong expected value
    expected = uint128_t(0, 100); // Wrong, it's now 200
    desired = uint128_t(0, 300);
    success = shared_value.compare_exchange(expected, desired);
    std::cout << "Second CAS (100 -> 300): " << (success ? "Success" : "Failed") << std::endl;
    std::cout << "Expected was updated to: " << expected.low() << std::endl;

    uint128_t final_value = shared_value.get();
    std::cout << "Final value: " << final_value.low() << std::endl;
    std::cout << "✓ Success! (CAS prevents lost updates)" << std::endl;
    std::cout << std::endl;
}

// ========================= Example 6: Apply Custom Operation =========================

void example_custom_operation()
{
    std::cout << "Example 6: Apply Custom Operation" << std::endl;
    std::cout << "----------------------------------" << std::endl;

    ThreadSafeUint128 value(uint128_t(0, 10));

    // Apply custom operation atomically
    value.apply([](uint128_t v) {
        return v * uint128_t(0, 2) + uint128_t(0, 5); // v*2 + 5
    });

    uint128_t result = value.get();
    std::cout << "After apply (10*2 + 5): " << result.low() << std::endl;
    std::cout << "✓ Success! (Custom operations are atomic)" << std::endl;
    std::cout << std::endl;
}

// ========================= Main =========================

int main()
{
    std::cout << "==========================================================" << std::endl;
    std::cout << "  Thread-Safety Examples for uint128_t" << std::endl;
    std::cout << "==========================================================" << std::endl;
    std::cout << std::endl;

    example_global_counter();
    example_statistics();
    example_atomic();
    example_spinlock();
    example_cas();
    example_custom_operation();

    std::cout << "==========================================================" << std::endl;
    std::cout << "  All examples completed successfully!" << std::endl;
    std::cout << "==========================================================" << std::endl;
    std::cout << std::endl;

    std::cout << "Summary of Options:" << std::endl;
    std::cout << "  1. ThreadSafeUint128         - General purpose (mutex)" << std::endl;
    std::cout << "  2. ThreadSafeUint128RW       - Read-heavy workloads (RW-lock)" << std::endl;
    std::cout << "  3. ThreadSafeUint128Atomic   - Standard atomic interface" << std::endl;
    std::cout << "  4. ThreadSafeUint128SpinLock - Low contention (spin-lock)" << std::endl;
    std::cout << std::endl;
    std::cout << "Recommendation: Use option 1 (mutex) for most cases." << std::endl;

    return 0;
}

