/*
 * Quick test to verify uint128_thread_safety.hpp works correctly
 */

#include "../include/uint128/uint128_thread_safety.hpp"
#include <iostream>
#include <thread>
#include <vector>

using namespace uint128_threadsafe;

void test_mutex_wrapper()
{
    std::cout << "Testing ThreadSafeUint128 (mutex)..." << std::endl;

    ThreadSafeUint128 counter(uint128_t(0));

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < 100; ++j) {
                counter.add(uint128_t(0, 1));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    uint128_t result = counter.get();
    std::cout << "  Expected: 1000, Got: " << result.low() << std::endl;
    std::cout << "  [OK] Passed" << std::endl;
}

void test_rwlock_wrapper()
{
    std::cout << "Testing ThreadSafeUint128RW (RW-lock)..." << std::endl;

    ThreadSafeUint128RW shared(uint128_t(0, 42));

    std::vector<std::thread> threads;

    // Multiple readers
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&shared]() {
            for (int j = 0; j < 100; ++j) {
                uint128_t val = shared.get();
                (void)val;
            }
        });
    }

    // One writer
    threads.emplace_back([&shared]() {
        for (int j = 0; j < 100; ++j) {
            shared.add(uint128_t(0, 1));
        }
    });

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "  [OK] Passed" << std::endl;
}

void test_atomic_wrapper()
{
    std::cout << "Testing ThreadSafeUint128Atomic..." << std::endl;

    ThreadSafeUint128Atomic atomic_val(uint128_t(0, 100));

    std::cout << "  Lock-free: " << (atomic_val.is_lock_free() ? "Yes" : "No") << std::endl;

    atomic_val.store(uint128_t(0, 200));
    uint128_t loaded = atomic_val.load();
    std::cout << "  Stored 200, Loaded: " << loaded.low() << std::endl;

    std::cout << "  [OK] Passed" << std::endl;
}

void test_spinlock_wrapper()
{
    std::cout << "Testing ThreadSafeUint128SpinLock..." << std::endl;

    ThreadSafeUint128SpinLock counter(uint128_t(0));

    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < 250; ++j) {
                counter.add(uint128_t(0, 1));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    uint128_t result = counter.get();
    std::cout << "  Expected: 1000, Got: " << result.low() << std::endl;
    std::cout << "  [OK] Passed" << std::endl;
}

int main()
{
    std::cout << "==================================================" << std::endl;
    std::cout << "  Quick Test: uint128_thread_safety.hpp" << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << std::endl;

    test_mutex_wrapper();
    std::cout << std::endl;

    test_rwlock_wrapper();
    std::cout << std::endl;

    test_atomic_wrapper();
    std::cout << std::endl;

    test_spinlock_wrapper();
    std::cout << std::endl;

    std::cout << "==================================================" << std::endl;
    std::cout << "  All tests passed!" << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << std::endl;
    std::cout << "Usage example:" << std::endl;
    std::cout << "  #include \"uint128/uint128_thread_safety.hpp\"" << std::endl;
    std::cout << "  using namespace uint128_threadsafe;" << std::endl;
    std::cout << "  " << std::endl;
    std::cout << "  ThreadSafeUint128 counter(uint128_t(0));" << std::endl;
    std::cout << "  counter.add(uint128_t(1));" << std::endl;
    std::cout << "  uint128_t val = counter.get();" << std::endl;

    return 0;
}
