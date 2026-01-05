#include "int128_base.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "=== Test de constantes globales ===\n\n";

    // 1. UINT128_MAX
    std::cout << "1. UINT128_MAX:\n";
    std::cout << "   high() = " << UINT128_MAX.high() << "\n";
    std::cout << "   low()  = " << UINT128_MAX.low() << "\n";
    std::cout << "   ¿Es == uint128_t::max()? " << (UINT128_MAX == uint128_t::max() ? "SI" : "NO")
              << "\n\n";

    // 2. INT128_MAX
    std::cout << "2. INT128_MAX:\n";
    std::cout << "   high() = " << INT128_MAX.high() << "\n";
    std::cout << "   low()  = " << INT128_MAX.low() << "\n";
    std::cout << "   ¿Es == int128_t::max()? " << (INT128_MAX == int128_t::max() ? "SI" : "NO")
              << "\n";
    std::cout << "   ¿Es > 0? " << (INT128_MAX > int128_t(0ull, 0ull) ? "SI" : "NO") << "\n\n";

    // 3. INT128_MIN
    std::cout << "3. INT128_MIN:\n";
    std::cout << "   high() = " << INT128_MIN.high() << "\n";
    std::cout << "   low()  = " << INT128_MIN.low() << "\n";
    std::cout << "   ¿Es == int128_t::min()? " << (INT128_MIN == int128_t::min() ? "SI" : "NO")
              << "\n";
    std::cout << "   ¿Es < 0? " << (INT128_MIN < int128_t(0ull, 0ull) ? "SI" : "NO") << "\n\n";

    // 4. Verificar uso en constexpr
    constexpr uint128_t umax = UINT128_MAX;
    constexpr int128_t imax = INT128_MAX;
    constexpr int128_t imin = INT128_MIN;

    std::cout << "4. Uso en constexpr:\n";
    std::cout << "   constexpr uint128_t umax: high=" << umax.high() << ", low=" << umax.low()
              << "\n";
    std::cout << "   constexpr int128_t imax: high=" << imax.high() << ", low=" << imax.low()
              << "\n";
    std::cout << "   constexpr int128_t imin: high=" << imin.high() << ", low=" << imin.low()
              << "\n\n";

    // 5. Verificar abs()
    std::cout << "5. Test abs():\n";
    int128_t positive(0ull, 42ull);
    int128_t negative = -positive;
    std::cout << "   positive.high() = " << positive.high() << ", low() = " << positive.low()
              << "\n";
    std::cout << "   negative.high() = " << negative.high() << ", low() = " << negative.low()
              << "\n";

    int128_t abs_pos = positive.abs();
    int128_t abs_neg = negative.abs();
    std::cout << "   abs(positive).high() = " << abs_pos.high() << ", low() = " << abs_pos.low()
              << "\n";
    std::cout << "   abs(negative).high() = " << abs_neg.high() << ", low() = " << abs_neg.low()
              << "\n";
    std::cout << "   ¿abs(positive) == abs(negative)? " << (abs_pos == abs_neg ? "SI" : "NO")
              << "\n\n";

    std::cout << "✓ Todos los tests completados\n";

    return 0;
}
