/**
 * @file uint128_thread_safety_benchmarks.cpp
 * @brief Performance benchmarks for thread-safe operations on uint128_t
 *
 * Measures overhead of different synchronization strategies:
 * - Thread-local (baseline, no synchronization)
 * - Mutex-based wrapper
 * - RW-lock wrapper
 * - Spin-lock wrapper
 * - std::atomic wrapper
 *
 * Compile:
 *   g++ -std=c++20 -O2 -pthread -I../include \
 *       benchmarks/uint128_thread_safety_benchmarks.cpp -o uint128_thread_safety_bench -latomic
 */

#include "../include/uint128/uint128_t.hpp"
#include <atomic>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

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

// Wrapper 3: Spin-Lock
class ThreadSafeUint128SpinLock
{
  private:
    uint128_t value_;
    mutable std::atomic_flag lock_ = ATOMIC_FLAG_INIT;

    void acquire() const
    {
        while (lock_.test_and_set(std::memory_order_acquire)) {
#if defined(__x86_64__) || defined(_M_X64)
            __builtin_ia32_pause();
#endif
        }
    }

    void release() const
    {
        lock_.clear(std::memory_order_release);
    }

  public:
    ThreadSafeUint128SpinLock(uint128_t val = uint128_t(0)) : value_(val) {}

    uint128_t get() const
    {
        acquire();
        uint128_t result = value_;
        release();
        return result;
    }

    void set(uint128_t val)
    {
        acquire();
        value_ = val;
        release();
    }

    void add(uint128_t val)
    {
        acquire();
        value_ += val;
        release();
    }
};

// ========================= BENCHMARK UTILITIES =========================

struct BenchmarkResult {
    std::string name;
    long long duration_us;
    size_t operations;
    double ops_per_sec;
    double ns_per_op;
};

void print_header()
{
    std::cout << std::left << std::setw(35) << "Benchmark" << std::right << std::setw(12)
              << "Time (µs)" << std::setw(15) << "Ops" << std::setw(15) << "Ops/sec"
              << std::setw(12) << "ns/op" << std::endl;
    std::cout << std::string(89, '-') << std::endl;
}

void print_result(const BenchmarkResult& result)
{
    std::cout << std::left << std::setw(35) << result.name << std::right << std::setw(12)
              << result.duration_us << std::setw(15) << result.operations << std::setw(15)
              << std::fixed << std::setprecision(0) << result.ops_per_sec << std::setw(12)
              << std::setprecision(2) << result.ns_per_op << std::endl;
}

// ========================= BENCHMARKS =========================

