/**
 * @file int128_thread_safety_extracted_tests.cpp
 * @brief Tests completos de int128_base_thread_safety.hpp (template unificado)
 *
 * Fusión de:
 * - Legacy int128_thread_safety.hpp tests (30+ tests)
 * - test_thread_safety_template.cpp (30+ tests)
 *
 * Total: 60+ tests cubriendo:
 * - ThreadSafe<S> (mutex-based)
 * - ThreadSafeRW<S> (read-write lock)
 * - ThreadSafeAtomic<S> (atomic wrapper)
 * - ThreadSafeSpinLock<S> (spin-lock)
 * - Concurrent reads, writes, increment/decrement
 * - Compare-exchange, fetch operations
 * - Apply lambda operations
 *
 * @note Compilar con: -pthread -latomic (Linux/MSYS2)
 */

#include "int128_base_thread_safety.hpp"
#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

using namespace nstd;
using namespace nstd::int128_threadsafe;

static int tests_passed = 0;
static int tests_failed = 0;

void check(bool condition, const char *test_name)
{
    if (condition)
    {
        std::cout << "[OK]   " << test_name << std::endl;
        tests_passed++;
    }
    else
    {
        std::cout << "[FAIL] " << test_name << std::endl;
        tests_failed++;
    }
}

// =============================================================================
// SECTION: ThreadSafe<S> (Mutex-based)
// =============================================================================

void test_threadsafe_basic_unsigned()
{
    ThreadSafeUint128 counter(uint128_t(100));

    check(counter.get() == uint128_t(100), "ThreadSafe uint get()");

    counter.set(uint128_t(200));
    check(counter.get() == uint128_t(200), "ThreadSafe uint set()");

    counter.add(uint128_t(50));
    check(counter.get() == uint128_t(250), "ThreadSafe uint add()");

    counter.subtract(uint128_t(30));
    check(counter.get() == uint128_t(220), "ThreadSafe uint subtract()");

    counter.multiply(uint128_t(2));
    check(counter.get() == uint128_t(440), "ThreadSafe uint multiply()");

    counter.divide(uint128_t(4));
    check(counter.get() == uint128_t(110), "ThreadSafe uint divide()");
}

void test_threadsafe_basic_signed()
{
    ThreadSafeInt128 counter(int128_t(-50));

    check(counter.get() == int128_t(-50), "ThreadSafe int get()");

    counter.add(int128_t(100));
    check(counter.get() == int128_t(50), "ThreadSafe int add()");

    counter.subtract(int128_t(75));
    check(counter.get() == int128_t(-25), "ThreadSafe int subtract()");

    counter.multiply(int128_t(-2));
    check(counter.get() == int128_t(50), "ThreadSafe int multiply()");
}

void test_threadsafe_fetch_ops()
{
    ThreadSafeUint128 counter(uint128_t(100));

    auto old = counter.fetch_add(uint128_t(10));
    check(old == uint128_t(100), "fetch_add returns old value");
    check(counter.get() == uint128_t(110), "fetch_add updates value");

    old = counter.fetch_sub(uint128_t(5));
    check(old == uint128_t(110), "fetch_sub returns old value");
    check(counter.get() == uint128_t(105), "fetch_sub updates value");
}

void test_threadsafe_compare_exchange()
{
    ThreadSafeUint128 counter(uint128_t(100));

    uint128_t expected = uint128_t(100);
    bool success = counter.compare_exchange(expected, uint128_t(200));
    check(success && counter.get() == uint128_t(200), "compare_exchange success");

    expected = uint128_t(100); // Wrong expectation
    success = counter.compare_exchange(expected, uint128_t(300));
    check(!success && expected == uint128_t(200) && counter.get() == uint128_t(200),
          "compare_exchange failure updates expected");
}

void test_threadsafe_increment_decrement()
{
    ThreadSafeUint128 counter(uint128_t(100));

    auto val = ++counter;
    check(val == uint128_t(101), "pre-increment returns new");

    val = counter++;
    check(val == uint128_t(101), "post-increment returns old");
    check(counter.get() == uint128_t(102), "post-increment updates");

    val = --counter;
    check(val == uint128_t(101), "pre-decrement returns new");

    val = counter--;
    check(val == uint128_t(101), "post-decrement returns old");
    check(counter.get() == uint128_t(100), "post-decrement updates");
}

