/**
 * @file int128_base_thread_safety_tests.cpp
 * @brief Tests para int128_base_thread_safety.hpp (template unificado)
 */

#include "int128_base_thread_safety.hpp"
#include "int128_base_tt.hpp"

#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

using namespace nstd;
using namespace nstd::int128_threadsafe;

// Contador de tests
static int tests_passed = 0;
static int tests_failed = 0;

void test_passed(const char* name)
{
    std::cout << "[PASS] " << name << std::endl;
    tests_passed++;
}

void test_failed(const char* name, const char* reason)
{
    std::cout << "[FAIL] " << name << ": " << reason << std::endl;
    tests_failed++;
}

// =============================================================================
// TEST THREADSAFE (MUTEX)
// =============================================================================

void test_threadsafe_basic()
{
    ThreadSafeUint128 counter(uint128_t(0));

    counter.set(uint128_t(100));
    assert(counter.get() == uint128_t(100));

    counter.add(uint128_t(50));
    assert(counter.get() == uint128_t(150));

    counter.subtract(uint128_t(25));
    assert(counter.get() == uint128_t(125));

    test_passed("ThreadSafe basic operations");
}

void test_threadsafe_fetch_ops()
{
    ThreadSafeUint128 counter(uint128_t(100));

    uint128_t old = counter.fetch_add(uint128_t(10));
    assert(old == uint128_t(100));
    assert(counter.get() == uint128_t(110));

    old = counter.fetch_sub(uint128_t(5));
    assert(old == uint128_t(110));
    assert(counter.get() == uint128_t(105));

    test_passed("ThreadSafe fetch operations");
}

void test_threadsafe_increment()
{
    ThreadSafeUint128 counter(uint128_t(0));

    uint128_t result = ++counter;
    assert(result == uint128_t(1));
    assert(counter.get() == uint128_t(1));

    result = counter++;
    assert(result == uint128_t(1));
    assert(counter.get() == uint128_t(2));

    result = --counter;
    assert(result == uint128_t(1));
    assert(counter.get() == uint128_t(1));

    result = counter--;
    assert(result == uint128_t(1));
    assert(counter.get() == uint128_t(0));

    test_passed("ThreadSafe increment/decrement");
}

void test_threadsafe_compare_exchange()
{
    ThreadSafeUint128 counter(uint128_t(100));

    uint128_t expected = uint128_t(100);
    bool success = counter.compare_exchange(expected, uint128_t(200));
    assert(success == true);
    assert(counter.get() == uint128_t(200));

    expected = uint128_t(100);
    success = counter.compare_exchange(expected, uint128_t(300));
    assert(success == false);
    assert(expected == uint128_t(200));

    test_passed("ThreadSafe compare_exchange");
}

