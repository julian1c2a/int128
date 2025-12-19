/**
 * @file int128_thread_safety_benchmarks.cpp
 * @brief Performance benchmarks for thread-safe operations on int128_t
 *
 * Measures overhead of different synchronization strategies for signed 128-bit integers:
 * - Thread-local (baseline, no synchronization)
 * - Mutex-based wrapper
 * - RW-lock wrapper
 * - Spin-lock wrapper
 * - std::atomic wrapper
 *
 * Compile:
 *   g++ -std=c++20 -O2 -pthread -I../include \
 *       benchmarks/int128_thread_safety_benchmarks.cpp -o int128_thread_safety_bench -latomic
 */

#include "../include/int128/int128_t.hpp"
#include <atomic>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#endif

#ifdef __INTEL_COMPILER
#include <ia32intrin.h>
#endif

// Función para leer ciclos de CPU (rdtsc)
inline uint64_t rdtsc()
{
#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
    return __rdtsc();
#elif defined(__x86_64__) || defined(__i386__)
    uint32_t hi, lo;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
#else
    return 0; // Fallback para arquitecturas no soportadas
#endif
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

// Wrapper 3: Spin-Lock
class ThreadSafeInt128SpinLock
{
  private:
    int128_t value_;
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
    ThreadSafeInt128SpinLock(int128_t val = int128_t(0)) : value_(val) {}

    int128_t get() const
    {
        acquire();
        int128_t result = value_;
        release();
        return result;
    }

    void set(int128_t val)
    {
        acquire();
        value_ = val;
        release();
    }

    void add(int128_t val)
    {
        acquire();
        value_ += val;
        release();
    }

    void negate()
    {
        acquire();
        value_ = -value_;
        release();
    }

    bool is_negative() const
    {
        acquire();
        bool result = value_.is_negative();
        release();
        return result;
    }
};

// ========================= BENCHMARK UTILITIES =========================

struct BenchmarkResult {
    std::string name;
    long long duration_us;
    uint64_t total_cycles;
    size_t operations;
    double ops_per_sec;
    double ns_per_op;
    double cycles_per_op;
};

void print_header()
{
    std::cout << std::left << std::setw(35) << "Benchmark" << std::right << std::setw(12)
              << "Time (µs)" << std::setw(15) << "Ops" << std::setw(15) << "Ops/sec"
              << std::setw(12) << "ns/op" << std::setw(14) << "cycles/op" << std::endl;
    std::cout << std::string(103, '-') << std::endl;
}

void print_result(const BenchmarkResult& result)
{
    std::cout << std::left << std::setw(35) << result.name << std::right << std::setw(12)
              << result.duration_us << std::setw(15) << result.operations << std::setw(15)
              << std::fixed << std::setprecision(0) << result.ops_per_sec << std::setw(12)
              << std::setprecision(2) << result.ns_per_op << std::setw(14) << std::setprecision(1)
              << result.cycles_per_op << std::endl;
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
            int128_t local{int64_t(0)};
            for (size_t j = 0; j < ops_per_thread; ++j) {
                local += int128_t(1);
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
    ThreadSafeInt128Mutex counter{int128_t(0)};

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back([&counter, ops_per_thread]() {
            for (size_t j = 0; j < ops_per_thread; ++j) {
                counter.add(int128_t(1));
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

// Benchmark 3: Mutex wrapper with signed operations
BenchmarkResult bench_mutex_signed_ops(size_t num_threads, size_t ops_per_thread)
{
    ThreadSafeInt128Mutex counter{int128_t(int64_t(-1000))};

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back([&counter, ops_per_thread]() {
            for (size_t j = 0; j < ops_per_thread; ++j) {
                counter.add(int128_t(1));
                if (j % 100 == 0) {
                    counter.negate();
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
    return {"Mutex wrapper (with negate)", duration, total_ops, total_ops * 1e6 / duration,
            duration * 1000.0 / total_ops};
}

// Benchmark 4: RW-lock wrapper (write-heavy)
BenchmarkResult bench_rwlock_write_heavy(size_t num_threads, size_t ops_per_thread)
{
    ThreadSafeInt128RW counter{int128_t(0)};

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back([&counter, ops_per_thread]() {
            for (size_t j = 0; j < ops_per_thread; ++j) {
                counter.add(int128_t(1)); // Write operation
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

// Benchmark 5: RW-lock wrapper (read-heavy)
BenchmarkResult bench_rwlock_read_heavy(size_t num_threads, size_t ops_per_thread)
{
    ThreadSafeInt128RW counter{int128_t(int64_t(12345))};

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    // 90% readers, 10% writers
    size_t num_readers = num_threads * 9 / 10;
    size_t num_writers = num_threads - num_readers;

    for (size_t i = 0; i < num_readers; ++i) {
        threads.emplace_back([&counter, ops_per_thread]() {
            for (size_t j = 0; j < ops_per_thread; ++j) {
                volatile auto val = counter.get();         // Read operation
                volatile auto neg = counter.is_negative(); // Signed read
                (void)val;
                (void)neg;
            }
        });
    }

    for (size_t i = 0; i < num_writers; ++i) {
        threads.emplace_back([&counter, ops_per_thread]() {
            for (size_t j = 0; j < ops_per_thread; ++j) {
                counter.add(int128_t(1)); // Write operation
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

// Benchmark 6: Spin-lock wrapper
BenchmarkResult bench_spinlock_wrapper(size_t num_threads, size_t ops_per_thread)
{
    ThreadSafeInt128SpinLock counter{int128_t(0)};

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back([&counter, ops_per_thread]() {
            for (size_t j = 0; j < ops_per_thread; ++j) {
                counter.add(int128_t(1));
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

// Benchmark 7: std::atomic (if lock-free)
BenchmarkResult bench_atomic_wrapper(size_t num_threads, size_t ops_per_thread)
{
    std::atomic<int128_t> counter{int128_t(0)};

    if (!counter.is_lock_free()) {
        return {"std::atomic (NOT lock-free)", 0, 0, 0, 0};
    }

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back([&counter, ops_per_thread]() {
            for (size_t j = 0; j < ops_per_thread; ++j) {
                int128_t current = counter.load();
                while (!counter.compare_exchange_weak(current, current + int128_t(1))) {
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

// Benchmark 8: Read-only concurrent access (const operations)
BenchmarkResult bench_concurrent_reads(size_t num_threads, size_t ops_per_thread)
{
    const int128_t shared_value{int64_t(0x1234567890ABCDEFLL), 0xFEDCBA0987654321ULL};

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back([&shared_value, ops_per_thread]() {
            for (size_t j = 0; j < ops_per_thread; ++j) {
                volatile uint64_t high = shared_value.high();
                volatile uint64_t low = shared_value.low();
                volatile bool neg = shared_value.is_negative();
                (void)high;
                (void)low;
                (void)neg;
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

// Benchmark 9: Signed-specific operations
BenchmarkResult bench_signed_operations(size_t num_threads, size_t ops_per_thread)
{
    ThreadSafeInt128Mutex counter{int128_t(int64_t(-1000))};

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back([&counter, ops_per_thread]() {
            for (size_t j = 0; j < ops_per_thread; ++j) {
                counter.negate(); // Flip sign
                volatile bool neg = counter.is_negative();
                (void)neg;
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    size_t total_ops = num_threads * ops_per_thread;
    return {"Signed ops (negate+check)", duration, total_ops, total_ops * 1e6 / duration,
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
    std::cout << "║  Thread Safety Benchmarks for int128_t (signed)           ║" << std::endl;
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
    results.push_back(bench_mutex_signed_ops(NUM_THREADS, OPS_PER_THREAD));
    results.push_back(bench_spinlock_wrapper(NUM_THREADS, OPS_PER_THREAD));
    results.push_back(bench_rwlock_write_heavy(NUM_THREADS, OPS_PER_THREAD));
    results.push_back(bench_rwlock_read_heavy(NUM_THREADS, OPS_PER_THREAD));
    results.push_back(bench_signed_operations(NUM_THREADS, OPS_PER_THREAD));

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
    std::cout << "  • Signed operations (negate, is_negative) have minimal overhead" << std::endl;

    return 0;
}
