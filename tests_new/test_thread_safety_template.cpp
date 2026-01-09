/**
 * @file test_thread_safety_template.cpp
 * @brief Tests para int128_base_thread_safety.hpp
 *
 * Tests: ~30 total
 * - ThreadSafe<S> (mutex-based)
 * - ThreadSafeRW<S> (read-write lock)
 * - ThreadSafeAtomic<S> (atomic wrapper)
 * - ThreadSafeSpinLock<S> (spin-lock)
 *
 * @note Compilar con: -pthread (Linux/MSYS2)
 */

#include "../include_new/int128_base_thread_safety.hpp"
#include <iostream>
#include <thread>
#include <vector>

using namespace nstd;
using namespace nstd::int128_threadsafe;

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name, condition)                                                                      \
    do {                                                                                           \
        if (condition) {                                                                           \
            std::cout << "[PASS] " << name << std::endl;                                           \
            tests_passed++;                                                                        \
        } else {                                                                                   \
            std::cout << "[FAIL] " << name << std::endl;                                           \
            tests_failed++;                                                                        \
        }                                                                                          \
    } while (0)

// =============================================================================
// Tests: ThreadSafe<S> (Mutex-based)
// =============================================================================

void test_threadsafe_basic_unsigned()
{
    ThreadSafeUint128 counter(uint128_t(100));

    TEST("ThreadSafe unsigned get()", counter.get() == uint128_t(100));

    counter.set(uint128_t(200));
    TEST("ThreadSafe unsigned set()", counter.get() == uint128_t(200));

    counter.add(uint128_t(50));
    TEST("ThreadSafe unsigned add()", counter.get() == uint128_t(250));

    counter.subtract(uint128_t(30));
    TEST("ThreadSafe unsigned subtract()", counter.get() == uint128_t(220));

    counter.multiply(uint128_t(2));
    TEST("ThreadSafe unsigned multiply()", counter.get() == uint128_t(440));

    counter.divide(uint128_t(4));
    TEST("ThreadSafe unsigned divide()", counter.get() == uint128_t(110));
}

void test_threadsafe_basic_signed()
{
    ThreadSafeInt128 counter(int128_t(-50));

    TEST("ThreadSafe signed get()", counter.get() == int128_t(-50));

    counter.add(int128_t(100));
    TEST("ThreadSafe signed add()", counter.get() == int128_t(50));

    counter.subtract(int128_t(75));
    TEST("ThreadSafe signed subtract()", counter.get() == int128_t(-25));

    counter.multiply(int128_t(-2));
    TEST("ThreadSafe signed multiply()", counter.get() == int128_t(50));
}

void test_threadsafe_fetch_ops()
{
    ThreadSafeUint128 counter(uint128_t(100));

    auto old = counter.fetch_add(uint128_t(10));
    TEST("fetch_add returns old value", old == uint128_t(100));
    TEST("fetch_add updates value", counter.get() == uint128_t(110));

    old = counter.fetch_sub(uint128_t(5));
    TEST("fetch_sub returns old value", old == uint128_t(110));
    TEST("fetch_sub updates value", counter.get() == uint128_t(105));
}

void test_threadsafe_compare_exchange()
{
    ThreadSafeUint128 counter(uint128_t(100));

    uint128_t expected = uint128_t(100);
    bool success = counter.compare_exchange(expected, uint128_t(200));
    TEST("compare_exchange success", success && counter.get() == uint128_t(200));

    expected = uint128_t(100); // Wrong expectation
    success = counter.compare_exchange(expected, uint128_t(300));
    TEST("compare_exchange failure updates expected",
         !success && expected == uint128_t(200) && counter.get() == uint128_t(200));
}

void test_threadsafe_increment_decrement()
{
    ThreadSafeUint128 counter(uint128_t(100));

    auto val = ++counter;
    TEST("pre-increment returns new", val == uint128_t(101));

    val = counter++;
    TEST("post-increment returns old", val == uint128_t(101));
    TEST("post-increment updates", counter.get() == uint128_t(102));

    val = --counter;
    TEST("pre-decrement returns new", val == uint128_t(101));

    val = counter--;
    TEST("post-decrement returns old", val == uint128_t(101));
    TEST("post-decrement updates", counter.get() == uint128_t(100));
}

void test_threadsafe_apply()
{
    ThreadSafeUint128 counter(uint128_t(10));

    auto result = counter.apply([](uint128_t v) { return v * v; });
    TEST("apply (square)", result == uint128_t(100) && counter.get() == uint128_t(100));
}

// =============================================================================
// Tests: ThreadSafeRW<S> (Read-Write Lock)
// =============================================================================

void test_threadsafe_rw()
{
    ThreadSafeUint128RW stats(uint128_t(1000));

    TEST("RW get()", stats.get() == uint128_t(1000));

    stats.set(uint128_t(2000));
    TEST("RW set()", stats.get() == uint128_t(2000));

    stats.add(uint128_t(500));
    TEST("RW add()", stats.get() == uint128_t(2500));

    // Test read lambda
    auto doubled = stats.read([](const uint128_t& v) { return v * uint128_t(2); });
    TEST("RW read() lambda", doubled == uint128_t(5000));

    // Test modify lambda
    stats.modify([](uint128_t& v) { v += uint128_t(100); });
    TEST("RW modify() lambda", stats.get() == uint128_t(2600));
}

