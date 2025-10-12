// The jmaths library for C++
// Copyright (C) 2025  Jasper de Smaele

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

/**
 * @file TMP.hpp
 * @brief Template Meta-Programming utilities for the jmaths library
 *
 * This file contains compile-time template metaprogramming utilities used throughout
 * the jmaths library. These utilities enable type manipulation, concept checking,
 * and compile-time computations.
 *
 * KEY CONCEPTS:
 * - Type lists (pack_container): Store and manipulate lists of types at compile-time
 * - Type matching: Find types in lists that satisfy predicates
 * - Size comparisons: Compare sizeof types at compile-time
 * - Concepts: C++20 concepts for template constraints
 */

#pragma once

#include <concepts>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace jmaths::TMP {

/**
 * @struct empty_t
 * @brief Sentinel type representing "no type" or "not found"
 * Used as a return value when type searches fail
 */
struct empty_t;

/**
 * @struct pack_container
 * @brief Container for a parameter pack of types
 * @tparam Types Variable number of types to store
 *
 * USAGE: Stores multiple types for manipulation at compile-time
 * Example: pack_container<int, double, char>
 */
template <typename...> struct pack_container;

/**
 * @struct unpack
 * @brief Unpacks the first type from a pack, separating it from the rest
 * @tparam T First type in the pack
 * @tparam U Remaining types
 *
 * USAGE: Enables recursive processing of type lists
 */
template <typename T, typename... U> struct unpack {
    using first_type = T;
    using other_types = pack_container<U...>;
};

/**
 * @struct add_pack_containers
 * @brief Concatenates two pack_containers into one
 *
 * ALGORITHM: Merges two type lists into a single type list
 */
template <typename, typename> struct add_pack_containers;

template <typename... T, typename... U>
struct add_pack_containers<pack_container<T...>, pack_container<U...>> {
    using types = pack_container<T..., U...>;
};

/**
 * @struct all_cv_qualifications
 * @brief Generates all const-volatile variations of a type
 * @tparam T Base type
 *
 * PRODUCES: {T, const T, volatile T, const volatile T}
 * USAGE: Used to match types regardless of cv-qualifiers
 */
template <typename T> struct all_cv_qualifications {
    using used_type = std::decay_t<T>;
    using types = pack_container<used_type,
                                 std::add_const_t<used_type>,
                                 std::add_volatile_t<used_type>,
                                 std::add_cv_t<used_type>>;
};

template <typename T> using all_cv_qualifications_ts = typename all_cv_qualifications<T>::types;

/**
 * @struct for_all_types
 * @brief Applies a template operation to all types in a pack
 * @tparam Op Template template parameter (operation to apply)
 * @tparam T Pack of types to process
 *
 * ALGORITHM: Recursively applies Op to each type and collects results
 */
template <template <typename> class Op, typename T> struct for_all_types;

template <template <typename> class Op, typename... T>
struct for_all_types<Op, pack_container<T...>> {
    using types = typename add_pack_containers<
        typename Op<typename unpack<T...>::first_type>::types,
        typename for_all_types<Op, typename unpack<T...>::other_types>::types>::types;
};

template <template <typename> class Op, typename T> struct for_all_types<Op, pack_container<T>> {
    using types = typename Op<T>::types;
};

template <template <typename> class Op, typename T>
using for_all_types_ts = typename for_all_types<Op, T>::types;

/**
 * @brief Type list containing all primitive types with all cv-qualifications
 * Includes: char, unsigned/signed integral types, floating-point types
 */
using primitive_types = for_all_types_ts<all_cv_qualifications,
                                         pack_container<char,
                                                        unsigned char,
                                                        unsigned short,
                                                        unsigned int,
                                                        unsigned long int,
                                                        unsigned long long int,
                                                        signed char,
                                                        signed short,
                                                        signed int,
                                                        signed long int,
                                                        signed long long int,
                                                        float,
                                                        double,
                                                        long double /*,void, std::nullptr_t*/>>;

/**
 * @brief Type list containing unsigned integral types
 * USAGE: Used for selecting appropriate unsigned types for storage
 */
using unsigned_integral_types =
    pack_container<unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long>;

