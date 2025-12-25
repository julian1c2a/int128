/*
 * Thread-safety tests for int128_t
 * These tests verify concurrent access patterns for signed 128-bit integers
 */

#include "../include/int128/int128_t.hpp"
#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

// ========================= BASIC THREAD-SAFETY TESTS =========================

void test_concurrent_reads()
{
    std::cout << "test_concurrent_reads: ";

    // Shared read-only value (signed)
    const int128_t shared_value(int64_t(0x1234567890ABCDEFLL), uint64_t(0xFEDCBA0987654321ULL));
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
                if (high == uint64_t(0x1234567890ABCDEFULL) && low == 0xFEDCBA0987654321ULL) {
                    success_count.fetch_add(1, std::memory_order_relaxed);
                }

                // More read operations
                int128_t copy = shared_value;
                int128_t sum = shared_value + int128_t(1);
                bool is_nonzero = (shared_value != int128_t(0));
                bool is_neg = shared_value.is_negative();

                (void)copy;
                (void)sum;
                (void)is_nonzero;
                (void)is_neg;
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
    std::vector<int128_t> results(NUM_THREADS);

    // Each thread has its own local object
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&results, i]() {
            int128_t local{int64_t(i)};
            for (int j = 0; j < ITERATIONS; ++j) {
                local += int128_t(1);
            }
            results[i] = local;
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // Verify results
    for (int i = 0; i < NUM_THREADS; ++i) {
        assert(results[i] == int128_t{int64_t(i + ITERATIONS)});
    }

    std::cout << "passed (thread-local modification is safe)" << std::endl;
}

void test_copy_construction_concurrent()
{
    std::cout << "test_copy_construction_concurrent: ";

    const int128_t original(int64_t(-0x5555444433332222LL), 0x1111222233334444ULL);
    constexpr int NUM_THREADS = 10;
    constexpr int ITERATIONS = 1000;

    std::vector<std::thread> threads;
    std::atomic<int> success_count{0};

    // Multiple threads copying the same object
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&original, &success_count]() {
            for (int j = 0; j < ITERATIONS; ++j) {
                int128_t copy(original);
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

void test_signed_operations_concurrent()
{
    std::cout << "test_signed_operations_concurrent: ";

    const int128_t negative_value(int64_t(-1000));
    const int128_t positive_value(int64_t(1000));
    constexpr int NUM_THREADS = 10;
    constexpr int ITERATIONS = 500;

    std::vector<std::thread> threads;
    std::atomic<int> neg_count{0};
    std::atomic<int> pos_count{0};

    // Multiple threads performing signed operations
    for (int i = 0; i < NUM_THREADS / 2; ++i) {
        threads.emplace_back([&negative_value, &neg_count]() {
            for (int j = 0; j < ITERATIONS; ++j) {
                bool is_neg = negative_value.is_negative();
                int128_t abs_val = negative_value.is_negative() ? -negative_value : negative_value;
                int128_t negated = -negative_value;

                if (is_neg && abs_val > int128_t(0) && negated > int128_t(0)) {
                    neg_count.fetch_add(1, std::memory_order_relaxed);
                }

                (void)abs_val;
                (void)negated;
            }
        });
    }

    for (int i = 0; i < NUM_THREADS / 2; ++i) {
        threads.emplace_back([&positive_value, &pos_count]() {
            for (int j = 0; j < ITERATIONS; ++j) {
                bool is_pos = !positive_value.is_negative();
                int128_t negated = -positive_value;

                if (is_pos && negated.is_negative()) {
                    pos_count.fetch_add(1, std::memory_order_relaxed);
                }

                (void)negated;
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    assert(neg_count.load() == (NUM_THREADS / 2) * ITERATIONS);
    assert(pos_count.load() == (NUM_THREADS / 2) * ITERATIONS);
    std::cout << "passed (concurrent signed operations are safe)" << std::endl;
}

// ========================= UNSAFE PATTERNS (FOR DEMONSTRATION) =========================

void test_unsafe_concurrent_modification_detected()
{
    std::cout << "test_unsafe_concurrent_modification: ";

    // This test demonstrates WHY concurrent modification is unsafe
    // We expect inconsistent results without synchronization

    int128_t counter(int64_t(0));
    constexpr int NUM_THREADS = 4;
    constexpr int ITERATIONS = 250; // Small to finish quickly

    std::vector<std::thread> threads;

    // UNSAFE: Multiple threads modifying the same object
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < ITERATIONS; ++j) {
                counter += int128_t(1); // RACE CONDITION!
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
              << ", actual: " << counter.to_string() << ") - ";

    if (counter != int128_t(int64_t(NUM_THREADS * ITERATIONS))) {
        std::cout << "passed (race condition confirmed)" << std::endl;
    } else {
        std::cout << "passed (or got lucky!)" << std::endl;
    }
}

// ========================= THREAD-SAFE WRAPPERS =========================

// Wrapper 1: Mutex-based
class ThreadSafeInt128Mutex
{
  private:
    int128_t value_;
    mutable std::mutex mutex_;

  public:
    ThreadSafeInt128Mutex(int128_t val = int128_t(0)) : value_(val) {}

    int128_t get() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return value_;
    }

    void set(int128_t val)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        value_ = val;
    }

    void add(int128_t val)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        value_ += val;
    }

    void negate()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        value_ = -value_;
    }

    bool is_negative() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return value_.is_negative();
    }

    int128_t fetch_add(int128_t val)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        int128_t old = value_;
        value_ += val;
        return old;
    }
};

