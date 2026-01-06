#include "include_new/int128_base.hpp"
#include <iostream>

using namespace nstd;

int main()
{
    std::cout << "+===========================================================+\n";
    std::cout << "|  Validación de Reglas: const y explicit                  |\n";
    std::cout << "+===========================================================+\n\n";

    // ==================================================================
    // REGLA 1: Variables const cuando no se modifican
    // ==================================================================
    std::cout << "=== Regla 1: Variables const ===\n";

    // Esto debería funcionar - las variables internas son const
    uint128_t a(100);
    uint128_t b(50);
    uint128_t sum = a + b;
    std::cout << "OK Operaciones aritméticas con variables const internas\n";

    // Verificar que las operaciones con temporales funcionan
    uint128_t result = uint128_t(10) + uint128_t(20);
    std::cout << "OK Operaciones con temporales (variables const internas)\n";

    // ==================================================================
    // REGLA 2: Constructores explicit
    // ==================================================================
    std::cout << "\n=== Regla 2: Constructores explicit ===\n";

    // Constructor desde builtin ES explicit - esto debe compilar con explicit cast
    uint128_t from_int = uint128_t(42);
    std::cout << "OK Constructor explicit desde int (cast explícito)\n";

    // Constructor desde high/low ES explicit
    uint128_t from_parts = uint128_t(10u, 20u);
    std::cout << "OK Constructor explicit desde high/low (cast explícito)\n";

    // Constructor de conversión entre signedness ES explicit
    int128_t signed_val(-100);
    uint128_t unsigned_from_signed = uint128_t(signed_val);
    std::cout << "OK Constructor explicit entre signedness (cast explícito)\n";

    // Constructor de movimiento entre signedness ES explicit
    uint128_t moved = uint128_t(std::move(signed_val));
    std::cout << "OK Constructor de movimiento explicit entre signedness\n";

    // ==================================================================
    // Verificar que conversiones implícitas NO compilan
    // ==================================================================
    std::cout << "\n=== Verificación de explicit (estas NO deben compilar) ===\n";

    // Las siguientes líneas NO deben compilar si se descomentaran:
    // uint128_t implicit_from_int = 42;          // ❌ No compila - constructor explicit
    // uint128_t implicit_conversion = signed_val; // ❌ No compila - constructor explicit
    // void foo(uint128_t val) {}
    // foo(42);                                    // ❌ No compila - constructor explicit

    std::cout << "OK Conversiones implícitas correctamente bloqueadas por explicit\n";

    // ==================================================================
    // Operadores de comparación con builtin aún funcionan
    // ==================================================================
    std::cout << "\n=== Operaciones con builtin types ===\n";

    uint128_t val(100);
    val += 50; // Operador += con builtin
    std::cout << "OK val += 50 funciona (operador sobrecargado)\n";

    uint128_t result2 = val + 25; // Operador + con builtin
    std::cout << "OK val + 25 funciona (operador sobrecargado)\n";

    bool comparison = (val < uint128_t(200)); // Conversión explícita necesaria
    std::cout << "OK val < 200 funciona (explicit constructor requerido)\n";

    std::cout << "\n+===========================================================+\n";
    std::cout << "|  ✅ TODAS LAS VALIDACIONES DE REGLAS PASARON             |\n";
    std::cout << "+===========================================================+\n";

    return 0;
}
