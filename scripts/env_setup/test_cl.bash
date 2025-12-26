#!/usr/bin/env bash
# Test rápido de cl

echo "========================================="
echo "TEST: Ejecutando 'cl' directamente"
echo "========================================="

echo ""
echo "1. Con stdin cerrado (</dev/null):"
echo "-----------------------------------"
cl 2>&1 </dev/null | head -n 5

echo ""
echo "2. Sin stdin cerrado:"
echo "-----------------------------------"
timeout 1 bash -c 'cl 2>&1 | head -n 5' || echo "Timeout"

echo ""
echo "3. type cl:"
echo "-----------------------------------"
type cl

echo ""
echo "4. which cl:"
echo "-----------------------------------"
which cl

echo ""
echo "5. where cl (Windows):"
echo "-----------------------------------"
where cl 2>/dev/null | head -n 3

echo ""
echo "========================================="
echo "Completado"
echo "========================================="
