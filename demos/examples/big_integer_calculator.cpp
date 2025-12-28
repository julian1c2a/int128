/**
 * ===========================================================================
 * EJEMPLO: Calculadora de Enteros Grandes
 * ===========================================================================
 *
 * Calculadora interactiva que demuestra las capacidades de uint128_t
 * para operaciones con números que exceden uint64_t.
 *
 * Operaciones soportadas:
 * - Aritméticas: +, -, *, /, %, ^
 * - Funciones: factorial, fibonacci, gcd, lcm
 * - Conversiones: dec, hex, oct, bin
 */

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <uint128/uint128_cmath.hpp>
#include <uint128/uint128_iostreams.hpp>
#include <uint128/uint128_t.hpp>

using namespace nstd;

using namespace uint128_literals;

// Cálculos matemáticos
uint128_t factorial(int n)
{
    if (n <= 1)
        return 1;
    uint128_t result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

uint128_t fibonacci(int n)
{
    if (n <= 1)
        return n;
    uint128_t a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        uint128_t temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

uint128_t power(uint128_t base, uint128_t exp)
{
    uint128_t result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result *= base;
        }
        base *= base;
        exp /= 2;
    }
    return result;
}

// Clase calculadora
class BigIntCalculator
{
  private:
    uint128_t memory;

    void print_number(const std::string& label, const uint128_t& num)
    {
        std::cout << "\n" << label << ":\n";
        std::cout << "  Decimal: " << num << "\n";
        std::cout << "  Hex:     0x" << std::hex << num << std::dec << "\n";

        std::string str = num.to_string();
        std::cout << "  Dígitos: " << str.length() << "\n";
    }

  public:
    BigIntCalculator() : memory(0) {}

    void run()
    {
        std::cout << R"(
╔═══════════════════════════════════════════════════════════╗
║        CALCULADORA DE ENTEROS GRANDES (uint128_t)         ║
╚═══════════════════════════════════════════════════════════╝

Comandos disponibles:
  Operaciones básicas:
    add <a> <b>      - Suma
    sub <a> <b>      - Resta
    mul <a> <b>      - Multiplicación
    div <a> <b>      - División
    mod <a> <b>      - Módulo
    pow <base> <exp> - Potencia
  
  Funciones especiales:
    fact <n>         - Factorial
    fib <n>          - Fibonacci
    gcd <a> <b>      - Máximo común divisor
    lcm <a> <b>      - Mínimo común múltiplo
  
  Memoria:
    mem <valor>      - Guardar en memoria
    recall           - Recuperar de memoria
    clear            - Limpiar memoria
  
  Otros:
    help             - Mostrar esta ayuda
    quit / exit      - Salir

Ingrese números en decimal (ej: 12345) o hex (ej: 0xABCD)
)" << "\n";