/**
 * @struct matches_which
 * @brief Finds the first type in a pack that satisfies a binary predicate
 * @tparam T Type to match against
 * @tparam Op Binary predicate template
 * @tparam U Pack of types to search
 *
 * RETURNS: First type U_i where Op<T, U_i>::value is true, or empty_t if none match
 *
 * ALGORITHM: Linear search through type list, testing predicate on each
 */
template <typename T, template <typename, typename> class Op, typename... U> struct matches_which {
    using type = typename matches_which<T, Op, pack_container<U...>>::type;
};

template <typename T, template <typename, typename> class Op, typename... U>
struct matches_which<T, Op, pack_container<U...>> {
    using type =
        std::conditional_t<Op<T, typename unpack<U...>::first_type>::value,
                           typename unpack<U...>::first_type,
                           typename matches_which<T, Op, typename unpack<U...>::other_types>::type>;
};

template <typename T, template <typename, typename> class Op, typename U>
struct matches_which<T, Op, pack_container<U>> {
    using type = std::conditional_t<Op<T, U>::value, U, empty_t>;
};

template <typename T, template <typename, typename> class Op, typename... U>
using matches_which_t = typename matches_which<T, Op, U...>::type;

/**
 * @struct matches_any
 * @brief Checks if any type in pack satisfies predicate with T
 * @returns true if Op<T, U_i>::value is true for any U_i, false otherwise
 */
template <typename T, template <typename, typename> class Op, typename... U> struct matches_any {
    static constexpr bool value = matches_any<T, Op, pack_container<U...>>::value;
};

template <typename T, template <typename, typename> class Op, typename... U>
struct matches_any<T, Op, pack_container<U...>> {
    static constexpr bool value = Op<T, typename unpack<U...>::first_type>::value ?
                                      true :
                                      matches_any<T, Op, typename unpack<U...>::other_types>::value;
};

template <typename T, template <typename, typename> class Op, typename U>
struct matches_any<T, Op, pack_container<U>> {
    static constexpr bool value = Op<T, U>::value ? true : false;
};

template <typename T, template <typename, typename> class Op, typename... U>
inline constexpr bool matches_any_v = matches_any<T, Op, U...>::value;

// Size comparison predicates for types

/**
 * @struct equal_size
 * @brief Checks if two types have equal sizeof
 */
template <typename T, typename U>
struct equal_size : std::conditional_t<(sizeof(T) == sizeof(U)), std::true_type, std::false_type> {};

template <typename T, typename U> inline constexpr bool equal_size_v = equal_size<T, U>::value;

/**
 * @struct greater_size
 * @brief Checks if sizeof(T) > sizeof(U)
 */
template <typename T, typename U>
struct greater_size :
    std::conditional_t<(sizeof(T) > sizeof(U)), std::true_type, std::false_type> {};

template <typename T, typename U> inline constexpr bool greater_size_v = greater_size<T, U>::value;

/**
 * @struct lesser_size
 * @brief Checks if sizeof(T) < sizeof(U)
 */
template <typename T, typename U>
struct lesser_size : std::conditional_t<(sizeof(T) < sizeof(U)), std::true_type, std::false_type> {};

template <typename T, typename U> inline constexpr bool lesser_size_v = lesser_size<T, U>::value;

/**
 * @struct geq_size
 * @brief Checks if sizeof(T) >= sizeof(U)
 */
template <typename T, typename U>
struct geq_size : std::conditional_t<(sizeof(T) >= sizeof(U)), std::true_type, std::false_type> {};

template <typename T, typename U> inline constexpr bool geq_size_v = geq_size<T, U>::value;

/**
 * @struct leq_size
 * @brief Checks if sizeof(T) <= sizeof(U)
 */
template <typename T, typename U>
struct leq_size : std::conditional_t<(sizeof(T) <= sizeof(U)), std::true_type, std::false_type> {};

template <typename T, typename U> inline constexpr bool leq_size_v = leq_size<T, U>::value;

/**
 * @concept primitive_type
 * @brief Checks if T is a primitive type (integral, floating-point, char)
 */
template <typename T>
concept primitive_type =
    !std::is_same_v<typename matches_which<T, std::is_same, primitive_types>::type, empty_t>;

