// The jmaths library for C++
// Copyright (C) 2024  Jasper de Smaele

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

#pragma once

#include <concepts>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace jmaths::TMP {

struct empty_t;

template <typename...> struct pack_container;

template <typename T, typename... U> struct unpack {
    using first_type = T;
    using other_types = pack_container<U...>;
};

template <typename, typename> struct add_pack_containers;

template <typename... T, typename... U>
struct add_pack_containers<pack_container<T...>, pack_container<U...>> {
    using types = pack_container<T..., U...>;
};

template <typename T> struct all_cv_qualifications {
    using used_type = std::decay_t<T>;
    using types = pack_container<used_type,
                                 std::add_const_t<used_type>,
                                 std::add_volatile_t<used_type>,
                                 std::add_cv_t<used_type>>;
};

template <typename T> using all_cv_qualifications_ts = typename all_cv_qualifications<T>::types;

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

using unsigned_integral_types =
    pack_container<unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long>;

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

template <typename T, typename U>
struct equal_size : std::conditional_t<(sizeof(T) == sizeof(U)), std::true_type, std::false_type> {};

template <typename T, typename U> inline constexpr bool equal_size_v = equal_size<T, U>::value;

template <typename T, typename U>
struct greater_size :
    std::conditional_t<(sizeof(T) > sizeof(U)), std::true_type, std::false_type> {};

template <typename T, typename U> inline constexpr bool greater_size_v = greater_size<T, U>::value;

template <typename T, typename U>
struct lesser_size : std::conditional_t<(sizeof(T) < sizeof(U)), std::true_type, std::false_type> {};

template <typename T, typename U> inline constexpr bool lesser_size_v = lesser_size<T, U>::value;

template <typename T, typename U>
struct geq_size : std::conditional_t<(sizeof(T) >= sizeof(U)), std::true_type, std::false_type> {};

template <typename T, typename U> inline constexpr bool geq_size_v = geq_size<T, U>::value;

template <typename T, typename U>
struct leq_size : std::conditional_t<(sizeof(T) <= sizeof(U)), std::true_type, std::false_type> {};

template <typename T, typename U> inline constexpr bool leq_size_v = leq_size<T, U>::value;

template <typename T>
concept primitive_type =
    !std::is_same_v<typename matches_which<T, std::is_same, primitive_types>::type, empty_t>;

// static_assert(primitive_type<int> && primitive_type<char> && primitive_type<const volatile void>);
static_assert(primitive_type<int> && primitive_type<char>);
static_assert(primitive_type<char>);

template <typename T, typename... U>
concept any_of = (std::same_as<T, U> || ...);

template <typename T, typename...> struct fake_dependency {
    using type = T;
};

template <typename T, typename... Ts> using fake_dependency_t = fake_dependency<T, Ts...>;

template <auto n>
concept is_power_of_2 = n != 0 && (n & (n - 1)) == 0;

template <typename T, typename U>
concept decays_to = std::same_as<std::decay_t<T>, std::decay_t<U>>;

template <template <typename...> class T, typename... Ts>
concept valid_template_params = requires { std::ignore = std::declval<T<Ts...>>(); };

template <template <typename...> class, typename> struct same_template_helper;

template <template <typename...> class U, template <typename...> class T, typename... Ts>
struct same_template_helper<U, T<Ts...>> {
    static constexpr bool value =
        valid_template_params<U, Ts...> && std::same_as<T<Ts...>, U<Ts...>>;
};

template <typename T, template <typename...> class U>
concept same_template = same_template_helper<U, T>::value;

template <typename T>
using ref_or_copy_t = std::conditional_t<std::is_rvalue_reference_v<T> and
                                             !std::is_const_v<std::remove_reference_t<T>>,
                                         std::add_lvalue_reference_t<std::decay_t<T>>,
                                         std::decay_t<T>>;

template <auto size> struct type_size {
    char _[size];
};

template <typename, template <typename...> class> struct is_instance_of : std::false_type {};

template <template <typename...> class T, typename... U>
struct is_instance_of<T<U...>, T> : std::true_type {};

template <typename T, template <typename...> class U>
inline constexpr bool is_instance_of_v = is_instance_of<T, U>::value;

template <typename T, template <typename...> class... U>
concept instance_of = (is_instance_of_v<std::decay_t<T>, U> || ...);

template <typename T> using id_t = std::type_identity_t<T>;
template <typename T> using id_decay_t = id_t<std::decay_t<T>>;

template <typename T, template <typename...> class... U>
/*[[deprecated]]*/ concept decays_to_instance = instance_of<std::decay_t<T>, U...>;

}  // namespace jmaths::TMP
