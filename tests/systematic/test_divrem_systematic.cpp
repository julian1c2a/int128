/**
 * @file test_divrem_systematic.cpp
 * @brief Tests sistemáticos de divrem para int128_base_t
 *
 * Organización de casos de prueba:
 * 1. División por 0 (nullopt esperado)
 * 2. División por 1 (identidad)
 * 3. División por potencias de 2 (2^1 a 2^127)
 * 4. Divisor que cabe en tipos builtin (int8_t, uint8_t, ..., uint64_t)
 * 5. Dividendo y divisor que caben en low (64 bits)
 * 6. Divisor con N bits activos (popcount = N, distribución aleatoria)
 * 7. Propiedad euclidiana: dividend == quotient * divisor + remainder
 * 8. Propiedad del resto: remainder < divisor (cuando divisor > 0)
 *
 * @author Julián Calderón Almendros
 * @date 2026-01-07
 */

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <optional>
#include <random>
#include <string>
#include <vector>

#include "../include_new/int128_base_tt.hpp"

using namespace nstd;
using uint128_t = int128_base_t<signedness::unsigned_type>;
using int128_t = int128_base_t<signedness::signed_type>;

// ============================================================================
// Configuración de tests
// ============================================================================
static constexpr size_t INSTANCES_PER_CATEGORY = 100;
static constexpr size_t BITS_ACTIVE_MIN = 2;
static constexpr size_t BITS_ACTIVE_MAX = 125;

// ============================================================================
// Generadores de valores aleatorios
// ============================================================================

class RandomGenerator
{
    std::mt19937_64 rng;

  public:
    explicit RandomGenerator(uint64_t seed = 42) : rng(seed) {}

    uint64_t next_u64()
    {
        return rng();
    }

    uint128_t next_u128()
    {
        return uint128_t(rng(), rng());
    }

    // Valor que cabe en low (high == 0)
    uint128_t next_fits_in_low()
    {
        return uint128_t(0, rng());
    }

    // Valor que cabe en N bits
    template <typename T> T next_fits_in()
    {
        static_assert(std::is_integral_v<T>, "T must be integral");
        if constexpr (std::is_same_v<T, uint64_t>) {
            return rng();
        } else if constexpr (std::is_same_v<T, int64_t>) {
            return static_cast<int64_t>(rng() >> 1); // Positive
        } else if constexpr (sizeof(T) < 8) {
            return static_cast<T>(rng() % (static_cast<uint64_t>(1) << (sizeof(T) * 8)));
        } else {
            return static_cast<T>(rng());
        }
    }

    // Valor con exactamente N bits activos (popcount == N)
    uint128_t next_with_popcount(size_t n)
    {
        if (n == 0)
            return uint128_t(0);
        if (n >= 128)
            return uint128_t(~0ULL, ~0ULL);

        uint128_t result(0);
        std::vector<int> positions;
        for (int i = 0; i < 128; ++i)
            positions.push_back(i);

        // Shuffle y seleccionar primeros N
        for (size_t i = 0; i < n && i < positions.size(); ++i) {
            size_t j = i + (rng() % (positions.size() - i));
            std::swap(positions[i], positions[j]);
        }

        for (size_t i = 0; i < n; ++i) {
            int bit = positions[i];
            if (bit < 64) {
                uint64_t low = result.low();
                low |= (1ULL << bit);
                result = uint128_t(result.high(), low);
            } else {
                uint64_t high = result.high();
                high |= (1ULL << (bit - 64));
                result = uint128_t(high, result.low());
            }
        }
        return result;
    }

    // Potencia de 2: 2^n
    static uint128_t power_of_2(int n)
    {
        if (n < 0 || n > 127)
            return uint128_t(0);
        if (n < 64) {
            return uint128_t(0, 1ULL << n);
        } else {
            return uint128_t(1ULL << (n - 64), 0);
        }
    }
};

// ============================================================================
// Verificación de propiedades
// ============================================================================

struct TestResult {
    bool passed;
    std::string message;
    size_t tests_run;
    size_t tests_passed;

    TestResult() : passed(true), message(""), tests_run(0), tests_passed(0) {}