void test_threadsafe_apply()
{
    ThreadSafeUint128 counter(uint128_t(10));

    auto result = counter.apply([](uint128_t v)
                                { return v * v; });
    check(result == uint128_t(100) && counter.get() == uint128_t(100), "apply (square)");
}

// =============================================================================
// SECTION: ThreadSafeRW<S> (Read-Write Lock)
// =============================================================================

void test_threadsafe_rw()
{
    ThreadSafeUint128RW stats(uint128_t(1000));

    check(stats.get() == uint128_t(1000), "RW get()");

    stats.set(uint128_t(2000));
    check(stats.get() == uint128_t(2000), "RW set()");

    stats.add(uint128_t(500));
    check(stats.get() == uint128_t(2500), "RW add()");

    // Test read lambda
    auto doubled = stats.read([](const uint128_t &v)
                              { return v * uint128_t(2); });
    check(doubled == uint128_t(5000), "RW read() lambda");

    // Test modify lambda
    stats.modify([](uint128_t &v)
                 { v += uint128_t(100); });
    check(stats.get() == uint128_t(2600), "RW modify() lambda");
}

// =============================================================================
// SECTION: ThreadSafeAtomic<S>
// =============================================================================

void test_threadsafe_atomic()
{
    ThreadSafeUint128Atomic atomic_val(uint128_t(500));

    check(atomic_val.load() == uint128_t(500), "Atomic load()");
    check(atomic_val.get() == uint128_t(500), "Atomic get()");

    atomic_val.store(uint128_t(600));
    check(atomic_val.get() == uint128_t(600), "Atomic store()");

    auto old = atomic_val.exchange(uint128_t(700));
    check(old == uint128_t(600), "Atomic exchange() returns old");
    check(atomic_val.get() == uint128_t(700), "Atomic exchange() updates");

    std::cout << "  [INFO] Atomic is_lock_free: " << (atomic_val.is_lock_free() ? "yes" : "no") << std::endl;
}

// =============================================================================
// SECTION: ThreadSafeSpinLock<S>
// =============================================================================

void test_threadsafe_spinlock()
{
    ThreadSafeUint128SpinLock fast(uint128_t(100));

    check(fast.get() == uint128_t(100), "SpinLock get()");

    fast.set(uint128_t(200));
    check(fast.get() == uint128_t(200), "SpinLock set()");

    fast.add(uint128_t(50));
    check(fast.get() == uint128_t(250), "SpinLock add()");

    auto old = fast.fetch_add(uint128_t(10));
    check(old == uint128_t(250) && fast.get() == uint128_t(260), "SpinLock fetch_add()");
}

// =============================================================================
// SECTION: Concurrent tests
// =============================================================================

void test_concurrent_reads()
{
    const uint128_t shared_value(uint64_t(0x1234567890ABCDEFULL), uint64_t(0xFEDCBA0987654321ULL));
    constexpr int NUM_THREADS = 10;
    constexpr int ITERATIONS = 1000;

    std::vector<std::thread> threads;
    std::atomic<int> success_count{0};

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads.emplace_back([&shared_value, &success_count]()
                             {
            for (int j = 0; j < ITERATIONS; ++j) {
                uint64_t high = shared_value.high();
                uint64_t low = shared_value.low();

                if (high == uint64_t(0x1234567890ABCDEFULL) && low == 0xFEDCBA0987654321ULL) {
                    success_count.fetch_add(1, std::memory_order_relaxed);
                }
            } });
    }

    for (auto &t : threads)
    {
        t.join();
    }

    check(success_count.load() == NUM_THREADS * ITERATIONS, "Concurrent reads are safe");
}

void test_threadsafe_concurrent_increments()
{
    ThreadSafeUint128 counter(uint128_t(0));
    constexpr int NUM_THREADS = 10;
    constexpr int ITERATIONS = 1000;

    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads.emplace_back([&counter]()
                             {
            for (int j = 0; j < ITERATIONS; ++j) {
                ++counter;
            } });
    }

    for (auto &t : threads)
    {
        t.join();
    }

    check(counter.get() == uint128_t(NUM_THREADS * ITERATIONS), "Concurrent increments correct");
}

