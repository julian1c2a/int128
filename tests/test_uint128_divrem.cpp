#include "../include/uint128/uint128_t.hpp"
#include <cassert>
#include <iostream>
#include <random>

using namespace uint128_literals;
void test_divrem_basic()
{
    std::cout << "test_divrem: test_divrem_basic ......" << std::endl;
    uint128_t a = 100_u128;
    uint128_t b = 7_u128;

    auto res = a.divrem(b);
    assert(res.has_value());
    assert(res->first == 14_u128);
    assert(res->second == 2_u128);
    std::cout << "test_divrem: test_divrem_basic passed" << std::endl;
}

void test_divrem_zero()
{
    std::cout << "test_divrem: test_divrem_zero ......" << std::endl;
    uint128_t a = 100_u128;
    auto res = a.divrem(0_u128);
    assert(!res.has_value());
    std::cout << "test_divrem: test_divrem_zero passed" << std::endl;
}

void test_divrem_large()
{
    std::cout << "test_divrem: test_divrem_large ......" << std::endl;
    // 2^100
    uint128_t large = 1_u128 << 100;
    uint128_t divisor = 3_u128;

    auto res = large.divrem(divisor);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res->first * divisor + res->second == large);
    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res->second < divisor);

    std::cout << "test_divrem: test_divrem_large passed" << std::endl;
}

void test_divrem_zero_integral_divisor()
{
    std::cout << "test_divrem: test_divrem_zero_integral_divisor ......" << std::endl;
    uint128_t a = 100_u128;
    auto res_u64 = a.divrem(static_cast<uint64_t>(0U));
    assert(!res_u64.has_value());
    auto res_i64 = a.divrem(static_cast<int64_t>(0));
    assert(!res_i64.has_value());
    auto res_u32 = a.divrem(static_cast<uint32_t>(0U));
    assert(!res_u32.has_value());
    auto res_i32 = a.divrem(static_cast<int32_t>(0));
    assert(!res_i32.has_value());
    auto res_u16 = a.divrem(static_cast<uint16_t>(0U));
    assert(!res_u16.has_value());
    auto res_i16 = a.divrem(static_cast<int16_t>(0));
    assert(!res_i16.has_value());
    auto res_u8 = a.divrem(static_cast<uint8_t>(0U));
    assert(!res_u8.has_value());
    auto res_i8 = a.divrem(static_cast<int8_t>(0));
    assert(!res_i8.has_value());
    std::cout << "test_divrem: test_divrem_zero_integral_divisor passed" << std::endl;
}

void test_divrem_basic_integral_divisor()
{
    std::cout << "test_divrem: test_divrem_basic_integral_divisor ......" << std::endl;
    uint128_t a = 100_u128;
    uint64_t b_u64 = 7ULL;
    auto res_u64 = a.divrem(b_u64);
    assert(res_u64.has_value());
    assert(res_u64->first == 14_u128);
    assert(res_u64->second == 2_u128);
    int64_t b_i64 = 7LL;
    auto res_i64 = a.divrem(b_i64);
    assert(res_i64.has_value());
    assert(res_i64->first == 14_u128);
    assert(res_i64->second == 2_u128);
    uint32_t b_u32 = 7U;
    auto res_u32 = a.divrem(b_u32);
    assert(res_u32.has_value());
    assert(res_u32->first == 14_u128);
    assert(res_u32->second == 2_u128);
    int32_t b_i32 = 7L;
    auto res_i32 = a.divrem(b_i32);
    assert(res_i32.has_value());
    assert(res_i32->first == 14_u128);
    assert(res_i32->second == 2_u128);
    uint16_t b_u16 = 7u;
    auto res_u16 = a.divrem(b_u16);
    assert(res_u16.has_value());
    assert(res_u16->first == 14_u128);
    assert(res_u16->second == 2_u128);
    int16_t b_i16 = 7;
    auto res_i16 = a.divrem(b_i16);
    assert(res_i16.has_value());
    assert(res_i16->first == 14_u128);
    assert(res_i16->second == 2_u128);
    uint8_t b_u8 = 7;
    auto res_u8 = a.divrem(b_u8);
    assert(res_u8.has_value());
    assert(res_u8->first == 14_u128);
    assert(res_u8->second == 2_u128);
    int8_t b_i8 = 7;
    auto res_i8 = a.divrem(b_i8);
    assert(res_i8.has_value());
    assert(res_i8->first == 14_u128);
    assert(res_i8->second == 2_u128);
    std::cout << "test_divrem: test_divrem_basic_integral_divisor passed" << std::endl;
}

