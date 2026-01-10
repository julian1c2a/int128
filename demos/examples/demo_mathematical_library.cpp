/**
 * @file demo_mathematical_library.cpp
 * @brief Demostración de funciones matemáticas para int128_t/uint128_t
 *
 * Compilar:
 *   g++ -std=c++20 -O2 -Iinclude demos/examples/demo_mathematical_library.cpp \
 *       -o demo_mathematical_library
 */

#include <int128_simple.hpp>
#include <int128_base_cmath.hpp>
#include <iostream>

using namespace nstd;
using namespace nstd::int128_literals;

void demo_basic_math()
{
    std::cout << "\n=== DEMO: Funciones Matematicas Basicas ===" << std::endl;

    // Demostrar GCD y LCM
    const uint128_t a{48};
    const uint128_t b{18};
    const auto gcd_result = nstd::gcd(a, b);
    const auto lcm_result = nstd::lcm(a, b);

    std::cout << "GCD(48, 18) = " << gcd_result.to_string() << std::endl;
    std::cout << "LCM(48, 18) = " << lcm_result.to_string() << std::endl;
    std::cout << "Verificacion: GCD x LCM = " << (gcd_result * lcm_result).to_string()
              << ", a x b = " << (a * b).to_string() << std::endl;

    // Potenciación
    const auto power_result = nstd::pow(uint128_t{2}, uint128_t{32});
    std::cout << "2^32 = " << power_result.to_string() << std::endl;

    // Potenciación con números grandes
    const auto power_large = nstd::pow(uint128_t{3}, uint128_t{50});
    std::cout << "3^50 = " << power_large.to_string() << std::endl;

    // Raíz cuadrada
    const auto sqrt_result = nstd::sqrt(uint128_t{1000000});
    std::cout << "sqrt(1000000) = " << sqrt_result.to_string() << std::endl;

    const auto sqrt_large = nstd::sqrt(power_result);
    std::cout << "sqrt(2^32) = " << sqrt_large.to_string() << std::endl;
}

void demo_signed_math()
{
    std::cout << "\n=== DEMO: Operaciones con Signed (int128_t) ===" << std::endl;

    // Valor absoluto
    const int128_t neg_val{-12345};
    const auto abs_result = nstd::abs(neg_val);
    std::cout << "abs(-12345) = " << abs_result.to_string() << std::endl;

    // GCD con valores signed
    const int128_t x{-48};
    const int128_t y{18};
    const auto gcd_signed = nstd::gcd(x, y);
    std::cout << "GCD(-48, 18) = " << gcd_signed.to_string() << std::endl;

    // min, max, clamp
    const int128_t a{-100};
    const int128_t b{50};
    std::cout << "min(-100, 50) = " << nstd::min(a, b).to_string() << std::endl;
    std::cout << "max(-100, 50) = " << nstd::max(a, b).to_string() << std::endl;

    const int128_t val{-150};
    const int128_t lo{-100};
    const int128_t hi{100};
    std::cout << "clamp(-150, -100, 100) = " << nstd::clamp(val, lo, hi).to_string() << std::endl;

    // sign function
    std::cout << "sign(-100) = " << nstd::sign(a) << std::endl;
    std::cout << "sign(50) = " << nstd::sign(b) << std::endl;
    std::cout << "sign(0) = " << nstd::sign(int128_t{0}) << std::endl;
}

void demo_large_numbers()
{
    std::cout << "\n=== DEMO: Numeros Muy Grandes ===" << std::endl;

    // Potencias de 2 grandes
    const auto pow_64 = nstd::pow(uint128_t{2}, uint128_t{64});
    std::cout << "2^64 = " << pow_64.to_string() << std::endl;

    const auto pow_100 = nstd::pow(uint128_t{2}, uint128_t{100});
    std::cout << "2^100 = " << pow_100.to_string() << std::endl;
    std::cout << "Este numero tiene " << pow_100.to_string().length() << " digitos" << std::endl;

    // Raíz cuadrada de potencias
    const auto sqrt_pow_100 = nstd::sqrt(pow_100);
    std::cout << "sqrt(2^100) = 2^50 = " << sqrt_pow_100.to_string() << std::endl;

    // Verificar: sqrt(2^100)^2 debe ser aproximadamente 2^100
    const auto verify = sqrt_pow_100 * sqrt_pow_100;
    std::cout << "Verificacion: (2^50)^2 = " << verify.to_string() << std::endl;

    // GCD de números grandes
    const auto large1 = nstd::pow(uint128_t{2}, uint128_t{30}) * uint128_t{3};
    const auto large2 = nstd::pow(uint128_t{2}, uint128_t{25}) * uint128_t{5};
    const auto gcd_large = nstd::gcd(large1, large2);
    std::cout << "\nNumero1 = 2^30 * 3 = " << large1.to_string() << std::endl;
    std::cout << "Numero2 = 2^25 * 5 = " << large2.to_string() << std::endl;
    std::cout << "GCD = 2^25 = " << gcd_large.to_string() << std::endl;
}

