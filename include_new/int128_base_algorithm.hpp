/*
 * Boost Software License - Version 1.0 - August 17th, 2003
 */

#ifndef INT128_BASE_ALGORITHM_HPP
#define INT128_BASE_ALGORITHM_HPP

/**
 * @file int128_base_algorithm.hpp
 * @brief Algoritmos para int128_base_t<S>
 * @author Julián Calderón Almendros <julian.calderon.almendros@gmail.com>
 * @version 1.0.0
 * @date 2026-01-05
 * @copyright Boost Software License 1.0
 *
 * Proporciona algoritmos STL-compatibles para int128_base_t<signedness S>:
 * - Búsqueda: binary_search, find_if, lower_bound, upper_bound
 * - Transformación: transform, for_each
 * - Reducción: accumulate, sum, product
 * - Partición: partition, stable_partition
 * - Ordenamiento: sort, partial_sort, nth_element
 * - Especializados: gcd_range, lcm_range, generate_sequence
 */

#include "int128_base_numeric.hpp"
#include "int128_base_tt.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <type_traits>
#include <vector>

namespace nstd
{

/**
 * @brief Concepto para predicados sobre int128_base_t
 */
template <typename P, signedness S>
concept int128_predicate = requires(P pred, int128_base_t<S> val) {
    { pred(val) } -> std::convertible_to<bool>;
};

/**
 * @brief Concepto para funciones sobre int128_base_t
 */
template <typename F, signedness S>
concept int128_function = requires(F func, int128_base_t<S> val) { func(val); };

/**
 * @brief Concepto para operaciones binarias sobre int128_base_t
 */
template <typename Op, signedness S>
concept int128_binary_op = requires(Op op, int128_base_t<S> a, int128_base_t<S> b) {
    { op(a, b) } -> std::same_as<int128_base_t<S>>;
};

// =============================================================================
// ALGORITMOS DE BÚSQUEDA
// =============================================================================

/**
 * @brief Búsqueda binaria en rango ordenado
 */
template <signedness S, std::forward_iterator ForwardIt>
bool binary_search_128(ForwardIt first, ForwardIt last, const int128_base_t<S>& value)
    requires std::same_as<typename std::iterator_traits<ForwardIt>::value_type, int128_base_t<S>>
{
    first = std::lower_bound(first, last, value);
    return (first != last) && (*first == value);
}

/**
 * @brief Encuentra primer elemento que satisface predicado
 */
template <signedness S, std::forward_iterator ForwardIt, typename Pred>
    requires int128_predicate<Pred, S> &&
             std::same_as<typename std::iterator_traits<ForwardIt>::value_type, int128_base_t<S>>
ForwardIt find_if_128(ForwardIt first, ForwardIt last, Pred pred)
{
    return std::find_if(first, last, pred);
}

/**
 * @brief Cuenta elementos que satisfacen predicado
 */
template <signedness S, std::forward_iterator ForwardIt, typename Pred>
    requires int128_predicate<Pred, S> &&
             std::same_as<typename std::iterator_traits<ForwardIt>::value_type, int128_base_t<S>>
typename std::iterator_traits<ForwardIt>::difference_type count_if_128(ForwardIt first,
                                                                       ForwardIt last, Pred pred)
{
    return std::count_if(first, last, pred);
}

/**
 * @brief Lower bound en rango ordenado
 */
template <signedness S, std::forward_iterator ForwardIt>
ForwardIt lower_bound_128(ForwardIt first, ForwardIt last, const int128_base_t<S>& value)
    requires std::same_as<typename std::iterator_traits<ForwardIt>::value_type, int128_base_t<S>>
{
    return std::lower_bound(first, last, value);
}

/**
 * @brief Upper bound en rango ordenado
 */
template <signedness S, std::forward_iterator ForwardIt>
ForwardIt upper_bound_128(ForwardIt first, ForwardIt last, const int128_base_t<S>& value)
    requires std::same_as<typename std::iterator_traits<ForwardIt>::value_type, int128_base_t<S>>
{
    return std::upper_bound(first, last, value);
}

// =============================================================================
// ALGORITMOS DE TRANSFORMACIÓN
// =============================================================================

/**
 * @brief Transforma un rango aplicando una operación
 */
template <signedness S, std::input_iterator InputIt,
          std::output_iterator<int128_base_t<S>> OutputIt, typename UnaryOp>
OutputIt transform_128(InputIt first, InputIt last, OutputIt result, UnaryOp op)
    requires std::same_as<typename std::iterator_traits<InputIt>::value_type, int128_base_t<S>>
{
    return std::transform(first, last, result, op);
}

/**
 * @brief Aplica función a cada elemento
 */
template <signedness S, std::forward_iterator ForwardIt, typename Function>
    requires int128_function<Function, S> &&
             std::same_as<typename std::iterator_traits<ForwardIt>::value_type, int128_base_t<S>>
void for_each_128(ForwardIt first, ForwardIt last, Function f)
{
    std::for_each(first, last, f);
}

// =============================================================================
// ALGORITMOS DE REDUCCIÓN
// =============================================================================

/**
 * @brief Acumula valores con operación binaria
 */
template <signedness S, std::input_iterator InputIt, typename BinaryOp>
    requires int128_binary_op<BinaryOp, S> &&
             std::same_as<typename std::iterator_traits<InputIt>::value_type, int128_base_t<S>>
int128_base_t<S> accumulate_128(InputIt first, InputIt last, int128_base_t<S> init, BinaryOp op)
{
    return std::accumulate(first, last, init, op);
}

/**
 * @brief Suma de todos los elementos
 */
template <signedness S, std::input_iterator InputIt>
int128_base_t<S> sum_128(InputIt first, InputIt last)
    requires std::same_as<typename std::iterator_traits<InputIt>::value_type, int128_base_t<S>>
{
    return std::accumulate(first, last, int128_base_t<S>(0));
}

/**
 * @brief Producto de todos los elementos
 */
template <signedness S, std::input_iterator InputIt>
int128_base_t<S> product_128(InputIt first, InputIt last)
    requires std::same_as<typename std::iterator_traits<InputIt>::value_type, int128_base_t<S>>
{
    return std::accumulate(
        first, last, int128_base_t<S>(1),
        [](const int128_base_t<S>& a, const int128_base_t<S>& b) { return a * b; });
}

// =============================================================================
// ALGORITMOS DE ORDENAMIENTO
// =============================================================================

/**
 * @brief Ordena un rango
 */
template <signedness S, std::random_access_iterator RandomIt>
void sort_128(RandomIt first, RandomIt last)
    requires std::same_as<typename std::iterator_traits<RandomIt>::value_type, int128_base_t<S>>
{
    std::sort(first, last);
}

/**
 * @brief Ordena un rango con comparador personalizado
 */
template <signedness S, std::random_access_iterator RandomIt, typename Compare>
void sort_128(RandomIt first, RandomIt last, Compare comp)
    requires std::same_as<typename std::iterator_traits<RandomIt>::value_type, int128_base_t<S>>
{
    std::sort(first, last, comp);
}

/**
 * @brief Ordena parcialmente
 */
template <signedness S, std::random_access_iterator RandomIt>
void partial_sort_128(RandomIt first, RandomIt middle, RandomIt last)
    requires std::same_as<typename std::iterator_traits<RandomIt>::value_type, int128_base_t<S>>
{
    std::partial_sort(first, middle, last);
}

/**
 * @brief nth_element
 */
template <signedness S, std::random_access_iterator RandomIt>
void nth_element_128(RandomIt first, RandomIt nth, RandomIt last)
    requires std::same_as<typename std::iterator_traits<RandomIt>::value_type, int128_base_t<S>>
{
    std::nth_element(first, nth, last);
}

// =============================================================================
// ALGORITMOS DE PARTICIÓN
// =============================================================================

/**
 * @brief Particiona según predicado
 */
template <signedness S, std::forward_iterator ForwardIt, typename Pred>
    requires int128_predicate<Pred, S> &&
             std::same_as<typename std::iterator_traits<ForwardIt>::value_type, int128_base_t<S>>
ForwardIt partition_128(ForwardIt first, ForwardIt last, Pred pred)
{
    return std::partition(first, last, pred);
}

/**
 * @brief Partición estable
 */
template <signedness S, std::bidirectional_iterator BidirIt, typename Pred>
    requires int128_predicate<Pred, S> &&
             std::same_as<typename std::iterator_traits<BidirIt>::value_type, int128_base_t<S>>
BidirIt stable_partition_128(BidirIt first, BidirIt last, Pred pred)
{
    return std::stable_partition(first, last, pred);
}

// =============================================================================
// ALGORITMOS ESPECIALIZADOS
// =============================================================================

/**
 * @brief GCD de todos los elementos en un rango
 */
template <signedness S, std::input_iterator InputIt>
int128_base_t<S> gcd_range(InputIt first, InputIt last)
    requires std::same_as<typename std::iterator_traits<InputIt>::value_type, int128_base_t<S>>
{
    if (first == last)
        return int128_base_t<S>(0);

    int128_base_t<S> result = *first++;
    while (first != last) {
        result = gcd(result, *first++);
        if (result == int128_base_t<S>(1))
            break;
    }
    return result;
}

/**
 * @brief LCM de todos los elementos en un rango
 */
template <signedness S, std::input_iterator InputIt>
int128_base_t<S> lcm_range(InputIt first, InputIt last)
    requires std::same_as<typename std::iterator_traits<InputIt>::value_type, int128_base_t<S>>
{
    if (first == last)
        return int128_base_t<S>(0);

    int128_base_t<S> result = *first++;
    while (first != last) {
        result = lcm(result, *first++);
    }
    return result;
}

/**
 * @brief Genera secuencia aritmética
 */
template <signedness S, std::output_iterator<int128_base_t<S>> OutputIt>
OutputIt generate_arithmetic_sequence(OutputIt first, std::size_t count, int128_base_t<S> start,
                                      int128_base_t<S> step)
{
    for (std::size_t i = 0; i < count; ++i) {
        *first++ = start;
        start += step;
    }
    return first;
}

/**
 * @brief Genera secuencia geométrica
 */
template <signedness S, std::output_iterator<int128_base_t<S>> OutputIt>
OutputIt generate_geometric_sequence(OutputIt first, std::size_t count, int128_base_t<S> start,
                                     int128_base_t<S> ratio)
{
    for (std::size_t i = 0; i < count; ++i) {
        *first++ = start;
        start *= ratio;
    }
    return first;
}

/**
 * @brief Iota: llena con valores incrementales
 */
template <signedness S, std::forward_iterator ForwardIt>
void iota_128(ForwardIt first, ForwardIt last, int128_base_t<S> value)
    requires std::same_as<typename std::iterator_traits<ForwardIt>::value_type, int128_base_t<S>>
{
    while (first != last) {
        *first++ = value++;
    }
}

/**
 * @brief Calcula mínimo y máximo simultáneamente
 */
template <signedness S, std::input_iterator InputIt>
std::pair<int128_base_t<S>, int128_base_t<S>> minmax_128(InputIt first, InputIt last)
    requires std::same_as<typename std::iterator_traits<InputIt>::value_type, int128_base_t<S>>
{
    if (first == last) {
        return {int128_base_t<S>(0), int128_base_t<S>(0)};
    }

    int128_base_t<S> min_val = *first;
    int128_base_t<S> max_val = *first;
    ++first;

    while (first != last) {
        if (*first < min_val)
            min_val = *first;
        if (*first > max_val)
            max_val = *first;
        ++first;
    }

    return {min_val, max_val};
}

/**
 * @brief Estadísticas básicas (min, max, sum, count)
 */
template <signedness S> struct basic_stats {
    int128_base_t<S> min_val;
    int128_base_t<S> max_val;
    int128_base_t<S> sum;
    std::size_t count;
};

template <signedness S, std::input_iterator InputIt>
basic_stats<S> calculate_stats(InputIt first, InputIt last)
    requires std::same_as<typename std::iterator_traits<InputIt>::value_type, int128_base_t<S>>
{
    if (first == last) {
        return {int128_base_t<S>(0), int128_base_t<S>(0), int128_base_t<S>(0), 0};
    }

    int128_base_t<S> min_val = *first;
    int128_base_t<S> max_val = *first;
    int128_base_t<S> sum = int128_base_t<S>(0);
    std::size_t count = 0;

    while (first != last) {
        if (*first < min_val)
            min_val = *first;
        if (*first > max_val)
            max_val = *first;
        sum += *first;
        ++count;
        ++first;
    }

    return {min_val, max_val, sum, count};
}

// =============================================================================
// NAMESPACES DE COMPATIBILIDAD
// =============================================================================

namespace uint128_algorithm
{
using nstd::accumulate_128;
using nstd::basic_stats;
using nstd::binary_search_128;
using nstd::calculate_stats;
using nstd::count_if_128;
using nstd::find_if_128;
using nstd::for_each_128;
using nstd::gcd_range;
using nstd::generate_arithmetic_sequence;
using nstd::generate_geometric_sequence;
using nstd::iota_128;
using nstd::lcm_range;
using nstd::lower_bound_128;
using nstd::minmax_128;
using nstd::nth_element_128;
using nstd::partial_sort_128;
using nstd::partition_128;
using nstd::product_128;
using nstd::sort_128;
using nstd::stable_partition_128;
using nstd::sum_128;
using nstd::transform_128;
using nstd::upper_bound_128;
} // namespace uint128_algorithm

namespace int128_algorithm
{
using nstd::accumulate_128;
using nstd::basic_stats;
using nstd::binary_search_128;
using nstd::calculate_stats;
using nstd::count_if_128;
using nstd::find_if_128;
using nstd::for_each_128;
using nstd::gcd_range;
using nstd::generate_arithmetic_sequence;
using nstd::generate_geometric_sequence;
using nstd::iota_128;
using nstd::lcm_range;
using nstd::lower_bound_128;
using nstd::minmax_128;
using nstd::nth_element_128;
using nstd::partial_sort_128;
using nstd::partition_128;
using nstd::product_128;
using nstd::sort_128;
using nstd::stable_partition_128;
using nstd::sum_128;
using nstd::transform_128;
using nstd::upper_bound_128;
} // namespace int128_algorithm

} // namespace nstd

#endif // INT128_BASE_ALGORITHM_HPP
