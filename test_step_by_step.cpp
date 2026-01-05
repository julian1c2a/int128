// Test paso a paso del bug de parse_base
#include "include_new/int128_base.hpp"
#include <iostream>

using namespace nstd;

// Test manual simulando parse_base
void test_manual_parse()
{
    std::cout << "\n=== SIMULACION MANUAL DE parse_base(\"12\", 10) ===\n";

    uint128_t result{0ull, 0ull};
    uint128_t base_val{10ull, 0ull};

    std::cout << "Inicial: result = " << result.low() << ", " << result.high() << "\n";

    // Primera iteración: dígito '1'
    {
        std::cout << "\n--- Iteracion 1: digito='1' (value=1) ---\n";
        uint128_t old_result = result;
        std::cout << "old_result = " << old_result.low() << ", " << old_result.high() << "\n";

        result = result * base_val;
        std::cout << "Despues de multiplicar: result = " << result.low() << ", " << result.high()
                  << "\n";

        // Check overflow multiplicación
        bool overflow_mul = result < old_result && old_result != uint128_t{0ull, 0ull};
        std::cout << "result < old_result = " << (result < old_result ? "true" : "false") << "\n";
        std::cout << "old_result != 0 = "
                  << (old_result != uint128_t{0ull, 0ull} ? "true" : "false") << "\n";
        std::cout << "overflow_mul = " << (overflow_mul ? "TRUE [ERROR!]" : "false") << "\n";

        if (overflow_mul) {
            std::cout << "*** OVERFLOW EN MULTIPLICACION DETECTADO ***\n";
            return;
        }

        // Sumar el dígito
        uint128_t digit_val{1ull, 0ull};
        uint128_t old_result2 = result;
        std::cout << "old_result2 antes de sumar = " << old_result2.low() << ", "
                  << old_result2.high() << "\n";

        result = result + digit_val;
        std::cout << "Despues de sumar digit=1: result = " << result.low() << ", " << result.high()
                  << "\n";

        // Check overflow suma
        bool overflow_sum = result < old_result2;
        std::cout << "result < old_result2 = " << (result < old_result2 ? "TRUE [ERROR!]" : "false")
                  << "\n";

        if (overflow_sum) {
            std::cout << "*** OVERFLOW EN SUMA DETECTADO ***\n";
            return;
        }

        std::cout << "Resultado final iteracion 1: " << result.low() << ", " << result.high()
                  << "\n";
    }

    // Segunda iteración: dígito '2'
    {
        std::cout << "\n--- Iteracion 2: digito='2' (value=2) ---\n";
        uint128_t old_result = result;
        std::cout << "old_result = " << old_result.low() << ", " << old_result.high() << "\n";

        result = result * base_val;
        std::cout << "Despues de multiplicar: result = " << result.low() << ", " << result.high()
                  << "\n";

        // Check overflow multiplicación
        bool overflow_mul = result < old_result && old_result != uint128_t{0ull, 0ull};
        std::cout << "result < old_result = " << (result < old_result ? "true" : "false") << "\n";
        std::cout << "old_result != 0 = "
                  << (old_result != uint128_t{0ull, 0ull} ? "true" : "false") << "\n";
        std::cout << "overflow_mul = " << (overflow_mul ? "TRUE [ERROR!]" : "false") << "\n";

        if (overflow_mul) {
            std::cout << "*** OVERFLOW EN MULTIPLICACION DETECTADO ***\n";
            return;
        }

        // Sumar el dígito
        uint128_t digit_val{2ull, 0ull};
        uint128_t old_result2 = result;
        std::cout << "old_result2 antes de sumar = " << old_result2.low() << ", "
                  << old_result2.high() << "\n";

        result = result + digit_val;
        std::cout << "Despues de sumar digit=2: result = " << result.low() << ", " << result.high()
                  << "\n";

        // Check overflow suma
        bool overflow_sum = result < old_result2;
        std::cout << "result < old_result2 = " << (result < old_result2 ? "TRUE [ERROR!]" : "false")
                  << "\n";

        if (overflow_sum) {
            std::cout << "*** OVERFLOW EN SUMA DETECTADO ***\n";
            return;
        }

        std::cout << "Resultado final iteracion 2: " << result.low() << ", " << result.high()
                  << "\n";
    }

    std::cout << "\n=== PARSING EXITOSO: result = " << result.low() << " ===\n";
}

int main()
{
    test_manual_parse();

    std::cout << "\n\n=== TEST REAL DE parse_base ===\n";
    auto [err, val] = uint128_t::parse_base("12", 10);
    std::cout << "parse_base(\"12\", 10): error=" << static_cast<int>(err)
              << ", value=" << val.low() << "\n";

    return 0;
}
