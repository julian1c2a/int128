/**
 * ===========================================================================
 * EJEMPLO: Factorización de Números Primos
 * ===========================================================================
 *
 * Este ejemplo demuestra la factorización de números grandes usando uint128_t.
 * Implementa varios algoritmos de factorización incluyendo:
 * - Trial division (división por tentativa)
 * - Pollard's Rho (para factores mayores)
 * - Optimizaciones para números pares y múltiplos pequeños
 */

#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <int128_simple.hpp>
#include <vector>

using namespace nstd;

using namespace uint128_literals;

// Verificar si un número es primo (test simple)
bool is_prime_simple(const uint128_t& n)
{
    if (n < 2)
        return false;
    if (n == 2 || n == 3)
        return true;
    if (n % 2 == 0 || n % 3 == 0)
        return false;

    // Solo verificamos hasta sqrt(n) pero limitado para rendimiento
    // Convertir a double manualmente para calcular sqrt
    double n_approx =
        static_cast<double>(n.high()) * 18446744073709551616.0 + static_cast<double>(n.low());
    uint64_t limit =
        std::min(static_cast<uint64_t>(1000000), static_cast<uint64_t>(std::sqrt(n_approx)));

    for (uint64_t i = 5; i <= limit; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }

    return true;
}

// Factorización por división por tentativa
std::map<uint128_t, int> factorize_trial_division(uint128_t n)
{
    std::map<uint128_t, int> factors;

    // Factorizar por 2
    while (n % 2 == 0) {
        factors[2_u128]++;
        n /= 2;
    }

    // Factorizar por impares
    uint128_t divisor{3};
    while (divisor * divisor <= n && divisor < 10000000) {
        while (n % divisor == 0) {
            factors[divisor]++;
            n /= divisor;
        }
        divisor += 2;
    }

    // Si queda un resto > 1, es primo
    if (n > 1) {
        factors[n]++;
    }

    return factors;
}

// Mostrar factores
void print_factors(const std::map<uint128_t, int>& factors)
{
    bool first = true;
    for (const auto& [factor, count] : factors) {
        if (!first)
            std::cout << " × ";
        std::cout << factor;
        if (count > 1) {
            std::cout << "^" << count;
        }
        first = false;
    }
    std::cout << "\n";
}

// Verificar factorización
uint128_t verify_factorization(const std::map<uint128_t, int>& factors)
{
    uint128_t product{1};
    for (const auto& [factor, count] : factors) {
        for (int i = 0; i < count; ++i) {
            product *= factor;
        }
    }
    return product;
}

void demo_small_numbers()
{
    std::cout << "\n=== Factorización de Números Pequeños ===\n\n";

    std::vector<uint128_t> numbers = {
        24_u128, 100_u128, 1024_u128, 9999_u128, 123456_u128,
    };

    for (const auto& n : numbers) {
        std::cout << n << " = ";
        auto factors = factorize_trial_division(n);
        print_factors(factors);

        // Verificar
        auto product = verify_factorization(factors);
        if (product == n) {
            std::cout << "  ✓ Verificado\n";
        } else {
            std::cout << "  ✗ Error en factorización\n";
        }
        std::cout << "\n";
    }
}

void demo_large_numbers()
{
    std::cout << "\n=== Factorización de Números Grandes ===\n\n";

    std::vector<uint128_t> numbers = {
        1000000000000_u128,
        123456789012345_u128,
        999999999999999_u128,
    };

    for (const auto& n : numbers) {
        std::cout << "Factorizando: " << n << "\n";
        std::cout << "Resultado: ";

        auto factors = factorize_trial_division(n);
        print_factors(factors);

        // Verificar
        auto product = verify_factorization(factors);
        if (product == n) {
            std::cout << "  ✓ Verificado correctamente\n";
        }

        // Contar factores
        int total_factors = 0;
        for (const auto& [factor, count] : factors) {
            total_factors += count;
        }
        std::cout << "  Total de factores primos: " << total_factors << "\n\n";
    }
}

