/*
 * Thread-safety tests for uint128_t
 * These tests verify concurrent access patterns
 */

#include "../include/uint128/uint128_t.hpp"
#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

using namespace nstd;


// ========================= BASIC THREAD-SAFETY TESTS =========================

void test_concurrent_reads()
{
    std::cout << "test_concurrent_reads: ";

    // Shared read-only value
    const uint128_t shared_value(0x1234567890ABCDEFULL, 0xFEDCBA0987654321ULL);
    constexpr int NUM_THREADS = 10;
    constexpr int ITERATIONS = 1000;

    std::vector<std::thread> threads;
    std::atomic<int> success_count{0};

    // Multiple threads reading the same const object
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&shared_value, &success_count]() {
            for (int j = 0; j < ITERATIONS; ++j) {
                // Read operations (all const)
                uint64_t high = shared_value.high();
                uint64_t low = shared_value.low();

                // Verify consistency
                if (high == 0x1234567890ABCDEFULL && low == 0xFEDCBA0987654321ULL) {
                    success_count.fetch_add(1, std::memory_order_relaxed);
                }

                // More read operations
                uint128_t copy = shared_value;
                uint128_t sum = shared_value + uint128_t(1);
                bool is_nonzero = static_cast<bool>(shared_value);

                (void)copy;
                (void)sum;
                (void)is_nonzero;
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    assert(success_count.load() == NUM_THREADS * ITERATIONS);
    std::cout << "passed (concurrent reads are safe)" << std::endl;
}

void test_thread_local_modification()
{
    std::cout << "test_thread_local_modification: ";

    constexpr int NUM_THREADS = 10;
    constexpr int ITERATIONS = 1000;

    std::vector<std::thread> threads;
    std::vector<uint128_t> results(NUM_THREADS);

    // Each thread has its own local object
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&results, i]() {
            uint128_t local(0, i);
            for (int j = 0; j < ITERATIONS; ++j) {
                local += uint128_t(0, 1);
            }
            results[i] = local;
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // Verify results
    for (int i = 0; i < NUM_THREADS; ++i) {
        assert(results[i] == uint128_t(0, i + ITERATIONS));
    }

    std::cout << "passed (thread-local modification is safe)" << std::endl;
}

void test_copy_construction_concurrent()
{
    std::cout << "test_copy_construction_concurrent: ";

    const uint128_t original(0xAAAABBBBCCCCDDDDULL, 0x1111222233334444ULL);
    constexpr int NUM_THREADS = 10;
    constexpr int ITERATIONS = 1000;

    std::vector<std::thread> threads;
    std::atomic<int> success_count{0};

    // Multiple threads copying the same object
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&original, &success_count]() {
            for (int j = 0; j < ITERATIONS; ++j) {
                uint128_t copy(original);
                if (copy.high() == original.high() && copy.low() == original.low()) {
                    success_count.fetch_add(1, std::memory_order_relaxed);
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    assert(success_count.load() == NUM_THREADS * ITERATIONS);
    std::cout << "passed (concurrent copy construction is safe)" << std::endl;
}

// ========================= UNSAFE PATTERNS (FOR DEMONSTRATION) =========================

void test_unsafe_concurrent_modification_detected()
{
    std::cout << "test_unsafe_concurrent_modification: ";

    // This test demonstrates WHY concurrent modification is unsafe
    // We expect inconsistent results without synchronization

    uint128_t counter(0);
    constexpr int NUM_THREADS = 4;
    constexpr int ITERATIONS = 250; // Small to finish quickly

    std::vector<std::thread> threads;

    // UNSAFE: Multiple threads modifying the same object
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < ITERATIONS; ++j) {
                counter += uint128_t(0, 1); // RACE CONDITION!
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // The final value is UNPREDICTABLE due to races
    // Expected: NUM_THREADS * ITERATIONS = 1000
    // Actual: Usually less due to lost updates
    std::cout << "detected race condition (expected: " << NUM_THREADS * ITERATIONS
              << ", actual: " << counter.low() << ") - ";

    if (counter != uint128_t(0, NUM_THREADS * ITERATIONS)) {
        std::cout << "passed (race condition confirmed)" << std::endl;
    } else {
        std::cout << "passed (or got lucky!)" << std::endl;
    }
}

// ========================= THREAD-SAFE WRAPPERS =========================

// Wrapper 1: Mutex-based
class ThreadSafeUint128Mutex
{
  private:
    uint128_t value_;
    mutable std::mutex mutex_;

  public:
    ThreadSafeUint128Mutex(uint128_t val = uint128_t(0)) : value_(val) {}

    uint128_t get() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return value_;
    }

    void set(uint128_t val)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        value_ = val;
    }

    void add(uint128_t val)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        value_ += val;
    }

    uint128_t fetch_add(uint128_t val)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        uint128_t old = value_;
        value_ += val;
        return old;
    }
};

void test_threadsafe_wrapper_mutex()
{
    std::cout << "test_threadsafe_wrapper_mutex: ";

    ThreadSafeUint128Mutex counter(uint128_t(0));
    constexpr int NUM_THREADS = 10;
    constexpr int ITERATIONS = 1000;

    std::vector<std::thread> threads;

    // Multiple threads safely modifying shared counter
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < ITERATIONS; ++j) {
                counter.add(uint128_t(0, 1));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    uint128_t final_value = counter.get();
    assert(final_value == uint128_t(0, NUM_THREADS * ITERATIONS));

    std::cout << "passed (mutex wrapper is thread-safe)" << std::endl;
}

