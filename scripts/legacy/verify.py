#!/usr/bin/env python3

# Verificar el cálculo manualmente
num_128 = (2**128) - 1
print(f"Número de 128 bits: {hex(num_128)}")

result = num_128 * 2
print(f"Resultado: {hex(result)}")

# Los bits 128-191 (parte alta de 192 bits)
bits_128_191 = result >> 128
print(f"Bits 128-191: {hex(bits_128_191)} = {bits_128_191}")

# Verificar: el resultado debe ser 2^129 - 2
expected = (2**129) - 2
print(f"Esperado 2^129 - 2: {hex(expected)}")
print(f"¿Son iguales?: {result == expected}")