    void fail(const std::string& msg)
    {
        passed = false;
        message = msg;
    }

    void check(bool condition, const std::string& msg)
    {
        tests_run++;
        if (condition) {
            tests_passed++;
        } else {
            fail(msg);
        }
    }
};

// Verificar propiedad euclidiana: dividend == quotient * divisor + remainder
bool verify_euclidean(const uint128_t& dividend, const uint128_t& divisor,
                      const uint128_t& quotient, const uint128_t& remainder)
{
    uint128_t reconstructed = quotient * divisor + remainder;
    return reconstructed == dividend;
}

// Verificar que remainder < divisor
bool verify_remainder_bound(const uint128_t& remainder, const uint128_t& divisor)
{
    if (divisor == uint128_t(0))
        return true; // N/A para divisor 0
    return remainder < divisor;
}

// ============================================================================
// Tests de división
// ============================================================================

TestResult test_division_by_zero()
{
    TestResult result;
    RandomGenerator rng;

    std::cout << "  Test: Division by 0...\n";

    for (size_t i = 0; i < INSTANCES_PER_CATEGORY; ++i) {
        uint128_t dividend = rng.next_u128();
        auto divrem_result = dividend.divrem(uint128_t(0));

        result.check(!divrem_result.has_value(), "Division by 0 should return nullopt");
    }

    return result;
}

TestResult test_division_by_one()
{
    TestResult result;
    RandomGenerator rng;

    std::cout << "  Test: Division by 1...\n";

    for (size_t i = 0; i < INSTANCES_PER_CATEGORY; ++i) {
        uint128_t dividend = rng.next_u128();
        auto divrem_result = dividend.divrem(uint128_t(1));

        result.check(divrem_result.has_value(), "Division by 1 should succeed");
        if (divrem_result.has_value()) {
            auto [q, r] = *divrem_result;
            result.check(q == dividend, "Quotient should equal dividend");
            result.check(r == uint128_t(0), "Remainder should be 0");
            result.check(verify_euclidean(dividend, uint128_t(1), q, r),
                         "Euclidean property violated");
        }
    }

    return result;
}

TestResult test_division_by_powers_of_2()
{
    TestResult result;
    RandomGenerator rng;

    std::cout << "  Test: Division by powers of 2 (2^1 to 2^127)...\n";

    for (int power = 1; power <= 127; ++power) {
        uint128_t divisor = RandomGenerator::power_of_2(power);

        for (size_t i = 0; i < 10; ++i) { // 10 instances per power
            uint128_t dividend = rng.next_u128();
            auto divrem_result = dividend.divrem(divisor);

            result.check(divrem_result.has_value(),
                         "Division by 2^" + std::to_string(power) + " should succeed");

            if (divrem_result.has_value()) {
                auto [q, r] = *divrem_result;
                result.check(verify_euclidean(dividend, divisor, q, r),
                             "Euclidean property violated for 2^" + std::to_string(power));
                result.check(verify_remainder_bound(r, divisor),
                             "Remainder >= divisor for 2^" + std::to_string(power));

                // Verificar que es equivalente a shift right
                uint128_t expected_q = dividend >> power;
                result.check(q == expected_q, "Division by 2^n should equal right shift by n");
            }
        }
    }

    return result;
}

template <typename T> TestResult test_division_by_builtin_type(const std::string& type_name)
{
    TestResult result;
    RandomGenerator rng;

    std::cout << "  Test: Division by " << type_name << "...\n";

    for (size_t i = 0; i < INSTANCES_PER_CATEGORY; ++i) {
        uint128_t dividend = rng.next_u128();
        T divisor_val = rng.template next_fits_in<T>();

        // Evitar división por 0
        while (divisor_val == 0) {
            divisor_val = rng.template next_fits_in<T>();
        }

        uint128_t divisor(static_cast<uint64_t>(divisor_val));
        auto divrem_result = dividend.divrem(divisor);

        result.check(divrem_result.has_value(), "Division by " + type_name + " should succeed");

        if (divrem_result.has_value()) {
            auto [q, r] = *divrem_result;
            result.check(verify_euclidean(dividend, divisor, q, r),
                         "Euclidean property violated for " + type_name);
            result.check(verify_remainder_bound(r, divisor),
                         "Remainder >= divisor for " + type_name);
        }
    }

    return result;
}