void test_threadsafe_wrapper_mutex()
{
    std::cout << "test_threadsafe_wrapper_mutex: ";

    ThreadSafeInt128Mutex counter(int128_t(0));
    constexpr int NUM_THREADS = 10;
    constexpr int ITERATIONS = 1000;

    std::vector<std::thread> threads;

    // Multiple threads safely modifying shared counter
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < ITERATIONS; ++j) {
                counter.add(int128_t(1));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    int128_t final_value = counter.get();
    assert(final_value == int128_t(int64_t(NUM_THREADS * ITERATIONS)));

    std::cout << "passed (mutex wrapper is thread-safe)" << std::endl;
}

void test_threadsafe_wrapper_mutex_signed()
{
    std::cout << "test_threadsafe_wrapper_mutex_signed: ";

    ThreadSafeInt128Mutex counter(int128_t(int64_t(-1000)));
    constexpr int NUM_THREADS = 8;
    constexpr int ITERATIONS = 500;

    std::vector<std::thread> threads;

    // Half threads increment, half decrement
    for (int i = 0; i < NUM_THREADS / 2; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < ITERATIONS; ++j) {
                counter.add(int128_t(1));
            }
        });
    }

    for (int i = 0; i < NUM_THREADS / 2; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < ITERATIONS; ++j) {
                counter.add(int128_t(-1));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    int128_t final_value = counter.get();
    // Started at -1000, added and subtracted same amount
    assert(final_value == int128_t(int64_t(-1000)));
    assert(counter.is_negative());

    std::cout << "passed (mutex wrapper handles signed correctly)" << std::endl;
}

// Wrapper 2: Read-Write Lock
class ThreadSafeInt128RW
{
  private:
    int128_t value_;
    mutable std::shared_mutex mutex_;

  public:
    ThreadSafeInt128RW(int128_t val = int128_t(0)) : value_(val) {}

    int128_t get() const
    {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return value_;
    }

    bool is_negative() const
    {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return value_.is_negative();
    }

    void set(int128_t val)
    {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        value_ = val;
    }

    void add(int128_t val)
    {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        value_ += val;
    }

    void negate()
    {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        value_ = -value_;
    }
};