        std::string line;
        while (true) {
            std::cout << "\n> ";
            std::getline(std::cin, line);

            if (line.empty())
                continue;
            if (line == "quit" || line == "exit")
                break;
            if (line == "help") {
                std::cout << "Ver comandos arriba.\n";
                continue;
            }

            std::istringstream iss(line);
            std::string cmd;
            iss >> cmd;

            try {
                if (cmd == "add") {
                    std::string a_str, b_str;
                    iss >> a_str >> b_str;
                    uint128_t a(a_str.c_str());
                    uint128_t b(b_str.c_str());
                    uint128_t result = a + b;
                    print_number("Resultado", result);
                } else if (cmd == "sub") {
                    std::string a_str, b_str;
                    iss >> a_str >> b_str;
                    uint128_t a(a_str.c_str());
                    uint128_t b(b_str.c_str());
                    uint128_t result = a - b;
                    print_number("Resultado", result);
                } else if (cmd == "mul") {
                    std::string a_str, b_str;
                    iss >> a_str >> b_str;
                    uint128_t a(a_str.c_str());
                    uint128_t b(b_str.c_str());
                    uint128_t result = a * b;
                    print_number("Resultado", result);
                } else if (cmd == "div") {
                    std::string a_str, b_str;
                    iss >> a_str >> b_str;
                    uint128_t a(a_str.c_str());
                    uint128_t b(b_str.c_str());
                    if (b == 0) {
                        std::cout << "Error: División por cero\n";
                    } else {
                        uint128_t result = a / b;
                        print_number("Resultado", result);
                    }
                } else if (cmd == "mod") {
                    std::string a_str, b_str;
                    iss >> a_str >> b_str;
                    uint128_t a(a_str.c_str());
                    uint128_t b(b_str.c_str());
                    uint128_t result = a % b;
                    print_number("Resultado", result);
                } else if (cmd == "pow") {
                    std::string base_str, exp_str;
                    iss >> base_str >> exp_str;
                    uint128_t base(base_str.c_str());
                    uint128_t exp(exp_str.c_str());
                    uint128_t result = power(base, exp);
                    print_number("Resultado", result);
                } else if (cmd == "fact") {
                    int n;
                    iss >> n;
                    if (n < 0 || n > 34) {
                        std::cout << "Factorial solo soporta 0-34 (overflow después)\n";
                    } else {
                        uint128_t result = factorial(n);
                        print_number(std::to_string(n) + "!", result);
                    }
                } else if (cmd == "fib") {
                    int n;
                    iss >> n;
                    if (n < 0 || n > 185) {
                        std::cout << "Fibonacci solo soporta 0-185 (overflow después)\n";
                    } else {
                        uint128_t result = fibonacci(n);
                        print_number("Fibonacci(" + std::to_string(n) + ")", result);
                    }
                } else if (cmd == "gcd") {
                    std::string a_str, b_str;
                    iss >> a_str >> b_str;
                    uint128_t a(a_str.c_str());
                    uint128_t b(b_str.c_str());
                    uint128_t result = nstd::gcd(a, b);
                    print_number("GCD", result);
                } else if (cmd == "lcm") {
                    std::string a_str, b_str;
                    iss >> a_str >> b_str;
                    uint128_t a(a_str.c_str());
                    uint128_t b(b_str.c_str());
                    uint128_t result = nstd::lcm(a, b);
                    print_number("LCM", result);
                } else if (cmd == "mem") {
                    std::string val_str;
                    iss >> val_str;
                    memory = uint128_t(val_str.c_str());
                    std::cout << "✓ Guardado en memoria: " << memory << "\n";
                } else if (cmd == "recall") {
                    print_number("Memoria", memory);
                } else if (cmd == "clear") {
                    memory = 0;
                    std::cout << "✓ Memoria limpiada\n";
                } else {
                    std::cout << "Comando desconocido: " << cmd << "\n";
                    std::cout << "Escribe 'help' para ver comandos disponibles\n";
                }
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }

        std::cout << "\n¡Adiós!\n";
    }
};

void demo_examples()
{
    std::cout << "\n=== Ejemplos de Uso ===\n\n";

    std::cout << "1. Factorial de 30:\n";
    auto f30 = factorial(30);
    std::cout << "   30! = " << f30 << "\n";
    std::cout << "   Dígitos: " << f30.to_string().length() << "\n\n";

    std::cout << "2. Fibonacci(100):\n";
    auto fib100 = fibonacci(100);
    std::cout << "   F(100) = " << fib100 << "\n\n";

    std::cout << "3. Potencia 2^100:\n";
    auto pow2_100 = power(2_u128, 100_u128);
    std::cout << "   2^100 = " << pow2_100 << "\n\n";

    std::cout << "4. GCD y LCM:\n";
    uint128_t a = 123456789012345_u128;
    uint128_t b = 987654321098765_u128;
    std::cout << "   GCD(" << a << ", " << b << ") = " << nstd::gcd(a, b) << "\n";
    std::cout << "   LCM(" << a << ", " << b << ") = " << nstd::lcm(a, b) << "\n";
}

int main(int argc, char* argv[])
{
    if (argc > 1 && std::string(argv[1]) == "--demo") {
        demo_examples();
        return 0;
    }

    BigIntCalculator calc;
    calc.run();

    return 0;
}