void test_divrem_large_integral_divisor()
{
    std::cout << "test_divrem: test_divrem_large_integral_divisor ......" << std::endl;
    // 2^100
    uint128_t large = 1_u128 << 100;

    // Divisor como entero de 64 bits sin signo
    uint64_t divisor_u64 = 3ULL;

    auto res_u64 = large.divrem(divisor_u64);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res_u64->first * divisor_u64 + res_u64->second == large);

    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res_u64->second < divisor_u64);

    // Divisor como entero de 64 bits con signo
    int64_t divisor_i64 = 3LL;

    auto res_i64 = large.divrem(divisor_i64);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res_i64->first * divisor_i64 + res_i64->second == large);
    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res_i64->second < divisor_i64);

    // Divisor como entero de 32 bits sin signo
    uint32_t divisor_u32 = 3U;

    auto res_u32 = large.divrem(divisor_u32);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res_u32->first * divisor_u32 + res_u32->second == large);
    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res_u32->second < divisor_u32);

    // Divisor como entero de 32 bits con signo
    int32_t divisor_i32 = 3L;

    auto res_i32 = large.divrem(divisor_i32);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res_i32->first * divisor_i32 + res_i32->second == large);
    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res_i32->second < divisor_i32);

    // Divisor como entero de 16 bits sin signo
    uint16_t divisor_u16 = 3U;

    auto res_u16 = large.divrem(divisor_u16);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res_u16->first * divisor_u16 + res_u16->second == large);
    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res_u16->second < divisor_u16);

    // Divisor como entero de 16 bits con signo
    int16_t divisor_i16 = 3;

    auto res_i16 = large.divrem(divisor_i16);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res_i16->first * divisor_i16 + res_i16->second == large);
    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res_i16->second < divisor_i16);

    // Divisor como entero de 8 bits sin signo
    uint8_t divisor_u8 = 3U;

    auto res_u8 = large.divrem(divisor_u8);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res_u8->first * divisor_u8 + res_u8->second == large);
    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res_u8->second < divisor_u8);

    // Divisor como entero de 8 bits con signo
    int8_t divisor_i8 = 3;

    auto res_i8 = large.divrem(divisor_i8);

    // Verificación: dividendo = cociente * divisor + resto
    assert(res_i8->first * divisor_i8 + res_i8->second == large);
    // Resto debe ser menor que divisor (en valor absoluto)
    assert(res_i8->second < divisor_i8);

    std::cout << "test_divrem: test_divrem_large_integral_divisor passed" << std::endl;
}

void test_divrem_random()
{
    std::cout << "test_divrem: test_divrem_random ......" << std::endl;
    std::mt19937_64 rng(std::random_device{}());

    for (int i = 0; i < 1000; ++i) {
        uint128_t a(rng(), rng());
        uint128_t b(rng(), rng());

        if (b == 0_u128)
            b = 1_u128;

        auto res = a.divrem(b);
        assert(res.has_value());
        uint128_t q = res->first;
        uint128_t r = res->second;
        assert(r < b);

        // 4. Si el divisor es > 1 y el dividendo > 0, el cociente debe ser < que el dividendo.
        if (b > 1_u128 && a > 0_u128) {
            assert(q < a);
        }

        // Verificación sin asumir multiplicación completa:
        // a = q * b + r  =>  a - r = q * b
        uint128_t a_minus_r = a - r;

        // 1. (a - r) debe ser divisible por b y dar q
        auto check1 = a_minus_r.divrem(b);
        assert(check1.has_value());
        assert(check1->first == q);
        assert(check1->second == 0_u128);

        // 2. (a - r) debe ser divisible por q y dar b (si q > 0)
        if (q > 0_u128) {
            auto check2 = a_minus_r.divrem(q);
            assert(check2.has_value());
            assert(check2->first == b);
            assert(check2->second == 0_u128);
        }

        // 3. Comprobación con (a-1) (check3)
        if (a > 0_u128) {
            auto check3 = (a - 1_u128).divrem(b);
            assert(check3.has_value());
            if (r != 0_u128) {
                // Si a.divrem(b) -> [q,r] con r > 0, entonces (a-1).divrem(b) -> [q, r-1]
                assert(check3->first == q);
                assert(check3->second == r - 1_u128);
            } else // r == 0
            {
                // Si a.divrem(b) -> [q,0] con a > 0, entonces (a-1).divrem(b) -> [q-1, b-1]
                assert(check3->first == q - 1_u128);
                assert(check3->second == b - 1_u128);
            }
        }
    }
    std::cout << "test_divrem: test_divrem_random passed" << std::endl;
}

