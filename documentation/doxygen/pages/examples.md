/**
@page examples Ejemplos de Uso

@tableofcontents

@section examples_intro Ejemplos Prácticos

Esta página contiene ejemplos completos y funcionales de cómo usar la biblioteca int128.

@section examples_basic Ejemplos Básicos

@subsection ex_basic_calc Calculadora Básica

@include demos/tutorials/01_basic_operations.cpp

@subsection ex_conversions Conversiones

@code{.cpp}
#include <uint128/uint128_t.hpp>
#include <iostream>

int main() {
    // De tipos nativos
    uint128_t from_int = 42;
    uint128_t from_long = 1234567890123456ULL;
    
    // A tipos nativos
    uint64_t to_u64 = static_cast<uint64_t>(from_int);
    uint32_t to_u32 = static_cast<uint32_t>(from_int);
    
    // De/a strings
    auto from_str = uint128_t::from_string("999999999999");
    if (from_str) {
        std::string to_str = from_str->to_string();
        std::cout << "String: " << to_str << "\n";
    }
    
    // De/a hex
    auto from_hex = uint128_t::from_string("0xDEADBEEF", 16);
    std::string hex_str = from_hex->to_string(16);
    std::cout << "Hex: " << hex_str << "\n";
    
    return 0;
}
@endcode

@section examples_math Ejemplos Matemáticos

@subsection ex_factorial Factorial

@code{.cpp}
#include <uint128/uint128_t.hpp>
#include <iostream>

uint128_t factorial(unsigned int n) {
    uint128_t result = 1;
    for (unsigned int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

int main() {
    // Factorial de 30 (muy grande para uint64_t)
    auto f30 = factorial(30);
    std::cout << "30! = " << f30 << "\n";
    // Output: 265252859812191058636308480000000
    
    return 0;
}
@endcode

@subsection ex_fibonacci Números de Fibonacci

@code{.cpp}
#include <uint128/uint128_t.hpp>
#include <iostream>
#include <vector>

std::vector<uint128_t> fibonacci(int n) {
    std::vector<uint128_t> fibs;
    fibs.reserve(n);
    
    if (n >= 1) fibs.push_back(0);
    if (n >= 2) fibs.push_back(1);
    
    for (int i = 2; i < n; ++i) {
        fibs.push_back(fibs[i-1] + fibs[i-2]);
    }
    
    return fibs;
}

int main() {
    auto fibs = fibonacci(50);
    
    std::cout << "Primeros 50 números de Fibonacci:\n";
    for (size_t i = 0; i < fibs.size(); ++i) {
        std::cout << "F(" << i << ") = " << fibs[i] << "\n";
    }
    
    return 0;
}
@endcode

@section examples_real_world Casos de Uso Reales

@subsection ex_ipv6 Direcciones IPv6

@code{.cpp}
#include <uint128/uint128_t.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

class IPv6Address {
    uint128_t addr;
    
public:
    IPv6Address(uint128_t address) : addr(address) {}
    
    static IPv6Address from_string(const std::string& ipv6_str) {
        // Parsear formato IPv6 (simplificado)
        // Ejemplo: "2001:0db8:85a3::8a2e:0370:7334"
        // Por simplicidad, aceptamos formato numérico
        auto parsed = uint128_t::from_string(ipv6_str, 16);
        return IPv6Address(parsed.value_or(uint128_t{0}));
    }
    
    std::string to_string() const {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        
        // Extraer grupos de 16 bits
        for (int i = 7; i >= 0; --i) {
            uint16_t group = static_cast<uint16_t>((addr >> (i * 16)).to_uint64());
            oss << std::setw(4) << group;
            if (i > 0) oss << ":";
        }
        
        return oss.str();
    }
    
    IPv6Address subnet_mask(int prefix_len) const {
        uint128_t mask = (uint128_t{1} << prefix_len) - 1;
        mask <<= (128 - prefix_len);
        return IPv6Address(addr & mask);
    }
};

int main() {
    IPv6Address google_dns("20010048860048600000000000008888"); // 2001:4860:4860::8888
    std::cout << "Google DNS: " << google_dns.to_string() << "\n";
    
    auto subnet = google_dns.subnet_mask(64);
    std::cout << "Subnet /64: " << subnet.to_string() << "\n";
    
    return 0;
}
@endcode

@subsection ex_uuid UUIDs (128-bit)

@code{.cpp}
#include <uint128/uint128_t.hpp>
#include <iostream>
#include <random>
#include <iomanip>
#include <sstream>

class UUID {
    uint128_t value;
    
public:
    UUID(uint128_t val) : value(val) {}
    
    static UUID generate() {
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<uint64_t> dis;
        
        uint64_t high = dis(gen);
        uint64_t low = dis(gen);
        
        uint128_t uuid_val = (uint128_t{high} << 64) | low;
        
        // Ajustar para UUID v4
        // Versión 4: bits 12-15 del time_hi_and_version = 0100
        // Variant: bits 6-7 del clock_seq_hi_and_reserved = 10
        
        return UUID(uuid_val);
    }
    
    std::string to_string() const {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        
        auto high = (value >> 64).to_uint64();
        auto low = value.to_uint64();
        
        // Formato: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
        oss << std::setw(8) << (high >> 32)
            << "-" << std::setw(4) << ((high >> 16) & 0xFFFF)
            << "-" << std::setw(4) << (high & 0xFFFF)
            << "-" << std::setw(4) << (low >> 48)
            << "-" << std::setw(12) << (low & 0xFFFFFFFFFFFFULL);
        
        return oss.str();
    }
};

int main() {
    for (int i = 0; i < 5; ++i) {
        UUID uuid = UUID::generate();
        std::cout << "UUID: " << uuid.to_string() << "\n";
    }
    
    return 0;
}
@endcode

@section examples_algorithms Algoritmos y STL

@subsection ex_sorting Ordenamiento

@code{.cpp}
#include <uint128/uint128_t.hpp>
#include <vector>
#include <algorithm>
#include <iostream>

int main() {
    std::vector<uint128_t> numbers = {
        100_u128, 50_u128, 200_u128, 25_u128, 150_u128
    };
    
    std::cout << "Original: ";
    for (const auto& n : numbers) std::cout << n << " ";
    std::cout << "\n";
    
    // Ordenar ascendente
    std::sort(numbers.begin(), numbers.end());
    
    std::cout << "Ordenado: ";
    for (const auto& n : numbers) std::cout << n << " ";
    std::cout << "\n";
    
    // Encontrar mediana
    auto median = numbers[numbers.size() / 2];
    std::cout << "Mediana: " << median << "\n";
    
    return 0;
}
@endcode

@subsection ex_ranges Rangos y Algoritmos

@code{.cpp}
#include <uint128/uint128_ranges.hpp>
#include <iostream>
#include <numeric>

int main() {
    std::vector<uint128_t> nums(10);
    
    // Llenar con secuencia
    std::iota(nums.begin(), nums.end(), 1_u128);
    
    // Sumar todos
    auto sum = std::accumulate(nums.begin(), nums.end(), 0_u128);
    std::cout << "Suma 1-10: " << sum << "\n"; // 55
    
    // Producto
    auto product = std::accumulate(nums.begin(), nums.end(), 1_u128,
                                  std::multiplies<uint128_t>());
    std::cout << "Producto 1-10: " << product << "\n"; // 3628800
    
    return 0;
}
@endcode

@section examples_performance Optimización y Rendimiento

@subsection ex_intrinsics Uso de Intrínsecos

@code{.cpp}
// El código automáticamente usa intrínsecos cuando están disponibles
#include <uint128/uint128_t.hpp>

uint128_t fast_multiply(uint128_t a, uint128_t b) {
    // En x86_64: usa _mulx_u64, _addcarry_u64
    // En otros: usa implementación genérica
    return a * b;
}

uint128_t fast_division(uint128_t dividend, uint128_t divisor) {
    // Optimizado con intrínsecos cuando disponible
    return dividend / divisor;
}
@endcode

@section examples_more Más Ejemplos

Visita el directorio `demos/` para más ejemplos:

- `demos/tutorials/` - Tutoriales paso a paso
- `demos/examples/` - Casos de uso reales completos
- `demos/showcase/` - Demo interactiva con todas las características

*/