// =============================================================================
// Tests: ThreadSafeAtomic<S>
// =============================================================================

void test_threadsafe_atomic()
{
    ThreadSafeUint128Atomic atomic_val(uint128_t(500));

    TEST("Atomic load()", atomic_val.load() == uint128_t(500));
    TEST("Atomic get()", atomic_val.get() == uint128_t(500));

    atomic_val.store(uint128_t(600));
    TEST("Atomic store()", atomic_val.get() == uint128_t(600));

    auto old = atomic_val.exchange(uint128_t(700));
    TEST("Atomic exchange() returns old", old == uint128_t(600));
    TEST("Atomic exchange() updates", atomic_val.get() == uint128_t(700));

    // is_lock_free típicamente es false para 128 bits
    std::cout << "  [INFO] Atomic is_lock_free: " << (atomic_val.is_lock_free() ? "yes" : "no")
              << std::endl;
}

// =============================================================================
// Tests: ThreadSafeSpinLock<S>
// =============================================================================

void test_threadsafe_spinlock()
{
    ThreadSafeUint128SpinLock fast(uint128_t(100));

    TEST("SpinLock get()", fast.get() == uint128_t(100));

    fast.set(uint128_t(200));
    TEST("SpinLock set()", fast.get() == uint128_t(200));

    fast.add(uint128_t(50));
    TEST("SpinLock add()", fast.get() == uint128_t(250));

    auto old = fast.fetch_add(uint128_t(10));
    TEST("SpinLock fetch_add()", old == uint128_t(250) && fast.get() == uint128_t(260));
}

// =============================================================================
// Tests: Concurrent Operations (simple)
// =============================================================================

void test_concurrent_increment()
{
    constexpr int NUM_THREADS = 4;
    constexpr int INCREMENTS_PER_THREAD = 1000;

    ThreadSafeUint128 counter(uint128_t(0));

    std::vector<std::thread> threads;
    for (int t = 0; t < NUM_THREADS; ++t) {
        threads.emplace_back([&counter]() {
            for (int i = 0; i < INCREMENTS_PER_THREAD; ++i) {
                counter.add(uint128_t(1));
            }
        });
    }

    for (auto& th : threads) {
        th.join();
    }

    uint128_t expected = uint128_t(NUM_THREADS * INCREMENTS_PER_THREAD);
    TEST("Concurrent increment (4 threads x 1000)", counter.get() == expected);
}

void test_concurrent_signed()
{
    constexpr int NUM_THREADS = 4;
    constexpr int OPS_PER_THREAD = 500;

    ThreadSafeInt128 counter(int128_t(0));

    std::vector<std::thread> threads;
    // 2 threads incrementan, 2 decrementan
    for (int t = 0; t < NUM_THREADS; ++t) {
        if (t < 2) {
            threads.emplace_back([&counter]() {
                for (int i = 0; i < OPS_PER_THREAD; ++i) {
                    counter.add(int128_t(1));
                }
            });
        } else {
            threads.emplace_back([&counter]() {
                for (int i = 0; i < OPS_PER_THREAD; ++i) {
                    counter.subtract(int128_t(1));
                }
            });
        }
    }

    for (auto& th : threads) {
        th.join();
    }

    // 2*500 increments - 2*500 decrements = 0
    TEST("Concurrent signed (inc/dec balance)", counter.get() == int128_t(0));
}

// =============================================================================
// Main
// =============================================================================

int main()
{
    std::cout << "=== Tests: int128_base_thread_safety.hpp ===" << std::endl << std::endl;

    // ThreadSafe (mutex)
    std::cout << "--- ThreadSafe<S> (Mutex) ---" << std::endl;
    test_threadsafe_basic_unsigned();
    test_threadsafe_basic_signed();
    test_threadsafe_fetch_ops();
    test_threadsafe_compare_exchange();
    test_threadsafe_increment_decrement();
    test_threadsafe_apply();

    // ThreadSafeRW
    std::cout << std::endl << "--- ThreadSafeRW<S> (RW-Lock) ---" << std::endl;
    test_threadsafe_rw();

    // ThreadSafeAtomic
    std::cout << std::endl << "--- ThreadSafeAtomic<S> ---" << std::endl;
    test_threadsafe_atomic();

    // ThreadSafeSpinLock
    std::cout << std::endl << "--- ThreadSafeSpinLock<S> ---" << std::endl;
    test_threadsafe_spinlock();

    // Concurrent tests
    std::cout << std::endl << "--- Concurrent Tests ---" << std::endl;
    test_concurrent_increment();
    test_concurrent_signed();

    std::cout << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Passed: " << tests_passed << "/" << (tests_passed + tests_failed) << std::endl;
    std::cout << "==================================" << std::endl;

    return tests_failed > 0 ? 1 : 0;
}