void test_divrem_random_integral_divisor()
{
    std::cout << "test_divrem: test_divrem_random_integral_divisor ......" << std::endl;
    std::mt19937_64 rng(std::random_device{}());

    for (int i = 0; i < 1000; ++i) {
        uint128_t a(rng(), rng());

        // Divisor como uint64_t
        auto b_u64(static_cast<uint64_t>(rng()));

        if (b_u64 == 0_u128)
            b_u64 = 1_u128;

        auto res_u64 = a.divrem(b_u64);
        assert(res_u64.has_value());
        uint128_t q_u64 = res_u64->first;
        uint128_t r_u64 = res_u64->second;
        assert(r_u64 < b_u64);

        // 4. Si el divisor es > 1 y el dividendo > 0, el cociente debe ser < que el dividendo.
        if (b_u64 > 1 && a > 0_u128) {
            assert(q_u64 < a);
        }

        // Verificación sin asumir multiplicación completa:
        // a = q * b + r  =>  a - r = q * b
        uint128_t a_minus_r = a - r_u64;

        // 1. (a - r) debe ser divisible por b y dar q
        auto check1_u64 = a_minus_r.divrem(b_u64);
        assert(check1_u64.has_value());
        assert(check1_u64->first == q_u64);
        assert(check1_u64->second == 0ULL);

        // 2. (a - r) debe ser divisible por q y dar b (si q > 0)
        if (q_u64 > 0_u128) {
            auto check2_u64 = a_minus_r.divrem(q_u64);
            assert(check2_u64.has_value());
            assert(check2_u64->first == b_u64);
            assert(check2_u64->second == 0ULL);
        }

        // 3. Comprobación con (a-1) (check3)
        if (a > 0_u128) {
            auto check3_u64 = (a - 1_u128).divrem(b_u64);
            assert(check3_u64.has_value());
            if (r_u64 != 0_u128) {
                // Si a.divrem(b) -> [q,r] con r > 0, entonces (a-1).divrem(b) -> [q, r-1]
                assert(check3_u64->first == q_u64);
                assert(check3_u64->second == r_u64 - 1_u128);
            } else // r == 0
            {
                // Si a.divrem(b) -> [q,0] con a > 0, entonces (a-1).divrem(b) -> [q-1, b-1]
                assert(check3_u64->first == q_u64 - 1_u128);
                assert(check3_u64->second == b_u64 - 1_u128);
            }
        }

        // Divisor como int64_t
        auto b_i64(static_cast<int64_t>(rng()));

        if (b_i64 == 0)
            b_i64 = 1;
        auto res_i64 = a.divrem(b_i64);
        assert(res_i64.has_value());
        uint128_t q_i64 = res_i64->first;
        uint128_t r_i64 = res_i64->second;
        assert(r_i64 < b_i64);

        // 4. Si el divisor es > 1 y el dividendo > 0, el cociente debe ser < que el dividendo.
        if (b_i64 > 1 && a > 0_u128) {
            assert(q_i64 < a);
        }

        // Verificación sin asumir multiplicación completa:
        // a = q * b + r  =>  a - r = q * b
        uint128_t a_minus_r = a - r_i64;

        // 1. (a - r) debe ser divisible por b y dar q
        auto check1_i64 = a_minus_r.divrem(b_i64);
        assert(check1_i64.has_value());
        assert(check1_i64->first == q_i64);
        assert(check1_i64->second == 0_u128);

        // 2. (a - r) debe ser divisible por q y dar b (si q > 0)
        if (q_i64 > 0_u128) {
            auto check2_i64 = a_minus_r.divrem(q_i64);
            assert(check2_i64.has_value());
            assert(check2_i64->first == b_i64);
            assert(check2_i64->second == 0_u128);
        }

        // 3. Comprobación con (a-1) (check3)
        if (a > 0_u128) {
            auto check3_i64 = (a - 1_u128).divrem(b_i64);
            assert(check3_i64.has_value());
            if (r_i64 != 0_u128) {
                // Si a.divrem(b) -> [q,r] con r > 0, entonces (a-1).divrem(b) -> [q, r-1]
                assert(check3_i64->first == q_i64);
                assert(check3_i64->second == r_i64 - 1_u128);
            } else // r == 0
            {
                // Si a.divrem(b) -> [q,0] con a > 0, entonces (a-1).divrem(b) -> [q-1, b-1]
                auto q2_i64 = q_i64;
                --q2_i64;
                auto b2_i64 = b_i64;
                --b2_i64;
                assert(check3_i64->first == q2_i64);
                assert(check3_i64->second == b2_i64);
            }
        }

        // Divisor como uint32_t
        auto b_u32(static_cast<uint32_t>(rng()));

        if (b_u32 == 0)
            b_u32 = 1;
        auto res_u32 = a.divrem(b_u32);
        assert(res_u32.has_value());
        uint128_t q_u32 = res_u32->first;
        uint128_t r_u32 = res_u32->second;
        assert(r_u32 < b_u32);

        // 4. Si el divisor es > 1 y el dividendo > 0, el cociente debe ser < que el dividendo.
        if (b_u32 > 1 && a > 0_u128) {
            assert(q_u32 < a);
        }

        // Verificación sin asumir multiplicación completa:
        // a = q * b + r  =>  a - r = q * b
        uint128_t a_minus_r = a - r_u32;

        // 1. (a - r) debe ser divisible por b y dar q
        auto check1_u32 = a_minus_r.divrem(b_u32);
        assert(check1_u32.has_value());
        assert(check1_u32->first == q_u32);
        assert(check1_u32->second == 0_u128);

        // 2. (a - r) debe ser divisible por q y dar b (si q > 0)
        if (q_u32 > 0_u128) {
            auto check2_u32 = a_minus_r.divrem(q_u32);
            assert(check2_u32.has_value());
            assert(check2_u32->first == b_u32);
            assert(check2_u32->second == 0_u128);
        }

        // 3. Comprobación con (a-1) (check3)
        if (a > 0_u128) {
            auto check3_u32 = (a - 1_u128).divrem(b_u32);
            assert(check3_u32.has_value());
            if (r_u32 != 0_u128) {
                // Si a.divrem(b) -> [q,r] con r > 0, entonces (a-1).divrem(b) -> [q, r-1]
                assert(check3_u32->first == q_u32);
                auto r2_u32 = r_u32;
                --r2_u32;
                assert(check3_u32->second == r2_u32);
            } else // r == 0
            {
                // Si a.divrem(b) -> [q,0] con a > 0, entonces (a-1).divrem(b) -> [q-1, b-1]
                auto q2_u32 = q_u32;
                --q2_u32;
                auto b2_u32 = b_u32;
                --b2_u32;
                assert(check3_u32->first == q2_u32);
                assert(check3_u32->second == b2_u32);
            }
        }

        // Divisor como int32_t
        auto b_i32(static_cast<int32_t>(rng()));

        if (b_i32 == 0)
            b_i32 = 1;
        auto res_i32 = a.divrem(b_i32);
        assert(res_i32.has_value());
        uint128_t q_i32 = res_i32->first;
        uint128_t r_i32 = res_i32->second;
        assert(r_i32 < b_i32);

        // 4. Si el divisor es > 1 y el dividendo > 0, el cociente debe ser < que el dividendo.
        if (b_i32 > 1 && a > 0_u128) {
            assert(q_i32 < a);
        }

        // Verificación sin asumir multiplicación completa:
        // a = q * b + r  =>  a - r = q * b
        uint128_t a_minus_r = a - r_i32;

        // 1. (a - r) debe ser divisible por b y dar q
        auto check1_i32 = a_minus_r.divrem(b_i32);
        assert(check1_i32.has_value());
        assert(check1_i32->first == q_i32);
        assert(check1_i32->second == 0_u128);

        // 2. (a - r) debe ser divisible por q y dar b (si q > 0)
        if (q_i32 > 0_u128) {
            auto check2_i32 = a_minus_r.divrem(q_i32);
            assert(check2_i32.has_value());
            assert(check2_i32->first == b_i32);
            assert(check2_i32->second == 0_u128);
        }

        // 3. Comprobación con (a-1) (check3)
        if (a > 0_u128) {
            auto check3_i32 = (a - 1_u128).divrem(b_i32);
            assert(check3_i32.has_value());
            if (r_i32 != 0_u128) {
                // Si a.divrem(b) -> [q,r] con r > 0, entonces (a-1).divrem(b) -> [q, r-1]
                assert(check3_i32->first == q_i32);
                auto r2_i32 = r_i32;
                --r2_i32;
                assert(check3_i32->second == r2_i32);
            } else // r == 0
            {
                // Si a.divrem(b) -> [q,0] con a > 0, entonces (a-1).divrem(b) -> [q-1, b-1]
                auto q2_i32 = q_i32;
                --q2_i32;
                auto b2_i32 = b_i32;
                --b2_i32;
                assert(check3_i32->first == q2_i32);
                assert(check3_i32->second == b2_i32);
            }
        }

        // Divisor como uint16_t
        auto b_u16(static_cast<uint16_t>(rng()));

        if (b_u16 == 0)
            b_u16 = 1;
        auto res_u16 = a.divrem(b_u16);
        assert(res_u16.has_value());
        uint128_t q_u16 = res_u16->first;
        uint128_t r_u16 = res_u16->second;
        assert(r_u16 < b_u16);

        // 4. Si el divisor es > 1 y el dividendo > 0, el cociente debe ser < que el dividendo.
        if (b_u16 > 1 && a > 0_u128) {
            assert(q_u16 < a);
        }

        // Verificación sin asumir multiplicación completa:
        // a = q * b + r  =>  a - r = q * b
        uint128_t a_minus_r = a - r_u16;

        // 1. (a - r) debe ser divisible por b y dar q
        auto check1_u16 = a_minus_r.divrem(b_u16);
        assert(check1_u16.has_value());
        assert(check1_u16->first == q_u16);
        assert(check1_u16->second == 0_u128);

        // 2. (a - r) debe ser divisible por q y dar b (si q > 0)
        if (q_u16 > 0_u128) {
            auto check2_u16 = a_minus_r.divrem(q_u16);
            assert(check2_u16.has_value());
            assert(check2_u16->first == b_u16);
            assert(check2_u16->second == 0_u128);
        }

        // 3. Comprobación con (a-1) (check3)
        if (a > 0_u128) {
            auto check3_u16 = (a - 1_u128).divrem(b_u16);
            assert(check3_u16.has_value());
            if (r_u16 != 0_u128) {
                // Si a.divrem(b) -> [q,r] con r > 0, entonces (a-1).divrem(b) -> [q, r-1]
                assert(check3_u16->first == q_u16);
                auto r2_u16 = r_u16;
                --r2_u16;
                assert(check3_u16->second == r2_u16);
            } else // r == 0
            {
                // Si a.divrem(b) -> [q,0] con a > 0, entonces (a-1).divrem(b) -> [q-1, b-1]
                auto q2_u16 = q_u16;
                --q2_u16;
                auto b2_u16 = b_u16;
                --b2_u16;
                assert(check3_u16->first == q2_u16);
                assert(check3_u16->second == b2_u16);
            }
        }

        // Divisor como int16_t
        auto b_i16(static_cast<int16_t>(rng()));

        if (b_i16 == 0)
            b_i16 = 1;
        auto res_i16 = a.divrem(b_i16);
        assert(res_i16.has_value());
        uint128_t q_i16 = res_i16->first;
        uint128_t r_i16 = res_i16->second;
        assert(r_i16 < b_i16);

        // 4. Si el divisor es > 1 y el dividendo > 0, el cociente debe ser < que el dividendo.
        if (b_i16 > 1 && a > 0_u128) {
            assert(q_i16 < a);
        }

        // Verificación sin asumir multiplicación completa:
        // a = q * b + r  =>  a - r = q * b
        uint128_t a_minus_r = a - r_i16;

        // 1. (a - r) debe ser divisible por b y dar q
        auto check1_i16 = a_minus_r.divrem(b_i16);
        assert(check1_i16.has_value());
        assert(check1_i16->first == q_i16);
        assert(check1_i16->second == 0_u128);

        // 2. (a - r) debe ser divisible por q y dar b (si q > 0)
        if (q_i16 > 0_u128) {
            auto check2_i16 = a_minus_r.divrem(q_i16);
            assert(check2_i16.has_value());
            assert(check2_i16->first == b_i16);
            assert(check2_i16->second == 0_u128);
        }

        // 3. Comprobación con (a-1) (check3)
        if (a > 0_u128) {
            auto check3_i16 = (a - 1_u128).divrem(b_i16);
            assert(check3_i16.has_value());
            if (r_i16 != 0_u128) {
                // Si a.divrem(b) -> [q,r] con r > 0, entonces (a-1).divrem(b) -> [q, r-1]
                assert(check3_i16->first == q_i16);
                auto r2_i16 = r_i16;
                --r2_i16;
                assert(check3_i16->second == r2_i16);
            } else // r == 0
            {
                // Si a.divrem(b) -> [q,0] con a > 0, entonces (a-1).divrem(b) -> [q-1, b-1]
                auto q2_i16 = q_i16;
                --q2_i16;
                auto b2_i16 = b_i16;
                --b2_i16;
                assert(check3_i16->first == q2_i16);
                assert(check3_i16->second == b2_i16);
            }
        }

        // Divisor como uint8_t
        auto b_u8(static_cast<uint8_t>(rng()));

        if (b_u8 == 0)
            b_u8 = 1;
        auto res_u8 = a.divrem(b_u8);
        assert(res_u8.has_value());
        uint128_t q_u8 = res_u8->first;
        uint128_t r_u8 = res_u8->second;
        assert(r_u8 < b_u8);

        // 4. Si el divisor es > 1 y el dividendo > 0, el cociente debe ser < que el dividendo.
        if (b_u8 > 1 && a > 0_u128) {
            assert(q_u8 < a);
        }

        // Verificación sin asumir multiplicación completa:
        // a = q * b + r  =>  a - r = q * b
        uint128_t a_minus_r = a - r_u8;

        // 1. (a - r) debe ser divisible por b y dar q
        auto check1_u8 = a_minus_r.divrem(b_u8);
        assert(check1_u8.has_value());
        assert(check1_u8->first == q_u8);
        assert(check1_u8->second == 0_u128);

        // 2. (a - r) debe ser divisible por q y dar b (si q > 0)
        if (q_u8 > 0_u128) {
            auto check2_u8 = a_minus_r.divrem(q_u8);
            assert(check2_u8.has_value());
            assert(check2_u8->first == b_u8);
            assert(check2_u8->second == 0_u128);
        }

        // 3. Comprobación con (a-1) (check3)
        if (a > 0_u128) {
            auto check3_u8 = (a - 1_u128).divrem(b_u8);
            assert(check3_u8.has_value());
            if (r_u8 != 0_u128) {
                // Si a.divrem(b) -> [q,r] con r > 0, entonces (a-1).divrem(b) -> [q, r-1]
                assert(check3_u8->first == q_u8);
                auto r2_u8 = r_u8;
                --r2_u8;
                assert(check3_u8->second == r2_u8);
            } else // r == 0
            {
                // Si a.divrem(b) -> [q,0] con a > 0, entonces (a-1).divrem(b) -> [q-1, b-1]
                auto q2_u8 = q_u8;
                --q2_u8;
                auto b2_u8 = b_u8;
                --b2_u8;
                assert(check3_u8->first == q2_u8);
                assert(check3_u8->second == b2_u8);
            }
        }

        // Divisor como int8_t
        auto b_i8(static_cast<int8_t>(rng()));

        if (b_i8 == 0)
            b_i8 = 1;
        auto res_i8 = a.divrem(b_i8);
        assert(res_i8.has_value());
        uint128_t q_i8 = res_i8->first;
        uint128_t r_i8 = res_i8->second;
        assert(r_i8 < b_i8);

        // 4. Si el divisor es > 1 y el dividendo > 0, el cociente debe ser < que el dividendo.
        if (b_i8 > 1 && a > 0_u128) {
            assert(q_i8 < a);
        }

        // Verificación sin asumir multiplicación completa:
        // a = q * b + r  =>  a - r = q * b
        uint128_t a_minus_r = a - r_i8;

        // 1. (a - r) debe ser divisible por b y dar q
        auto check1_i8 = a_minus_r.divrem(b_i8);
        assert(check1_i8.has_value());
        assert(check1_i8->first == q_i8);
        assert(check1_i8->second == 0_u128);

        // 2. (a - r) debe ser divisible por q y dar b (si q > 0)
        if (q_i8 > 0_u128) {
            auto check2_i8 = a_minus_r.divrem(q_i8);
            assert(check2_i8.has_value());
            assert(check2_i8->first == b_i8);
            assert(check2_i8->second == 0_u128);
        }

        // 3. Comprobación con (a-1) (check3)
        if (a > 0_u128) {
            auto check3_i8 = (a - 1_u128).divrem(b_i8);
            assert(check3_i8.has_value());
            if (r_i8 != 0_u128) {
                // Si a.divrem(b) -> [q,r] con r > 0, entonces (a-1).divrem(b) -> [q, r-1]
                assert(check3_i8->first == q_i8);
                auto r2_i8 = r_i8;
                --r2_i8;
                assert(check3_i8->second == r2_i8);
            } else // r == 0
            {
                // Si a.divrem(b) -> [q,0] con a > 0, entonces (a-1).divrem(b) -> [q-1, b-1]
                auto q2_i8 = q_i8;
                --q2_i8;
                auto b2_i8 = b_i8;
                --b2_i8;
                assert(check3_i8->first == q2_i8);
                assert(check3_i8->second == b2_i8);
            }
        }
    }
    std::cout << "test_divrem: test_divrem_random_integral_divisor passed" << std::endl;
}