void test_threadsafe_concurrent()
{
    ThreadSafeUint128 counter(uint128_t(0));
    const int num_threads = 4;
    const int increments_per_thread = 1000;

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&counter, increments_per_thread]() {
            for (int j = 0; j < increments_per_thread; ++j) {
                counter.add(uint128_t(1));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    uint128_t expected = uint128_t(num_threads * increments_per_thread);
    assert(counter.get() == expected);

    test_passed("ThreadSafe concurrent add");
}

// =============================================================================
// TEST THREADSAFERW (READ-WRITE LOCK)
// =============================================================================

void test_threadsaferw_basic()
{
    ThreadSafeUint128RW counter(uint128_t(0));

    counter.set(uint128_t(100));
    assert(counter.get() == uint128_t(100));

    counter.add(uint128_t(50));
    assert(counter.get() == uint128_t(150));

    test_passed("ThreadSafeRW basic operations");
}

void test_threadsaferw_concurrent_reads()
{
    ThreadSafeUint128RW counter(uint128_t(12345));
    const int num_readers = 8;

    std::vector<std::thread> threads;
    std::atomic<int> successful_reads{0};

    for (int i = 0; i < num_readers; ++i) {
        threads.emplace_back([&counter, &successful_reads]() {
            for (int j = 0; j < 100; ++j) {
                uint128_t val = counter.get();
                if (val == uint128_t(12345)) {
                    successful_reads++;
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    assert(successful_reads == num_readers * 100);
    test_passed("ThreadSafeRW concurrent reads");
}

// =============================================================================
// TEST THREADSAFEATOMIC
// Note: std::atomic<128-bit> may have issues on some compilers/platforms
// These tests are kept minimal to avoid hangs
// =============================================================================

void test_threadsafeatomic_basic()
{
#ifdef SKIP_ALL_ATOMIC_TESTS
    std::cout << "[SKIP] ThreadSafeAtomic store/load (disabled for this compiler)" << std::endl;
#else
    ThreadSafeUint128Atomic counter(uint128_t(0));

    // ThreadSafeAtomic uses std::atomic directly
    // Only has: load, store, exchange, compare_exchange_weak, compare_exchange_strong
    // Convenience: get, set

    counter.store(uint128_t(100));
    assert(counter.load() == uint128_t(100));

    counter.set(uint128_t(150));
    assert(counter.get() == uint128_t(150));

    test_passed("ThreadSafeAtomic store/load");
#endif
}

void test_threadsafeatomic_exchange()
{
#ifdef SKIP_ATOMIC_EXCHANGE_TESTS
    std::cout << "[SKIP] ThreadSafeAtomic exchange (disabled for this compiler)" << std::endl;
#else
    ThreadSafeUint128Atomic counter(uint128_t(100));

    // Note: exchange may hang on some Clang versions with 128-bit atomics
    // Test basic functionality only
    uint128_t old = counter.exchange(uint128_t(200));
    assert(old == uint128_t(100));
    assert(counter.get() == uint128_t(200));

    test_passed("ThreadSafeAtomic exchange");
#endif
}

void test_threadsafeatomic_compare_exchange()
{
#ifdef SKIP_ATOMIC_EXCHANGE_TESTS
    std::cout << "[SKIP] ThreadSafeAtomic compare_exchange (disabled for this compiler)"
              << std::endl;
#else
    ThreadSafeUint128Atomic counter(uint128_t(100));

    // Test compare_exchange_strong
    uint128_t expected = uint128_t(100);
    bool success = counter.compare_exchange_strong(expected, uint128_t(200));
    assert(success == true);
    assert(counter.get() == uint128_t(200));

    // Should fail now (value is 200, not 100)
    expected = uint128_t(100);
    success = counter.compare_exchange_strong(expected, uint128_t(300));
    assert(success == false);
    assert(expected == uint128_t(200)); // expected is updated to actual value

    test_passed("ThreadSafeAtomic compare_exchange");
#endif
}

void test_threadsafeatomic_is_lock_free()
{
#ifdef SKIP_ALL_ATOMIC_TESTS
    std::cout << "[SKIP] ThreadSafeAtomic is_lock_free (disabled for this compiler)" << std::endl;
#else
    ThreadSafeUint128Atomic counter(uint128_t(0));

    // Just check it doesn't crash - 128-bit atomics are usually NOT lock-free
    bool lock_free = counter.is_lock_free();
    std::cout << "  (is_lock_free: " << (lock_free ? "true" : "false") << ")" << std::endl;

    test_passed("ThreadSafeAtomic is_lock_free check");
#endif
}

// =============================================================================
// TEST THREADSAFESPINLOCK
// =============================================================================

void test_threadsafespinlock_basic()
{
    ThreadSafeUint128SpinLock counter(uint128_t(0));

    counter.set(uint128_t(100));
    assert(counter.get() == uint128_t(100));

    counter.add(uint128_t(50));
    assert(counter.get() == uint128_t(150));

    test_passed("ThreadSafeSpinLock basic operations");
}

void test_threadsafespinlock_concurrent()
{
    ThreadSafeUint128SpinLock counter(uint128_t(0));
    const int num_threads = 4;
    const int increments_per_thread = 1000;

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&counter, increments_per_thread]() {
            for (int j = 0; j < increments_per_thread; ++j) {
                counter.add(uint128_t(1));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    uint128_t expected = uint128_t(num_threads * increments_per_thread);
    assert(counter.get() == expected);

    test_passed("ThreadSafeSpinLock concurrent add");
}

// =============================================================================
// TEST INT128 (SIGNED)
// =============================================================================

void test_int128_threadsafe()
{
    ThreadSafeInt128 counter(int128_t(0));

    counter.set(int128_t(-100));
    assert(counter.get() == int128_t(-100));

    counter.add(int128_t(150));
    assert(counter.get() == int128_t(50));

    counter.subtract(int128_t(100));
    assert(counter.get() == int128_t(-50));

    test_passed("ThreadSafe int128_t basic operations");
}

void test_int128_concurrent()
{
    ThreadSafeInt128 counter(int128_t(0));
    const int num_threads = 4;
    const int ops_per_thread = 500;

    std::vector<std::thread> threads;

    // Half threads add, half subtract
    for (int i = 0; i < num_threads; ++i) {
        if (i % 2 == 0) {
            threads.emplace_back([&counter, ops_per_thread]() {
                for (int j = 0; j < ops_per_thread; ++j) {
                    counter.add(int128_t(1));
                }
            });
        } else {
            threads.emplace_back([&counter, ops_per_thread]() {
                for (int j = 0; j < ops_per_thread; ++j) {
                    counter.subtract(int128_t(1));
                }
            });
        }
    }

    for (auto& t : threads) {
        t.join();
    }

    // Should be 0 (equal adds and subtracts)
    assert(counter.get() == int128_t(0));

    test_passed("ThreadSafe int128_t concurrent add/subtract");
}

// =============================================================================
// MAIN
// =============================================================================

int main()
{
    std::cout << "=== int128_base_thread_safety Tests ===" << std::endl;
    std::cout << std::endl;

    // ThreadSafe (mutex) tests
    std::cout << "--- ThreadSafe (Mutex) ---" << std::endl;
    test_threadsafe_basic();
    test_threadsafe_fetch_ops();
    test_threadsafe_increment();
    test_threadsafe_compare_exchange();
    test_threadsafe_concurrent();

    // ThreadSafeRW tests
    std::cout << "\n--- ThreadSafeRW (Read-Write Lock) ---" << std::endl;
    test_threadsaferw_basic();
    test_threadsaferw_concurrent_reads();

    // ThreadSafeAtomic tests
    std::cout << "\n--- ThreadSafeAtomic ---" << std::endl;
    test_threadsafeatomic_basic();
    test_threadsafeatomic_exchange();
    test_threadsafeatomic_compare_exchange();
    test_threadsafeatomic_is_lock_free();

    // ThreadSafeSpinLock tests
    std::cout << "\n--- ThreadSafeSpinLock ---" << std::endl;
    test_threadsafespinlock_basic();
    test_threadsafespinlock_concurrent();

    // int128_t tests
    std::cout << "\n--- int128_t (Signed) ThreadSafe ---" << std::endl;
    test_int128_threadsafe();
    test_int128_concurrent();

    // Summary
    std::cout << "\n=== Summary ===" << std::endl;
    std::cout << "Tests passed: " << tests_passed << std::endl;
    std::cout << "Tests failed: " << tests_failed << std::endl;

    if (tests_failed == 0) {
        std::cout << "\n*** ALL TESTS PASSED! ***" << std::endl;
        return 0;
    } else {
        std::cout << "\n*** SOME TESTS FAILED ***" << std::endl;
        return 1;
    }
}
