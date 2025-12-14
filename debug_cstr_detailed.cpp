#include "include/uint128_t.hpp"
#include <cassert>
#include <cstring>
#include <iostream>

int main()
{
    uint128_t value(0, 12345);

    const char* decimal_cstr = value.to_cstr();

    std::cout << "Valor uint128_t: " << value.to_string() << std::endl;
    std::cout << "to_cstr() resultado: '" << decimal_cstr << "'" << std::endl;
    std::cout << "strlen: " << std::strlen(decimal_cstr) << std::endl;

    // Comparación manual caracter por caracter
    const char* expected = "12345";
    std::cout << "Esperado: '" << expected << "'" << std::endl;

    for (int i = 0; i < 10; ++i) {
        char actual = (i < static_cast<int>(std::strlen(decimal_cstr))) ? decimal_cstr[i] : '\0';
        char expect = (i < static_cast<int>(std::strlen(expected))) ? expected[i] : '\0';
        std::cout << "Pos " << i << ": actual='" << actual << "' (" << static_cast<int>(actual)
                  << "), expected='" << expect << "' (" << static_cast<int>(expect) << ")"
                  << std::endl;
        if (actual != expect)
            break;
    }

    int cmp_result = std::strcmp(decimal_cstr, expected);
    std::cout << "strcmp resultado: " << cmp_result << std::endl;
    std::cout << "Comparación: " << (cmp_result == 0 ? "IGUAL" : "DIFERENTE") << std::endl;

    return 0;
}