void test_divrem_known_result()
{
    std::cout << "test_divrem: test_divrem_known_result ......" << std::endl;

    // 100 / 3 = 33 rem 1
    auto res1 = 100_u128.divrem(3_u128);
    assert(res1->first == 33_u128);
    assert(res1->second == 1_u128);

    // Max / 2 = (2^128 - 1) / 2 = 2^127 - 1 rem 1
    uint128_t max_val(UINT64_MAX, UINT64_MAX);
    auto res2 = max_val.divrem(2_u128);
    assert(res2->first == uint128_t(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL));
    assert(res2->second == 1_u128);

    std::cout << "test_divrem: test_divrem_known_result passed" << std::endl;
}

void test_divrem_known_result_integral_divisor()
{
    std::cout << "test_divrem: test_divrem_known_result_integral_divisor ......" << std::endl;

    // divisor como uint64_t

    // 100 / 3 (int)
    auto res_u64 = 100_u128.divrem(static_cast<uint64_t>(3u));
    assert(res_u64->first == 33_u128);
    assert(res_u64->second == 1_u128);

    // Max / 2 (uint64_t)
    uint128_t max_val(UINT64_MAX, UINT64_MAX);
    auto res_u64_2 = max_val.divrem(static_cast<uint64_t>(2u));
    assert(res_u64_2->first == uint128_t(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL));
    assert(res_u64_2->second == 1_u128);

    // divisor como int64_t

    // 100 / 3 (int)
    auto res_i64 = 100_u128.divrem(static_cast<int64_t>(3));
    assert(res_i64->first == 33_u128);
    assert(res_i64->second == 1_u128);

    // Max / 2 (int64_t)
    uint128_t max_val(UINT64_MAX, UINT64_MAX);
    auto res_i64_2 = max_val.divrem(static_cast<int64_t>(2));
    assert(res_i64_2->first == uint128_t(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL));
    assert(res_i64_2->second == 1_u128);

    // divisor como uint32_t
    // 100 / 3 (uint32_t)
    auto res_u32 = 100_u128.divrem(static_cast<uint32_t>(3u));
    assert(res_u32->first == 33_u128);
    assert(res_u32->second == 1_u128);
    // Max / 2 (uint32_t)
    uint128_t max_val2(UINT64_MAX, UINT64_MAX);
    auto res_u32_2 = max_val2.divrem(static_cast<uint32_t>(2u));
    assert(res_u32_2->first == uint128_t(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL));
    assert(res_u32_2->second == 1_u128);

    // divisor como int32_t
    // 100 / 3 (int32_t)
    auto res_i32 = 100_u128.divrem(static_cast<int32_t>(3));
    assert(res_i32->first == 33_u128);
    assert(res_i32->second == 1_u128);
    // Max / 2 (int32_t)
    uint128_t max_val3(UINT64_MAX, UINT64_MAX);
    auto res_i32_2 = max_val3.divrem(static_cast<int32_t>(2));
    assert(res_i32_2->first == uint128_t(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL));
    assert(res_i32_2->second == 1_u128);

    // divisor como uint16_t
    // 100 / 3 (uint16_t)
    auto res_u16 = 100_u128.divrem(static_cast<uint16_t>(3u));
    assert(res_u16->first == 33_u128);
    assert(res_u16->second == 1_u128);
    // Max / 2 (uint16_t)
    uint128_t max_val4(UINT64_MAX, UINT64_MAX);
    auto res_u16_2 = max_val4.divrem(static_cast<uint16_t>(2u));
    assert(res_u16_2->first == uint128_t(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL));
    assert(res_u16_2->second == 1_u128);

    // divisor como int16_t
    // 100 / 3 (int16_t)
    auto res_i16 = 100_u128.divrem(static_cast<int16_t>(3));
    assert(res_i16->first == 33_u128);
    assert(res_i16->second == 1_u128);
    // Max / 2 (int16_t)
    uint128_t max_val5(UINT64_MAX, UINT64_MAX);
    auto res_i16_2 = max_val5.divrem(static_cast<int16_t>(2));
    assert(res_i16_2->first == uint128_t(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL));
    assert(res_i16_2->second == 1_u128);

    // divisor como uint8_t
    // 100 / 3 (uint8_t)
    auto res_u8 = 100_u128.divrem(static_cast<uint8_t>(3u));
    assert(res_u8->first == 33_u128);
    assert(res_u8->second == 1_u128);
    // Max / 2 (uint8_t)
    uint128_t max_val6(UINT64_MAX, UINT64_MAX);
    auto res_u8_2 = max_val6.divrem(static_cast<uint8_t>(2u));
    assert(res_u8_2->first == uint128_t(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL));
    assert(res_u8_2->second == 1_u128);

    // divisor como int8_t
    // 100 / 3 (int8_t)
    auto res_i8 = 100_u128.divrem(static_cast<int8_t>(3));
    assert(res_i8->first == 33_u128);
    assert(res_i8->second == 1_u128);
    // Max / 2 (int8_t)
    uint128_t max_val7(UINT64_MAX, UINT64_MAX);
    auto res_i8_2 = max_val7.divrem(static_cast<int8_t>(2));
    assert(res_i8_2->first == uint128_t(0x7FFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL));
    assert(res_i8_2->second == 1_u128);

    std::cout << "test_divrem: test_divrem_known_result_integral_divisor passed" << std::endl;
}

int main()
{
    std::cout << "=== Test uint128_t::divrem ===" << std::endl;
    test_divrem_basic();
    test_divrem_zero();
    test_divrem_large();
    test_divrem_random();
    test_divrem_known_result();
    test_divrem_basic_integral_divisor();
    test_divrem_zero_integral_divisor();
    test_divrem_large_integral_divisor();
    test_divrem_random_integral_divisor();
    test_divrem_known_result_integral_divisor();
    std::cout << "\n All divrem_tests passed!" << std::endl;
    return 0;
}