// Benchmark 1: Thread-local (no synchronization) - BASELINE
BenchmarkResult bench_thread_local(size_t num_threads, size_t ops_per_thread)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back([ops_per_thread]() {
            uint128_t local(0);
            for (size_t j = 0; j < ops_per_thread; ++j) {
                local += uint128_t(0, 1);
            }
            // Prevent optimization
            volatile uint64_t sink = local.low();
            (void)sink;
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    size_t total_ops = num_threads * ops_per_thread;
    return {"Thread-local (baseline)", duration, total_ops, total_ops * 1e6 / duration,
            duration * 1000.0 / total_ops};
}

// Benchmark 2: Mutex wrapper
BenchmarkResult bench_mutex_wrapper(size_t num_threads, size_t ops_per_thread)
{
    ThreadSafeUint128Mutex counter(uint128_t(0));

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back([&counter, ops_per_thread]() {
            for (size_t j = 0; j < ops_per_thread; ++j) {
                counter.add(uint128_t(0, 1));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    size_t total_ops = num_threads * ops_per_thread;
    return {"Mutex wrapper", duration, total_ops, total_ops * 1e6 / duration,
            duration * 1000.0 / total_ops};
}

// Benchmark 3: RW-lock wrapper (write-heavy)
BenchmarkResult bench_rwlock_write_heavy(size_t num_threads, size_t ops_per_thread)
{
    ThreadSafeUint128RW counter(uint128_t(0));

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back([&counter, ops_per_thread]() {
            for (size_t j = 0; j < ops_per_thread; ++j) {
                counter.add(uint128_t(0, 1)); // Write operation
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    size_t total_ops = num_threads * ops_per_thread;
    return {"RW-lock (write-heavy)", duration, total_ops, total_ops * 1e6 / duration,
            duration * 1000.0 / total_ops};
}

// Benchmark 4: RW-lock wrapper (read-heavy)
BenchmarkResult bench_rwlock_read_heavy(size_t num_threads, size_t ops_per_thread)
{
    ThreadSafeUint128RW counter(uint128_t(0, 12345));

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    // 90% readers, 10% writers
    size_t num_readers = num_threads * 9 / 10;
    size_t num_writers = num_threads - num_readers;

    for (size_t i = 0; i < num_readers; ++i) {
        threads.emplace_back([&counter, ops_per_thread]() {
            for (size_t j = 0; j < ops_per_thread; ++j) {
                volatile auto val = counter.get(); // Read operation
                (void)val;
            }
        });
    }

    for (size_t i = 0; i < num_writers; ++i) {
        threads.emplace_back([&counter, ops_per_thread]() {
            for (size_t j = 0; j < ops_per_thread; ++j) {
                counter.add(uint128_t(0, 1)); // Write operation
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    size_t total_ops = num_threads * ops_per_thread;
    return {"RW-lock (read-heavy 90%)", duration, total_ops, total_ops * 1e6 / duration,
            duration * 1000.0 / total_ops};
}

// Benchmark 5: Spin-lock wrapper
BenchmarkResult bench_spinlock_wrapper(size_t num_threads, size_t ops_per_thread)
{
    ThreadSafeUint128SpinLock counter(uint128_t(0));

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back([&counter, ops_per_thread]() {
            for (size_t j = 0; j < ops_per_thread; ++j) {
                counter.add(uint128_t(0, 1));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    size_t total_ops = num_threads * ops_per_thread;
    return {"Spin-lock wrapper", duration, total_ops, total_ops * 1e6 / duration,
            duration * 1000.0 / total_ops};
}

// Benchmark 6: std::atomic (if lock-free)
BenchmarkResult bench_atomic_wrapper(size_t num_threads, size_t ops_per_thread)
{
    std::atomic<uint128_t> counter(uint128_t(0));

    if (!counter.is_lock_free()) {
        return {"std::atomic (NOT lock-free)", 0, 0, 0, 0};
    }

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back([&counter, ops_per_thread]() {
            for (size_t j = 0; j < ops_per_thread; ++j) {
                uint128_t current = counter.load();
                while (!counter.compare_exchange_weak(current, current + uint128_t(0, 1))) {
                    // Retry
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    size_t total_ops = num_threads * ops_per_thread;
    return {"std::atomic (lock-free)", duration, total_ops, total_ops * 1e6 / duration,
            duration * 1000.0 / total_ops};
}

// Benchmark 7: Read-only concurrent access (const operations)
BenchmarkResult bench_concurrent_reads(size_t num_threads, size_t ops_per_thread)
{
    const uint128_t shared_value(0x1234567890ABCDEFULL, 0xFEDCBA0987654321ULL);

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back([&shared_value, ops_per_thread]() {
            for (size_t j = 0; j < ops_per_thread; ++j) {
                volatile uint64_t high = shared_value.high();
                volatile uint64_t low = shared_value.low();
                (void)high;
                (void)low;
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    size_t total_ops = num_threads * ops_per_thread;
    return {"Concurrent reads (const)", duration, total_ops, total_ops * 1e6 / duration,
            duration * 1000.0 / total_ops};
}

// ========================= CONTENTION ANALYSIS =========================

void run_contention_analysis()
{
    std::cout << "\n=== Contention Analysis (varying thread count) ===" << std::endl;
    std::cout << "\nOperations: 100,000 per thread\n" << std::endl;

    std::vector<size_t> thread_counts = {1, 2, 4, 8, 16};
    const size_t ops_per_thread = 100000;

    std::cout << std::left << std::setw(20) << "Threads" << std::setw(18) << "Mutex (µs)"
              << std::setw(18) << "SpinLock (µs)" << std::setw(18) << "RW-lock (µs)"
              << std::setw(18) << "Thread-local (µs)" << std::endl;
    std::cout << std::string(92, '-') << std::endl;

    for (size_t num_threads : thread_counts) {
        auto mutex_result = bench_mutex_wrapper(num_threads, ops_per_thread);
        auto spinlock_result = bench_spinlock_wrapper(num_threads, ops_per_thread);
        auto rwlock_result = bench_rwlock_write_heavy(num_threads, ops_per_thread);
        auto local_result = bench_thread_local(num_threads, ops_per_thread);

        std::cout << std::left << std::setw(20) << num_threads << std::setw(18)
                  << mutex_result.duration_us << std::setw(18) << spinlock_result.duration_us
                  << std::setw(18) << rwlock_result.duration_us << std::setw(18)
                  << local_result.duration_us << std::endl;
    }
}

// ========================= MAIN =========================

int main()
{
    std::cout << "╔═════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  Thread Safety Benchmarks for uint128_t                   ║" << std::endl;
    std::cout << "╚═════════════════════════════════════════════════════════════╝" << std::endl;

    const size_t NUM_THREADS = 4;
    const size_t OPS_PER_THREAD = 250000;

    std::cout << "\nConfiguration:" << std::endl;
    std::cout << "  Threads: " << NUM_THREADS << std::endl;
    std::cout << "  Operations per thread: " << OPS_PER_THREAD << std::endl;
    std::cout << "  Total operations: " << NUM_THREADS * OPS_PER_THREAD << std::endl;
    std::cout << std::endl;

    print_header();

    // Run benchmarks
    std::vector<BenchmarkResult> results;

    results.push_back(bench_thread_local(NUM_THREADS, OPS_PER_THREAD));
    results.push_back(bench_concurrent_reads(NUM_THREADS, OPS_PER_THREAD));
    results.push_back(bench_mutex_wrapper(NUM_THREADS, OPS_PER_THREAD));
    results.push_back(bench_spinlock_wrapper(NUM_THREADS, OPS_PER_THREAD));
    results.push_back(bench_rwlock_write_heavy(NUM_THREADS, OPS_PER_THREAD));
    results.push_back(bench_rwlock_read_heavy(NUM_THREADS, OPS_PER_THREAD));

    auto atomic_result = bench_atomic_wrapper(NUM_THREADS, OPS_PER_THREAD);
    if (atomic_result.duration_us > 0) {
        results.push_back(atomic_result);
    }

    for (const auto& result : results) {
        print_result(result);
    }

    if (atomic_result.duration_us == 0) {
        std::cout << std::left << std::setw(35) << "std::atomic (NOT lock-free)" << std::right
                  << std::setw(12) << "N/A" << std::endl;
    }

    // Contention analysis
    run_contention_analysis();

    // Summary
    std::cout << "\n=== Summary ===" << std::endl;
    std::cout << "Fastest (excluding thread-local): ";
    auto fastest = std::min_element(results.begin() + 1, results.end(),
                                    [](const BenchmarkResult& a, const BenchmarkResult& b) {
                                        return a.ns_per_op < b.ns_per_op;
                                    });
    std::cout << fastest->name << " (" << fastest->ns_per_op << " ns/op)" << std::endl;

    std::cout << "\nRecommendations:" << std::endl;
    std::cout << "  • Use thread-local for best performance (no synchronization)" << std::endl;
    std::cout << "  • Use Mutex wrapper for general-purpose shared access" << std::endl;
    std::cout << "  • Use RW-lock for read-heavy workloads (>90% reads)" << std::endl;
    std::cout << "  • Use Spin-lock for low-contention scenarios" << std::endl;

    return 0;
}