TestResult test_division_fits_in_low()
{
    TestResult result;
    RandomGenerator rng;

    std::cout << "  Test: Division where both fit in low (64 bits)...\n";

    for (size_t i = 0; i < INSTANCES_PER_CATEGORY; ++i) {
        uint128_t dividend = rng.next_fits_in_low();
        uint128_t divisor = rng.next_fits_in_low();

        // Evitar división por 0
        while (divisor == uint128_t(0)) {
            divisor = rng.next_fits_in_low();
        }

        auto divrem_result = dividend.divrem(divisor);

        result.check(divrem_result.has_value(), "Division fits_in_low should succeed");

        if (divrem_result.has_value()) {
            auto [q, r] = *divrem_result;
            result.check(verify_euclidean(dividend, divisor, q, r),
                         "Euclidean property violated for fits_in_low");
            result.check(verify_remainder_bound(r, divisor),
                         "Remainder >= divisor for fits_in_low");

            // Verificar resultado contra operaciones nativas uint64_t
            uint64_t native_q = dividend.low() / divisor.low();
            uint64_t native_r = dividend.low() % divisor.low();
            result.check(q.low() == native_q && q.high() == 0,
                         "Quotient mismatch with native uint64_t");
            result.check(r.low() == native_r && r.high() == 0,
                         "Remainder mismatch with native uint64_t");
        }
    }

    return result;
}

TestResult test_division_by_popcount_n(size_t popcount)
{
    TestResult result;
    RandomGenerator rng(42 + popcount); // Diferente seed por popcount

    for (size_t i = 0; i < 20; ++i) { // 20 instances per popcount
        uint128_t dividend = rng.next_u128();
        uint128_t divisor = rng.next_with_popcount(popcount);

        // Evitar división por 0
        while (divisor == uint128_t(0)) {
            divisor = rng.next_with_popcount(popcount);
        }

        auto divrem_result = dividend.divrem(divisor);

        result.check(divrem_result.has_value(),
                     "Division by popcount=" + std::to_string(popcount) + " should succeed");

        if (divrem_result.has_value()) {
            auto [q, r] = *divrem_result;
            result.check(verify_euclidean(dividend, divisor, q, r),
                         "Euclidean property violated for popcount=" + std::to_string(popcount));
            result.check(verify_remainder_bound(r, divisor),
                         "Remainder >= divisor for popcount=" + std::to_string(popcount));
        }
    }

    return result;
}

TestResult test_division_by_popcount_range()
{
    TestResult result;

    std::cout << "  Test: Division by divisors with N bits set (popcount)...\n";

    for (size_t pop = BITS_ACTIVE_MIN; pop <= BITS_ACTIVE_MAX; pop += 5) {
        TestResult sub_result = test_division_by_popcount_n(pop);
        result.tests_run += sub_result.tests_run;
        result.tests_passed += sub_result.tests_passed;
        if (!sub_result.passed) {
            result.passed = false;
            result.message += sub_result.message + "; ";
        }
    }

    return result;
}

