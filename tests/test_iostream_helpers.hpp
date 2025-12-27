// test_iostream_helpers.hpp
// Header auxiliar para tests que necesitan soporte de iostream básico
// Si el header completo uint128_iostreams.hpp está incluido, esto no hace nada

#ifndef TEST_IOSTREAM_HELPERS_HPP
#define TEST_IOSTREAM_HELPERS_HPP

#include "uint128/uint128_t.hpp"
#include <iostream>
#include <sstream>

// Solo definir operator<< si no está incluido el header completo
#ifndef UINT128_IOSTREAMS_AVAILABLE

// Versión simplificada de operator<< para tests (en scope global, no anónimo)
inline std::ostream& operator<<(std::ostream& os, const uint128_t& value)
{
    return os << value.to_string();
}

inline std::ostream& operator<<(std::ostream& os, const int128_t& value)
{
    return os << value.to_string();
}

#endif // !UINT128_IOSTREAMS_AVAILABLE

#endif // TEST_IOSTREAM_HELPERS_HPP