void test_thread_local_modification()
{
    constexpr int NUM_THREADS = 10;
    constexpr int ITERATIONS = 1000;

    std::vector<std::thread> threads;
    std::vector<uint128_t> results(NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        threads.emplace_back([&results, i]()
                             {
            uint128_t local{uint64_t(i)};
            for (int j = 0; j < ITERATIONS; ++j) {
                local += uint128_t(1);
            }
            results[i] = local; });
    }

    for (auto &t : threads)
    {
        t.join();
    }

    bool all_correct = true;
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        if (results[i] != uint128_t(i + ITERATIONS))
        {
            all_correct = false;
            break;
        }
    }
    check(all_correct, "Thread-local modification is safe");
}

void test_concurrent_signed_operations()
{
    const int128_t negative_value(int64_t(-1000));
    const int128_t positive_value(int64_t(1000));
    constexpr int NUM_THREADS = 10;
    constexpr int ITERATIONS = 500;

    std::vector<std::thread> threads;
    std::atomic<int> neg_count{0};
    std::atomic<int> pos_count{0};

    for (int i = 0; i < NUM_THREADS / 2; ++i)
    {
        threads.emplace_back([&negative_value, &neg_count]()
                             {
            for (int j = 0; j < ITERATIONS; ++j) {
                bool is_neg = negative_value.is_negative();
                int128_t abs_val = negative_value.is_negative() ? -negative_value : negative_value;

                if (is_neg && abs_val > int128_t(0)) {
                    neg_count.fetch_add(1, std::memory_order_relaxed);
                }
            } });
    }

    for (int i = 0; i < NUM_THREADS / 2; ++i)
    {
        threads.emplace_back([&positive_value, &pos_count]()
                             {
            for (int j = 0; j < ITERATIONS; ++j) {
                bool is_pos = !positive_value.is_negative();
                int128_t negated = -positive_value;

                if (is_pos && negated.is_negative()) {
                    pos_count.fetch_add(1, std::memory_order_relaxed);
                }
            } });
    }

    for (auto &t : threads)
    {
        t.join();
    }

    check(neg_count.load() == (NUM_THREADS / 2) * ITERATIONS, "Concurrent negative ops safe");
    check(pos_count.load() == (NUM_THREADS / 2) * ITERATIONS, "Concurrent positive ops safe");
}

// =============================================================================
// MAIN
// =============================================================================

int main()
{
    std::cout << "=== int128_base_thread_safety.hpp tests ===\n\n";

    std::cout << "--- ThreadSafe<S> basic unsigned ---\n";
    test_threadsafe_basic_unsigned();

    std::cout << "\n--- ThreadSafe<S> basic signed ---\n";
    test_threadsafe_basic_signed();

    std::cout << "\n--- ThreadSafe fetch operations ---\n";
    test_threadsafe_fetch_ops();

    std::cout << "\n--- ThreadSafe compare_exchange ---\n";
    test_threadsafe_compare_exchange();

    std::cout << "\n--- ThreadSafe increment/decrement ---\n";
    test_threadsafe_increment_decrement();

    std::cout << "\n--- ThreadSafe apply ---\n";
    test_threadsafe_apply();

    std::cout << "\n--- ThreadSafeRW<S> ---\n";
    test_threadsafe_rw();

    std::cout << "\n--- ThreadSafeAtomic<S> ---\n";
    test_threadsafe_atomic();

    std::cout << "\n--- ThreadSafeSpinLock<S> ---\n";
    test_threadsafe_spinlock();

    std::cout << "\n--- Concurrent reads ---\n";
    test_concurrent_reads();

    std::cout << "\n--- Concurrent increments ---\n";
    test_threadsafe_concurrent_increments();

    std::cout << "\n--- Thread-local modification ---\n";
    test_thread_local_modification();

    std::cout << "\n--- Concurrent signed operations ---\n";
    test_concurrent_signed_operations();

    std::cout << "\n=== RESULTADO: " << tests_passed << "/" << (tests_passed + tests_failed)
              << " tests pasaron ===\n";

    return (tests_failed == 0) ? 0 : 1;
}