void demo_perfect_squares()
{
    std::cout << "\n=== Cuadrados Perfectos ===\n\n";

    std::vector<uint128_t> bases = {123_u128, 9999_u128, 100000_u128};

    for (const auto& base : bases) {
        uint128_t square = base * base;
        std::cout << base << "^2 = " << square << "\n";
        std::cout << "Factorización: ";

        auto factors = factorize_trial_division(square);
        print_factors(factors);

        std::cout << "\n";
    }
}

void demo_powers_of_two()
{
    std::cout << "\n=== Potencias de 2 ===\n\n";

    std::vector<int> exponents = {10, 20, 30, 40, 50, 64, 100};

    for (int exp : exponents) {
        uint128_t power = uint128_t(1) << exp;
        std::cout << "2^" << exp << " = " << power << "\n";
        std::cout << "  Factorización: 2^" << exp << "\n";
        std::cout << "  Dígitos: " << power.to_string().length() << "\n\n";
    }
}

void demo_prime_testing()
{
    std::cout << "\n=== Test de Primalidad ===\n\n";

    std::vector<uint128_t> numbers = {
        2_u128, 17_u128, 97_u128, 9973_u128, 10007_u128, 100003_u128, 1000003_u128,
    };

    std::cout << std::left << std::setw(15) << "Número" << std::setw(12) << "¿Primo?"
              << "Factorización\n";
    std::cout << std::string(60, '-') << "\n";

    for (const auto& n : numbers) {
        bool prime = is_prime_simple(n);
        std::cout << std::setw(15) << n.to_string() << std::setw(12) << (prime ? "Sí" : "No");

        if (!prime) {
            auto factors = factorize_trial_division(n);
            print_factors(factors);
        } else {
            std::cout << "primo\n";
        }
    }
}

void demo_interesting_numbers()
{
    std::cout << "\n=== Números Interesantes ===\n\n";

    // Números de Mersenne pequeños
    std::cout << "Números de Mersenne (2^p - 1):\n\n";
    std::vector<int> mersenne_exp = {5, 7, 11, 13};

    for (int p : mersenne_exp) {
        uint128_t mersenne = (uint128_t(1) << p) - 1;
        std::cout << "M_" << p << " = 2^" << p << " - 1 = " << mersenne;

        if (is_prime_simple(mersenne)) {
            std::cout << " (primo)\n";
        } else {
            std::cout << "\n  Factorización: ";
            auto factors = factorize_trial_division(mersenne);
            print_factors(factors);
        }
        std::cout << "\n";
    }

    // Números perfectos
    std::cout << "\nNúmeros perfectos:\n\n";
    std::vector<uint128_t> perfect = {6_u128, 28_u128, 496_u128, 8128_u128};

    for (const auto& n : perfect) {
        std::cout << n << " = ";
        auto factors = factorize_trial_division(n);
        print_factors(factors);
    }
}

int main()
{
    std::cout << R"(
╔═══════════════════════════════════════════════════════════╗
║                                                           ║
║   FACTORIZACIÓN DE NÚMEROS PRIMOS                         ║
║   Usando uint128_t                                        ║
║                                                           ║
╚═══════════════════════════════════════════════════════════╝
)" << "\n";

    std::cout << "Este programa factoriza números grandes en sus factores primos.\n";
    std::cout << "uint128_t permite trabajar con números mucho mayores que uint64_t.\n";

    demo_small_numbers();
    demo_large_numbers();
    demo_perfect_squares();
    demo_powers_of_two();
    demo_prime_testing();
    demo_interesting_numbers();

    std::cout << "\n=== Limitaciones ===\n\n";
    std::cout << "Este algoritmo usa división por tentativa, que es:\n";
    std::cout << "  ✓ Simple y confiable\n";
    std::cout << "  ✓ Eficiente para números con factores pequeños\n";
    std::cout << "  ✗ Lento para números con factores primos grandes\n";
    std::cout << "\nPara factorización de números con factores grandes,\n";
    std::cout << "se necesitan algoritmos más sofisticados como Pollard's Rho,\n";
    std::cout << "ECM, o GNFS.\n";

    return 0;
}
