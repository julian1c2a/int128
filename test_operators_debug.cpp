#include "int128_base.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "Test operadores con int128_t\n\n";

    // 1. Crear valor base
    int128_t result(0ull, 0ull);
    int128_t base_val(0ull, 10ull);

    std::cout << "1. Valores iniciales:\n";
    std::cout << "   result: high=" << result.high() << ", low=" << result.low() << "\n";
    std::cout << "   base_val: high=" << base_val.high() << ", low=" << base_val.low() << "\n";

    // 2. Calcular max_val / base_val
    int128_t max_val(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);
    std::cout << "\n2. max_val (UINT128_MAX):\n";
    std::cout << "   high=" << max_val.high() << ", low=" << max_val.low() << "\n";

    int128_t max_div_base = max_val / base_val;
    std::cout << "\n3. max_div_base = max_val / base_val:\n";
    std::cout << "   high=" << max_div_base.high() << ", low=" << max_div_base.low() << "\n";

    // 4. Comparación result > max_div_base
    std::cout << "\n4. ¿result > max_div_base?\n";
    bool cmp = result > max_div_base;
    std::cout << "   " << (cmp ? "SI" : "NO") << " (debería ser NO)\n";

    // 5. Comparación inversa para debug
    std::cout << "\n5. ¿result < max_div_base?\n";
    bool cmp2 = result < max_div_base;
    std::cout << "   " << (cmp2 ? "SI" : "NO") << " (debería ser SI)\n";

    std::cout << "\n6. ¿result == 0?\n";
    int128_t zero(0ull, 0ull);
    bool cmp3 = (result == zero);
    std::cout << "   " << (cmp3 ? "SI" : "NO") << " (debería ser SI)\n";

    return 0;
}