void demo_fibonacci()
{
    std::cout << "\n=== DEMO: Fibonacci con uint128_t ===" << std::endl;

    // Calcular Fibonacci hasta F(185) (máximo que cabe en 128 bits)
    uint128_t fib_prev{0};
    uint128_t fib_curr{1};

    std::cout << "F(0) = " << fib_prev.to_string() << std::endl;
    std::cout << "F(1) = " << fib_curr.to_string() << std::endl;

    // Calcular algunos valores clave
    for (int i = 2; i <= 186; ++i)
    {
        const uint128_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;

        // Mostrar algunos hitos
        if (i == 50 || i == 100 || i == 150 || i == 180 || i == 185 || i == 186)
        {
            std::cout << "F(" << i << ") = " << fib_curr.to_string()
                      << " (" << fib_curr.to_string().length() << " digitos)" << std::endl;
        }
    }

    // Propiedad: GCD(F(n), F(n-1)) = 1 siempre
    const auto gcd_fib = nstd::gcd(fib_curr, fib_prev);
    std::cout << "GCD(F(186), F(185)) = " << gcd_fib.to_string()
              << " (siempre es 1 para Fibonacci consecutivos)" << std::endl;
}

void demo_midpoint()
{
    std::cout << "\n=== DEMO: Midpoint sin Overflow ===" << std::endl;

    // Midpoint de números grandes (evita overflow)
    const uint128_t max_val = uint128_t::max();
    const uint128_t almost_max = max_val - uint128_t{100};

    std::cout << "a = MAX_UINT128 = " << max_val.to_string() << std::endl;
    std::cout << "b = MAX_UINT128 - 100 = " << almost_max.to_string() << std::endl;

    // (a + b) / 2 causaría overflow, pero midpoint lo maneja correctamente
    const auto mid = nstd::midpoint(almost_max, max_val);
    std::cout << "midpoint(a, b) = " << mid.to_string() << std::endl;

    // Verificar
    const auto expected = almost_max + uint128_t{50};
    std::cout << "Esperado (b + 50) = " << expected.to_string() << std::endl;
    std::cout << "Correcto?: " << (mid == expected ? "SI" : "NO") << std::endl;
}

void demo_divmod()
{
    std::cout << "\n=== DEMO: Division y Modulo Simultaneos ===" << std::endl;

    const int128_t dividend{12345};
    const int128_t divisor{100};

    const auto result = nstd::divmod(dividend, divisor);

    std::cout << dividend.to_string() << " / " << divisor.to_string()
              << " = " << result.first.to_string() << std::endl;
    std::cout << dividend.to_string() << " % " << divisor.to_string()
              << " = " << result.second.to_string() << std::endl;

    // Verificar: quotient * divisor + remainder = dividend
    const auto verify = result.first * divisor + result.second;
    std::cout << "Verificacion: " << result.first.to_string() << " * " << divisor.to_string()
              << " + " << result.second.to_string() << " = " << verify.to_string() << std::endl;
}

int main()
{
    std::cout << "+==============================================+" << std::endl;
    std::cout << "|  DEMO: Funciones Matematicas int128/uint128  |" << std::endl;
    std::cout << "+==============================================+" << std::endl;

    demo_basic_math();
    demo_signed_math();
    demo_large_numbers();
    demo_fibonacci();
    demo_midpoint();
    demo_divmod();

    std::cout << "\n+==============================================+" << std::endl;
    std::cout << "|  Capacidades demostradas:                    |" << std::endl;
    std::cout << "|  - gcd, lcm (Algoritmo de Stein)             |" << std::endl;
    std::cout << "|  - pow (exponenciacion por cuadrados)        |" << std::endl;
    std::cout << "|  - sqrt (Newton-Raphson)                     |" << std::endl;
    std::cout << "|  - abs, min, max, clamp, sign                |" << std::endl;
    std::cout << "|  - midpoint (sin overflow)                   |" << std::endl;
    std::cout << "|  - divmod (division + modulo)                |" << std::endl;
    std::cout << "+==============================================+" << std::endl;

    return 0;
}