// Validation that primitive_type works correctly
static_assert(primitive_type<int> && primitive_type<char>);
static_assert(primitive_type<char>);

/**
 * @concept any_of
 * @brief Checks if T is the same as any of the types U...
 * USAGE: any_of<int, float, double, int> is true
 */
template <typename T, typename... U>
concept any_of = (std::same_as<T, U> || ...);

/**
 * @struct fake_dependency
 * @brief Creates a dependent type for template contexts
 * USAGE: Used to delay instantiation or create SFINAE contexts
 */
template <typename T, typename...> struct fake_dependency {
    using type = T;
};

template <typename T, typename... Ts> using fake_dependency_t = fake_dependency<T, Ts...>;

/**
 * @concept is_power_of_2
 * @brief Checks if a compile-time value is a power of 2
 * ALGORITHM: Uses bit trick: n & (n-1) == 0 for powers of 2
 */
template <auto n>
concept is_power_of_2 = n != 0 && (n & (n - 1)) == 0;

/**
 * @concept decays_to
 * @brief Checks if T decays to the same type as U
 * USAGE: decays_to<const int&, int> is true
 */
template <typename T, typename U>
concept decays_to = std::same_as<std::decay_t<T>, std::decay_t<U>>;

/**
 * @concept valid_template_params
 * @brief Checks if Ts... are valid parameters for template T
 */
template <template <typename...> class T, typename... Ts>
concept valid_template_params = requires { std::ignore = std::declval<T<Ts...>>(); };

/**
 * @struct same_template_helper
 * @brief Helper for checking if two template instantiations use the same template
 */
template <template <typename...> class, typename> struct same_template_helper;

template <template <typename...> class U, template <typename...> class T, typename... Ts>
struct same_template_helper<U, T<Ts...>> {
    static constexpr bool value =
        valid_template_params<U, Ts...> && std::same_as<T<Ts...>, U<Ts...>>;
};

/**
 * @concept same_template
 * @brief Checks if T is an instantiation of template U
 * USAGE: same_template<std::vector<int>, std::vector> is true
 */
template <typename T, template <typename...> class U>
concept same_template = same_template_helper<U, T>::value;

/**
 * @brief Type alias for reference or copy based on value category
 * ALGORITHM: If T is non-const rvalue reference, use lvalue reference;
 * otherwise use decayed type (copy)
 * USAGE: Optimizes forwarding by avoiding unnecessary copies
 */
template <typename T>
using ref_or_copy_t = std::conditional_t<std::is_rvalue_reference_v<T> and
                                             !std::is_const_v<std::remove_reference_t<T>>,
                                         std::add_lvalue_reference_t<std::decay_t<T>>,
                                         std::decay_t<T>>;

/**
 * @struct type_size
 * @brief Creates a type with specific size
 * USAGE: Used for compile-time size comparisons
 */
template <auto size> struct type_size {
    char _[size];
};

/**
 * @struct is_instance_of
 * @brief Checks if a type is an instance of a template
 */
template <typename, template <typename...> class> struct is_instance_of : std::false_type {};

template <template <typename...> class T, typename... U>
struct is_instance_of<T<U...>, T> : std::true_type {};

template <typename T, template <typename...> class U>
inline constexpr bool is_instance_of_v = is_instance_of<T, U>::value;

/**
 * @concept instance_of
 * @brief Checks if T is an instance of any of the templates U...
 * USAGE: instance_of<std::vector<int>, std::vector, std::list> is true
 */
template <typename T, template <typename...> class... U>
concept instance_of = (is_instance_of_v<std::decay_t<T>, U> || ...);

/**
 * @brief Identity type alias (returns T unchanged)
 */
template <typename T> using id_t = std::type_identity_t<T>;

/**
 * @brief Identity with decay (returns decay_t<T>)
 */
template <typename T> using id_decay_t = id_t<std::decay_t<T>>;

/**
 * @concept decays_to_instance
 * @brief Checks if decay_t<T> is instance of any of U...
 * NOTE: Marked deprecated in favor of using instance_of with decayed type
 */
template <typename T, template <typename...> class... U>
/*[[deprecated]]*/ concept decays_to_instance = instance_of<std::decay_t<T>, U...>;

}  // namespace jmaths::TMP
