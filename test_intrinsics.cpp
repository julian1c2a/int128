#include <cstdint>
#include <iostream>

// Test simple de intrinsics::add_u64
namespace intrinsics
{
inline constexpr unsigned char addcarry_u64(unsigned char carry_in, uint64_t a, uint64_t b,
                                            uint64_t* result)
{
    uint64_t sum = a + b;
    uint64_t sum_with_carry = sum + carry_in;
    *result = sum_with_carry;
    return (sum < a) || (sum_with_carry < sum) ? 1 : 0;
}

inline constexpr unsigned char add_u64(uint64_t a, uint64_t b, uint64_t* result)
{
    return addcarry_u64(0, a, b, result);
}
} // namespace intrinsics

int main()
{
    uint64_t result = 0;
    unsigned char carry = intrinsics::add_u64(100, 200, &result);

    std::cout << "100 + 200 = " << result << ", carry = " << (int)carry << std::endl;
    std::cout << "Expected: result = 300, carry = 0" << std::endl;

    return 0;
}
