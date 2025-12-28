/**
 * ============================================================================
 * INT128 LIBRARY - SHOWCASE DEMO
 * ============================================================================
 *
 * Esta demo muestra TODAS las capacidades de la biblioteca int128:
 * - Operaciones aritméticas con números gigantes
 * - Operaciones bitwise avanzadas
 * - Funciones matemáticas (gcd, lcm, factoriales)
 * - Algoritmos STL
 * - Thread safety
 * - Safe operations con detección de overflow
 * - User-defined literals
 * - Formateo en múltiples bases
 *
 * ¡Prepárate para números REALMENTE grandes! 🚀
 */

#include <algorithm>
#include <chrono>
#include <cmath>
#include <int128.hpp>
#include <int128/int128_iostreams.hpp>
#include <iomanip>
#include <iostream>
#include <uint128/uint128_iostreams.hpp>
#include <vector>

using namespace nstd;

using namespace uint128_literals;
using namespace int128_literals;

// ANSI color codes para output atractivo
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

void print_header(const std::string& title)
{
    std::cout << "\n" << BOLD << CYAN;
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║ " << std::setw(60) << std::left << title << " ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    std::cout << RESET << "\n";
}

void print_section(const std::string& section)
{
    std::cout << "\n" << BOLD << YELLOW << ">>> " << section << RESET << "\n\n";
}

void print_result(const std::string& label, const auto& value)
{
    std::cout << GREEN << "  ✓ " << RESET << label << ": " << BOLD << value << RESET << "\n";
}

void print_warning(const std::string& msg)
{
    std::cout << YELLOW << "  ⚠  " << msg << RESET << "\n";
}

void print_error(const std::string& msg)
{
    std::cout << RED << "  ✗ " << msg << RESET << "\n";
}

void pause()
{
    std::cout << "\n" << BLUE << "Presiona Enter para continuar..." << RESET;
    std::cin.ignore();
    std::cin.get();
}

// ============================================================================
// DEMO 1: NÚMEROS GIGANTES
// ============================================================================

void demo_huge_numbers()
{
    print_header("DEMO 1: Números Realmente GRANDES");

    print_section("Factoriales");

    // Factorial de 34 (el más grande que cabe en uint128_t)
    uint128_t factorial = 1;
    for (int i = 2; i <= 34; ++i) {
        factorial *= i;
    }

    print_result("34!", factorial);
    std::cout << "  Dígitos: " << factorial.to_string().length() << "\n";

    print_section("Fibonacci");

    // Fibonacci hasta que no quepa en uint128_t
    std::vector<uint128_t> fib = {0_u128, 1_u128};

    while (fib.size() < 186) { // F(186) es el último que cabe
        auto next = fib[fib.size() - 1] + fib[fib.size() - 2];
        if (next < fib[fib.size() - 1])
            break; // Overflow
        fib.push_back(next);
    }

    print_result("Fibonacci(185)", fib.back());
    std::cout << "  Calculados: " << fib.size() << " términos\n";

    print_section("Potencias de 2");

    std::cout << "  2^64  = " << (1_u128 << 64) << "\n";
    std::cout << "  2^100 = " << (1_u128 << 100) << "\n";
    std::cout << "  2^127 = " << (1_u128 << 127) << "\n";

    pause();
}

// ============================================================================
// DEMO 2: OPERACIONES BITWISE AVANZADAS
// ============================================================================

void demo_bitwise_magic()
{
    print_header("DEMO 2: Magia Bitwise");

    print_section("Contar bits activados (popcount)");

    auto popcount = [](uint128_t n) -> int {
        int count = 0;
        while (n) {
            count += (n & 1) != 0;
            n >>= 1;
        }
        return count;
    };

    uint128_t num = 0xFFFFFFFFFFFFFFFF_u128; // 64 bits en 1
    std::cout << GREEN << "  ✓ " << RESET << "Número: " << BOLD << std::hex << std::showbase << num
              << std::dec << std::noshowbase << RESET << "\n";
    print_result("Bits activos", popcount(num));

    print_section("Encontrar el bit más alto (MSB)");

    auto msb_position = [](uint128_t n) -> int {
        if (n == 0)
            return -1;
        int pos = 0;
        while (n >>= 1)
            ++pos;
        return pos;
    };

    uint128_t big = (1_u128 << 100) | (1_u128 << 50) | 1;
    print_result("Número", big);
    print_result("Posición MSB", msb_position(big));

    print_section("Swap usando XOR (sin variable temporal)");

    uint128_t x = 12345_u128;
    uint128_t y = 67890_u128;

    std::cout << "  Antes:   x = " << x << ", y = " << y << "\n";

    x ^= y;
    y ^= x;
    x ^= y;

    std::cout << "  Después: x = " << x << ", y = " << y << "\n";

    pause();
}