TestResult test_division_special_cases()
{
    TestResult result;

    std::cout << "  Test: Special division cases...\n";

    // Dividend < Divisor
    {
        uint128_t dividend(0, 100);
        uint128_t divisor(0, 200);
        auto divrem_result = dividend.divrem(divisor);
        result.check(divrem_result.has_value(), "Small/Large should succeed");
        if (divrem_result.has_value()) {
            auto [q, r] = *divrem_result;
            result.check(q == uint128_t(0), "Quotient should be 0");
            result.check(r == dividend, "Remainder should equal dividend");
        }
    }

    // Dividend == Divisor
    {
        uint128_t value(0x12345678, 0x9ABCDEF0);
        auto divrem_result = value.divrem(value);
        result.check(divrem_result.has_value(), "Same/Same should succeed");
        if (divrem_result.has_value()) {
            auto [q, r] = *divrem_result;
            result.check(q == uint128_t(1), "Quotient should be 1");
            result.check(r == uint128_t(0), "Remainder should be 0");
        }
    }

    // MAX / 1
    {
        uint128_t max_val(~0ULL, ~0ULL);
        auto divrem_result = max_val.divrem(uint128_t(1));
        result.check(divrem_result.has_value(), "MAX/1 should succeed");
        if (divrem_result.has_value()) {
            auto [q, r] = *divrem_result;
            result.check(q == max_val, "Quotient should be MAX");
            result.check(r == uint128_t(0), "Remainder should be 0");
        }
    }

    // MAX / MAX
    {
        uint128_t max_val(~0ULL, ~0ULL);
        auto divrem_result = max_val.divrem(max_val);
        result.check(divrem_result.has_value(), "MAX/MAX should succeed");
        if (divrem_result.has_value()) {
            auto [q, r] = *divrem_result;
            result.check(q == uint128_t(1), "Quotient should be 1");
            result.check(r == uint128_t(0), "Remainder should be 0");
        }
    }

    // MAX / 2
    {
        uint128_t max_val(~0ULL, ~0ULL);
        auto divrem_result = max_val.divrem(uint128_t(2));
        result.check(divrem_result.has_value(), "MAX/2 should succeed");
        if (divrem_result.has_value()) {
            auto [q, r] = *divrem_result;
            uint128_t expected_q = max_val >> 1;
            result.check(q == expected_q, "MAX/2 quotient incorrect");
            result.check(r == uint128_t(1), "MAX/2 remainder should be 1");
        }
    }

    return result;
}

// ============================================================================
// Main
// ============================================================================

int main()
{
    std::cout << "============================================================\n";
    std::cout << "       SYSTEMATIC DIVREM TESTS\n";
    std::cout << "============================================================\n\n";

    size_t total_tests = 0;
    size_t total_passed = 0;

    auto run_test = [&](const std::string& name, TestResult (*test_fn)()) {
        std::cout << "[TEST] " << name << "\n";
        TestResult result = test_fn();
        total_tests += result.tests_run;
        total_passed += result.tests_passed;

        std::cout << "       Passed: " << result.tests_passed << "/" << result.tests_run;
        if (result.passed) {
            std::cout << " [OK]\n\n";
        } else {
            std::cout << " [FAIL]\n";
            std::cout << "       Error: " << result.message << "\n\n";
        }
        return result.passed;
    };

    bool all_passed = true;

    all_passed &= run_test("Division by 0", test_division_by_zero);
    all_passed &= run_test("Division by 1", test_division_by_one);
    all_passed &= run_test("Division by powers of 2", test_division_by_powers_of_2);
    all_passed &= run_test("Division by uint8_t", test_division_by_builtin_type<uint8_t>);
    all_passed &= run_test("Division by uint16_t", test_division_by_builtin_type<uint16_t>);
    all_passed &= run_test("Division by uint32_t", test_division_by_builtin_type<uint32_t>);
    all_passed &= run_test("Division by uint64_t", test_division_by_builtin_type<uint64_t>);
    all_passed &= run_test("Division by int8_t", test_division_by_builtin_type<int8_t>);
    all_passed &= run_test("Division by int16_t", test_division_by_builtin_type<int16_t>);
    all_passed &= run_test("Division by int32_t", test_division_by_builtin_type<int32_t>);
    all_passed &= run_test("Division by int64_t", test_division_by_builtin_type<int64_t>);
    all_passed &= run_test("Division fits in low", test_division_fits_in_low);
    all_passed &= run_test("Division by popcount N", test_division_by_popcount_range);
    all_passed &= run_test("Special cases", test_division_special_cases);

    std::cout << "============================================================\n";
    std::cout << "TOTAL: " << total_passed << "/" << total_tests << " tests passed\n";
    std::cout << "============================================================\n";

    if (all_passed) {
        std::cout << "\n*** ALL DIVREM TESTS PASSED ***\n";
        return 0;
    } else {
        std::cout << "\n*** SOME TESTS FAILED ***\n";
        return 1;
    }
}