void test_threadsafe_wrapper_rwlock()
{
    std::cout << "test_threadsafe_wrapper_rwlock: ";

    ThreadSafeInt128RW shared_value(int128_t(int64_t(1234)));
    constexpr int NUM_READERS = 8;
    constexpr int NUM_WRITERS = 2;
    constexpr int ITERATIONS = 500;

    std::vector<std::thread> threads;
    std::atomic<int> read_success{0};

    // Multiple reader threads
    for (int i = 0; i < NUM_READERS; ++i) {
        threads.emplace_back([&shared_value, &read_success]() {
            for (int j = 0; j < ITERATIONS; ++j) {
                int128_t val = shared_value.get();
                bool is_neg = shared_value.is_negative();

                // Value should be 1234, -1234, 1235, or -1235
                if ((val == int128_t(int64_t(1234)) || val == int128_t(int64_t(-1234)) ||
                     val == int128_t(int64_t(1235)) || val == int128_t(int64_t(-1235)))) {
                    read_success.fetch_add(1, std::memory_order_relaxed);
                }

                (void)is_neg;
            }
        });
    }

    // A few writer threads
    for (int i = 0; i < NUM_WRITERS; ++i) {
        threads.emplace_back([&shared_value]() {
            for (int j = 0; j < ITERATIONS; ++j) {
                shared_value.set(int128_t(int64_t(1234)));
                shared_value.negate(); // Now -1234
                shared_value.set(int128_t(int64_t(1235)));
                shared_value.negate(); // Now -1235
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

void test_atomic_int128_support()
{
    std::cout << "test_atomic_int128_support: ";

    // Check if std::atomic<int128_t> is supported
    std::atomic<int128_t> atomic_value(int128_t(0));

    bool is_lock_free = atomic_value.is_lock_free();

    std::cout << (is_lock_free ? "lock-free" : "uses locks") << " - ";

    if (is_lock_free) {
        // Test atomic operations
        atomic_value.store(int128_t(int64_t(-100)));
        int128_t loaded = atomic_value.load();
        assert(loaded == int128_t(int64_t(-100)));
        assert(loaded.is_negative());

        std::cout << "passed (std::atomic<int128_t> is lock-free!)" << std::endl;
    } else {
        std::cout << "passed (std::atomic<int128_t> uses locks, wrapper recommended)" << std::endl;
    }
}

void test_atomic_int128_concurrent()
{
    std::cout << "test_atomic_int128_concurrent: ";

    std::atomic<int128_t> atomic_counter(int128_t(int64_t(-500)));
    constexpr int NUM_THREADS = 4;
    constexpr int ITERATIONS = 250;

    std::vector<std::thread> threads;

    // Multiple threads using compare-exchange
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&atomic_counter]() {
            for (int j = 0; j < ITERATIONS; ++j) {
                int128_t current = atomic_counter.load();
                int128_t next = current + int128_t(1);
                while (!atomic_counter.compare_exchange_weak(current, next)) {
                    next = current + int128_t(1);
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    int128_t final_value = atomic_counter.load();
    assert(final_value == int128_t(int64_t(-500 + NUM_THREADS * ITERATIONS)));

    std::cout << "passed (atomic compare-exchange works correctly)" << std::endl;
}

// ========================= PERFORMANCE COMPARISON =========================

void test_performance_comparison()
{
    std::cout << "test_performance_comparison: " << std::endl;

    constexpr int NUM_OPS = 100000;

    // Test 1: Thread-local (no synchronization)
    {
        auto start = std::chrono::high_resolution_clock::now();

        int128_t local(int64_t(0));
        for (int i = 0; i < NUM_OPS; ++i) {
            local += int128_t(1);
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "  - Thread-local (no locks): " << duration.count() << " µs" << std::endl;
    }

    // Test 2: Mutex wrapper
    {
        ThreadSafeInt128Mutex counter(int128_t(0));

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < NUM_OPS; ++i) {
            counter.add(int128_t(1));
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "  - Mutex wrapper: " << duration.count() << " µs" << std::endl;
    }

    // Test 3: Atomic (if lock-free)
    {
        std::atomic<int128_t> atomic_value(int128_t(0));

        if (atomic_value.is_lock_free()) {
            auto start = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < NUM_OPS; ++i) {
                int128_t current = atomic_value.load();
                atomic_value.store(current + int128_t(1));
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

void test_performance_signed_operations()
{
    std::cout << "test_performance_signed_operations: " << std::endl;

    constexpr int NUM_OPS = 50000;

    // Test negation performance
    {
        ThreadSafeInt128Mutex counter(int128_t(int64_t(-1000)));

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < NUM_OPS; ++i) {
            counter.negate();
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "  - Negate operations: " << duration.count() << " µs" << std::endl;
    }

    // Test is_negative checks
    {
        ThreadSafeInt128Mutex counter(int128_t(int64_t(-1000)));
        int neg_count = 0;

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < NUM_OPS; ++i) {
            if (counter.is_negative()) {
                neg_count++;
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "  - is_negative checks: " << duration.count() << " µs" << std::endl;

        (void)neg_count;
    }

    std::cout << "passed (signed operations performance measured)" << std::endl;
}

// ========================= MAIN TEST RUNNER =========================

int main()
{
    std::cout << "Running thread-safety tests for int128_t..." << std::endl;
    std::cout << std::endl;

    std::cout << "=== Basic Thread-Safety Tests ===" << std::endl;
    test_concurrent_reads();
    test_thread_local_modification();
    test_copy_construction_concurrent();
    test_signed_operations_concurrent();

    std::cout << std::endl;
    std::cout << "=== Unsafe Patterns (Demonstration) ===" << std::endl;
    test_unsafe_concurrent_modification_detected();

    std::cout << std::endl;
    std::cout << "=== Thread-Safe Wrappers ===" << std::endl;
    test_threadsafe_wrapper_mutex();
    test_threadsafe_wrapper_mutex_signed();
    test_threadsafe_wrapper_rwlock();

    std::cout << std::endl;
    std::cout << "=== Atomic Support ===" << std::endl;
    test_atomic_int128_support();
    test_atomic_int128_concurrent();

    std::cout << std::endl;
    std::cout << "=== Performance Comparison ===" << std::endl;
    test_performance_comparison();
    test_performance_signed_operations();

    std::cout << std::endl;
    std::cout << "All thread-safety tests completed." << std::endl;
    std::cout << std::endl;
    std::cout << "Summary:" << std::endl;
    std::cout << "  [OK] int128_t is thread-safe for concurrent reads" << std::endl;
    std::cout << "  [OK] int128_t is thread-safe for thread-local modifications" << std::endl;
    std::cout << "  [OK] Signed operations (negate, is_negative) are thread-safe for reads"
              << std::endl;
    std::cout << "  [OK] Concurrent modifications require synchronization wrappers" << std::endl;

    return 0;
}