// ============================================================================
// DEMO 3: FUNCIONES MATEMÁTICAS
// ============================================================================

void demo_math_functions()
{
    print_header("DEMO 3: Funciones Matemáticas");

    print_section("GCD (Máximo Común Divisor)");

    auto a = 48_u128;
    auto b = 18_u128;
    auto gcd_result = nstd::gcd(a, b);

    std::cout << "  gcd(" << a << ", " << b << ") = " << gcd_result << "\n";

    // GCD de números grandes
    auto big_a = 123456789012345_u128;
    auto big_b = 987654321098765_u128;
    auto gcd_big = nstd::gcd(big_a, big_b);

    print_result("gcd(números grandes)", gcd_big);

    print_section("LCM (Mínimo Común Múltiplo)");

    auto lcm_result = nstd::lcm(a, b);
    std::cout << "  lcm(" << a << ", " << b << ") = " << lcm_result << "\n";

    print_section("Raíz cuadrada");

    auto sqrt_impl = [](uint128_t n) -> uint128_t {
        if (n == 0)
            return 0;
        uint128_t x = n;
        uint128_t y = (x + 1) / 2;
        while (y < x) {
            x = y;
            y = (x + n / x) / 2;
        }
        return x;
    };

    uint128_t square = 1000000_u128 * 1000000_u128;
    auto root = sqrt_impl(square);

    print_result("sqrt(1000000²)", root);
    print_result("Verificación", root * root);

    pause();
}

// ============================================================================
// DEMO 4: ALGORITMOS STL
// ============================================================================

void demo_stl_algorithms()
{
    print_header("DEMO 4: Algoritmos STL");

    print_section("Ordenamiento de números grandes");

    std::vector<uint128_t> numbers = {1000000000000_u128, 500000000000_u128, 2000000000000_u128,
                                      750000000000_u128, 100000000000_u128};

    std::cout << "  Antes:   ";
    for (const auto& n : numbers)
        std::cout << n << " ";
    std::cout << "\n";

    std::sort(numbers.begin(), numbers.end());

    std::cout << "  Después: ";
    for (const auto& n : numbers)
        std::cout << n << " ";
    std::cout << "\n";

    print_section("Accumulate (suma total)");

    auto total = std::accumulate(numbers.begin(), numbers.end(), 0_u128);
    print_result("Suma total", total);

    print_section("Transform (doblar cada valor)");

    std::vector<uint128_t> doubled;
    std::transform(numbers.begin(), numbers.end(), std::back_inserter(doubled),
                   [](uint128_t n) { return n * 2; });

    std::cout << "  Doblados: ";
    for (const auto& n : doubled)
        std::cout << n << " ";
    std::cout << "\n";

    pause();
}

// ============================================================================
// DEMO 5: OPERACIONES SEGURAS
// ============================================================================

void demo_safe_operations()
{
    print_header("DEMO 5: Operaciones Seguras");

    print_section("Detección de Overflow");

    auto safe_mul = [](uint128_t a, uint128_t b) -> std::optional<uint128_t> {
        if (a == 0 || b == 0)
            return uint128_t(0);
        uint128_t result = a * b;
        if (result / a != b)
            return std::nullopt;
        return result;
    };

    uint128_t max = std::numeric_limits<uint128_t>::max();

    auto result1 = safe_mul(1000_u128, 2000_u128);
    auto result2 = safe_mul(max, 2_u128);

    if (result1) {
        print_result("1000 × 2000", *result1);
    }

    if (!result2) {
        print_error("max × 2 = OVERFLOW detectado!");
    }

    print_section("División segura");

    auto safe_div = [](uint128_t a, uint128_t b) -> std::optional<uint128_t> {
        if (b == 0)
            return std::nullopt;
        return a / b;
    };

    auto div1 = safe_div(1000_u128, 10_u128);
    auto div2 = safe_div(1000_u128, 0_u128);

    if (div1) {
        print_result("1000 ÷ 10", *div1);
    }

    if (!div2) {
        print_error("1000 ÷ 0 = División por cero detectada!");
    }

    pause();
}

