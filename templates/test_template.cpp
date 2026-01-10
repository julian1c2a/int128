// =============================================================================
// Test: BRIEF_DESCRIPTION
// Part of int128 Library - https://github.com/julian-calderon/int128
// License: BSL-1.0
// =============================================================================

#include <iostream>
#include <cassert>

#include "int128_base_tt.hpp"

using nstd::int128_t;
using nstd::uint128_t;

// =============================================================================
// Test Cases
// =============================================================================

void test_case_1()
{
    std::cout << "Test 1: DESCRIPTION... ";

    // Arrange
    const uint128_t x{42};

    // Act
    const auto result = x.to_string();

    // Assert
    assert(result == "42");

    std::cout << "[OK]" << std::endl;
}

// =============================================================================
// Main
// =============================================================================

int main()
{
    std::cout << "=== TEST_NAME ===" << std::endl;

    test_case_1();

    std::cout << "\nAll tests passed!" << std::endl;
    return 0;
}