// Wrapper 2: Read-Write Lock
class ThreadSafeUint128RW
{
  private:
    uint128_t value_;
    mutable std::shared_mutex mutex_;

  public:
    ThreadSafeUint128RW(uint128_t val = uint128_t(0)) : value_(val) {}

    uint128_t get() const
    {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return value_;
    }

    void set(uint128_t val)
    {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        value_ = val;
    }

    void add(uint128_t val)
    {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        value_ += val;
    }
};

void test_threadsafe_wrapper_rwlock()
{
    std::cout << "test_threadsafe_wrapper_rwlock: ";

    ThreadSafeUint128RW shared_value(uint128_t(0x1234, 0x5678));
    constexpr int NUM_READERS = 8;
    constexpr int NUM_WRITERS = 2;
    constexpr int ITERATIONS = 500;

    std::vector<std::thread> threads;
    std::atomic<int> read_success{0};

    // Multiple reader threads
    for (int i = 0; i < NUM_READERS; ++i) {
        threads.emplace_back([&shared_value, &read_success]() {
            for (int j = 0; j < ITERATIONS; ++j) {
                uint128_t val = shared_value.get();
                if (val.high() == 0x1234 || val.high() == 0x1235) {
                    read_success.fetch_add(1, std::memory_order_relaxed);
                }
            }
        });
    }

    // A few writer threads
    for (int i = 0; i < NUM_WRITERS; ++i) {
        threads.emplace_back([&shared_value]() {
            for (int j = 0; j < ITERATIONS; ++j) {
                shared_value.set(uint128_t(0x1234, 0x5678));
                shared_value.set(uint128_t(0x1235, 0x5679));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    assert(read_success.load() == NUM_READERS * ITERATIONS);
    std::cout << "passed (RW-lock wrapper is thread-safe)" << std::endl;
}

// ========================= ATOMIC VERIFICATION =========================

void test_atomic_uint128_support()
{
    std::cout << "test_atomic_uint128_support: ";

    // Check if std::atomic<uint128_t> is supported
    std::atomic<uint128_t> atomic_value(uint128_t(0));

    bool is_lock_free = atomic_value.is_lock_free();

    std::cout << (is_lock_free ? "lock-free" : "uses locks") << " - ";

    if (is_lock_free) {
        // Test atomic operations
        atomic_value.store(uint128_t(100, 200));
        uint128_t loaded = atomic_value.load();
        assert(loaded.high() == 100 && loaded.low() == 200);

        std::cout << "passed (std::atomic<uint128_t> is lock-free!)" << std::endl;
    } else {
        std::cout << "passed (std::atomic<uint128_t> uses locks, wrapper recommended)" << std::endl;
    }
}

// ========================= PERFORMANCE COMPARISON =========================

void test_performance_comparison()
{
    std::cout << "test_performance_comparison: " << std::endl;

    constexpr int NUM_OPS = 100000;

    // Test 1: Thread-local (no synchronization)
    {
        auto start = std::chrono::high_resolution_clock::now();

        uint128_t local(0);
        for (int i = 0; i < NUM_OPS; ++i) {
            local += uint128_t(0, 1);
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "  - Thread-local (no locks): " << duration.count() << " µs" << std::endl;
    }

    // Test 2: Mutex wrapper
    {
        ThreadSafeUint128Mutex counter(uint128_t(0));

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < NUM_OPS; ++i) {
            counter.add(uint128_t(0, 1));
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "  - Mutex wrapper: " << duration.count() << " µs" << std::endl;
    }

    // Test 3: Atomic (if lock-free)
    {
        std::atomic<uint128_t> atomic_value(uint128_t(0));

        if (atomic_value.is_lock_free()) {
            auto start = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < NUM_OPS; ++i) {
                uint128_t current = atomic_value.load();
                atomic_value.store(current + uint128_t(0, 1));
            }

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            std::cout << "  - Atomic (lock-free): " << duration.count() << " µs" << std::endl;
        } else {
            std::cout << "  - Atomic: N/A (not lock-free)" << std::endl;
        }
    }

    std::cout << "passed (performance comparison completed)" << std::endl;
}

// ========================= MAIN TEST RUNNER =========================

int main()
{
    std::cout << "Running thread-safety tests for uint128_t..." << std::endl;
    std::cout << std::endl;

    std::cout << "=== Basic Thread-Safety Tests ===" << std::endl;
    test_concurrent_reads();
    test_thread_local_modification();
    test_copy_construction_concurrent();

    std::cout << std::endl;
    std::cout << "=== Unsafe Patterns (Demonstration) ===" << std::endl;
    test_unsafe_concurrent_modification_detected();

    std::cout << std::endl;
    std::cout << "=== Thread-Safe Wrappers ===" << std::endl;
    test_threadsafe_wrapper_mutex();
    test_threadsafe_wrapper_rwlock();

    std::cout << std::endl;
    std::cout << "=== Atomic Support ===" << std::endl;
    test_atomic_uint128_support();

    std::cout << std::endl;
    std::cout << "=== Performance Comparison ===" << std::endl;
    test_performance_comparison();

    std::cout << std::endl;
    std::cout << "All thread-safety tests completed." << std::endl;
    std::cout << std::endl;
    std::cout << "Summary:" << std::endl;
    std::cout << "  [OK] uint128_t is thread-safe for concurrent reads" << std::endl;
    std::cout << "  [OK] uint128_t is thread-safe for thread-local modifications" << std::endl;
    std::cout << "  [OK] Concurrent modifications require synchronization wrappers" << std::endl;

    return 0;
}