// ============================================================================
// DEMO 6: RENDIMIENTO
// ============================================================================

void demo_performance()
{
    print_header("DEMO 6: Benchmark de Rendimiento");

    print_section("Velocidad de operaciones");

    const int iterations = 1000000;

    // Suma
    {
        auto start = std::chrono::high_resolution_clock::now();
        uint128_t sum = 0;
        for (int i = 0; i < iterations; ++i) {
            sum += i;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "  Suma:            " << duration.count() << " μs (" << iterations
                  << " iteraciones)\n";
        std::cout << "  Resultado final: " << sum << "\n";
    }

    // Multiplicación
    {
        auto start = std::chrono::high_resolution_clock::now();
        uint128_t product = 1;
        for (int i = 1; i < 100; ++i) {
            product = (product * i) % 1000000007;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "  Multiplicación:  " << duration.count() << " μs\n";
    }

    pause();
}

// ============================================================================
// DEMO 7: FORMATEO Y REPRESENTACIÓN
// ============================================================================

void demo_formatting()
{
    print_header("DEMO 7: Formateo y Representación");

    uint128_t num = 0xDEADBEEFCAFEBABE_u128;

    print_section("Múltiples bases");

    std::cout << "  Decimal:     " << std::dec << num << "\n";
    std::cout << "  Hexadecimal: " << std::hex << std::showbase << std::uppercase << num << "\n";
    std::cout << "  Octal:       " << std::oct << num << "\n";
    std::cout << std::dec << std::noshowbase << std::nouppercase;

    print_section("Tabla formateada");

    struct Entry {
        std::string name;
        uint128_t value;
    };

    std::vector<Entry> table = {{"Millón", 1000000_u128},
                                {"Billón", 1000000000000_u128},
                                {"Trillón", 1000000_u128 * 1000000_u128 * 1000000_u128}};

    std::cout << "  " << std::left << std::setw(12) << "Nombre" << std::right << std::setw(30)
              << "Valor" << "\n";
    std::cout << "  " << std::string(42, '-') << "\n";

    for (const auto& entry : table) {
        std::cout << "  " << std::left << std::setw(12) << entry.name << std::right << std::setw(30)
                  << entry.value << "\n";
    }

    pause();
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    // Banner principal
    std::cout << BOLD << MAGENTA;
    std::cout << R"(
    ╔════════════════════════════════════════════════════════════════╗
    ║                                                                ║
    ║   ██╗███╗   ██╗████████╗ ██╗██████╗  █████╗     ██╗     ██╗██╗
║   ██║████╗  ██║╚══██╔══╝███║╚════██╗██╔══██╗    ██║     ██║██║    ║
    ║   ██║██╔██╗ ██║   ██║   ╚██║ █████╔╝╚█████╔╝    ██║     ██║██║    ║
    ║   ██║██║╚██╗██║   ██║    ██║██╔═══╝ ██╔══██╗    ██║     ██║██║    ║
    ║   ██║██║ ╚████║   ██║    ██║███████╗╚█████╔╝    ███████╗██║███████║
    ║   ╚═╝╚═╝  ╚═══╝   ╚═╝    ╚═╝╚══════╝ ╚════╝     ╚══════╝╚═╝╚══════╝
    ║                                                                ║
    ║                  SHOWCASE DEMO - 128-bit Integers              ║
    ║                                                                ║
    ╚════════════════════════════════════════════════════════════════╝
)" << RESET << "\n";

    std::cout << BOLD << "Esta demo muestra las capacidades completas de la biblioteca int128.\n";
    std::cout << "Cada sección demuestra características diferentes.\n" << RESET;

    pause();

    // Ejecutar demos
    demo_huge_numbers();
    demo_bitwise_magic();
    demo_math_functions();
    demo_stl_algorithms();
    demo_safe_operations();
    demo_performance();
    demo_formatting();

    // Final
    std::cout << "\n" << BOLD << GREEN;
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                   🎉 DEMO COMPLETADA 🎉                        ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n";
    std::cout << RESET << "\n";

    std::cout << "Explora más:\n";
    std::cout << "  • Tutoriales: demos/tutorials/\n";
    std::cout << "  • Ejemplos:   demos/examples/\n";
    std::cout << "  • Docs:       documentation/generated/html/\n\n";

    return 0;
}

