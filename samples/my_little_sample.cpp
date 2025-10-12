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

#include <bitset>
#include <chrono>
#include <cstring>
#include <iostream>
#include <print>
#include <thread>
#include <type_traits>
#include <utility>

#include "TMP.hpp"
#include "basic_N.hpp"
#include "basic_Q.hpp"
#include "basic_Z.hpp"
#include "calc.hpp"
#include "error.hpp"
#include "hash.hpp"
#include "literals.hpp"
#include "rand.hpp"
#include "scoped_timer.hpp"

using namespace jmaths;

template <typename T> class callable_container;

template <typename Ret, typename... Args> class callable_container<Ret(Args...)> {
   public:
    virtual Ret call(Args... args) = 0;
    virtual ~callable_container() = default;
};

template <typename T, typename U> class called_object;

template <typename T, typename Ret, typename... Args>
class called_object<T, Ret(Args...)> : public callable_container<Ret(Args...)> {
   public:
    called_object(T obj) : obj_(obj) {}

    Ret call(Args... args) override {
        return obj_(std::forward<Args>(args)...);
    }

   private:
    T obj_;
};

template <typename T> class function;

template <typename Ret, typename... Args> class function<Ret(Args...)> {
   public:
    function() : get_object_(nullptr) {}

    template <typename F> function(F && f) {
        using contained_type = called_object<F, function_type>;

        if constexpr (is_local(sizeof(contained_type))) {
            if constexpr (!std::is_trivially_destructible_v<contained_type>) {
                new (object_data_.invokable_arr.data) contained_type(std::forward<F>(f));
            } else {
                std::memcpy(object_data_.invokable_arr.data, &f, sizeof(f));
            }

            object_data_.invokable_arr.initialized = true;

            get_object_ = [](data_type * object_data) {
                return static_cast<callable_type *>(
                    static_cast<contained_type *>(object_data->invokable_arr.data));
            };

            destroy_ = [](data_type * object_data) {
                if constexpr (!std::is_trivially_destructible_v<contained_type>) {
                    static_cast<contained_type *>(object_data->invokable_arr.data)
                        ->~contained_type();
                }
                object_data->invokable_arr.initialized = false;
            };

            move_ = [](data_type * dest, data_type * src) {
                if constexpr (!std::is_trivially_move_constructible_v<contained_type>) {
                    new (dest->invokable_arr.data) contained_type(
                        std::move(*static_cast<contained_type *>(src->invokable_arr.data)));

                    dest->invokable_arr.initialized = true;
                } else {
                    std::memcpy(&dest->invokable_arr,
                                &src->invokable_arr,
                                sizeof(src->invokable_arr));
                }
            };
            copy_ = [](data_type * dest, data_type * src) {
                dest->invokable_ptr = new decltype(*dest->invokable_ptr)(*src->invokable_ptr);

                if constexpr (!std::is_trivially_copy_constructible_v<contained_type>) {
                    new (dest->invokable_arr.data)
                        contained_type(*static_cast<contained_type *>(src->invokable_arr.data));

                    dest->invokable_arr.initialized = true;
                } else {
                    std::memcpy(&dest->invokable_arr,
                                &src->invokable_arr,
                                sizeof(src->invokable_arr));
                }
            };
        } else {
            object_data_.invokable_ptr = new contained_type(std::forward<F>(f));
            get_object_ = [](data_type * object_data) {
                return object_data->invokable_ptr;
            };
            destroy_ = [](data_type * object_data) {
                delete object_data->invokable_ptr;
            };
            move_ = [](data_type * dest, data_type * src) {
                std::memcpy(&dest->invokable_ptr, &src->invokable_ptr, sizeof(src->invokable_ptr));
            };
            copy_ = [](data_type * dest, data_type * src) {
                dest->invokable_ptr =
                    new contained_type(*static_cast<contained_type *>(src->invokable_ptr));
            };
        }
    }

    function(function && f) {
        if (!f.contains_obj_()) {
            get_object_ = nullptr;
        } else {
            f.move_(this->object_data_, f.object_data_);
        }
    }

    function(const function & f) {
        if (!f.contains_obj_()) {
            get_object_ = nullptr;
        } else {
            f.copy_(this->object_data_, f.object_data_);
        }
    }

    Ret operator()(Args... args) {
        return get_object_(&object_data_)->call(std::forward<Args>(args)...);
    }

   private:
    using function_type = Ret(Args...);
    using callable_type = callable_container<function_type>;

    static constexpr auto max_allowed_size = sizeof(callable_type *) - sizeof(bool);

    static consteval bool is_local(std::size_t size) {
        return size <= max_allowed_size;
    }

    constexpr bool contains_obj_() {
        return get_object_ == nullptr;
    }

    union data_type {
        callable_type * invokable_ptr;

        struct {
            unsigned char data[max_allowed_size];
            bool initialized;

            // static_assert(sizeof(invokable_ptr) == 0);
        } invokable_arr;
    } object_data_;

    callable_type * (*get_object_)(data_type *);

    void (*destroy_)(data_type *);

    void (*move_)(data_type *, data_type *);
    void (*copy_)(data_type *, data_type *);
};

void free_func() {
    static int i = 0;
    std::cout << "Free func! " << ++i << '\n';
}

constexpr bool timed_func() {
    JMATHS_FUNCTION_TO_LOG;

    using namespace std::chrono_literals;

    std::this_thread::sleep_for(2s);

    return true;
}

int main(int, char *[]) {
#if 0
    const N n1("255");

    std::cout << n1 << '\n';
    std::cout << n1.to_str(10) << '\n';
    std::println("n1 == {}!", n1);
    std::println("n1 == {0}!", n1);
    std::println("n1 == {0:10}!", n1);

    std::cout << n1 << '\n';
    std::cout << n1.to_str(16) << '\n';
    std::println("n1 == {}!", n1);
    std::println("n1 == {0}!", n1);
    std::println("n1 == {0:64}!", n1);

    Z z1("255");

    z1 = z1 * Z("81764");

    std::cout << z1 << '\n';
    std::cout << z1.to_str(16) << '\n';
    std::println("z1 == {}!", z1);
    std::println("z1 == {0}!", z1);
    std::println("z1 == {0:64}!", z1);

    #if 0
    int base;
    std::cin >> base;
    std::cout << z1.to_str(base) << '\n';
    #endif

    auto inttest = 17'335'634'962'698'473'524ULL;

    N inthandletest{inttest};

    std::println("{}", inthandletest.fits_into<unsigned long>().value_or(1));
    std::println("{}", inthandletest);
    std::println("{}", inttest);
    std::println("{:16}", inthandletest);
    std::println("{}", inthandletest.to_hex());
    std::cout << std::hex << std::uppercase << inttest << '\n';
    std::println("{:2}", inthandletest);
    std::println("{}", inthandletest.to_bin());
    std::println("{}", std::bitset<64>(inttest).to_string());
    auto fraction1 = Q("12/17");
    std::println("{}", fraction1);
    auto fp1 = *fraction1.fits_into<double>();
    std::println("{}", fp1);
    std::println("{}", Q(*Q(fp1).fits_into<double>()));
    std::println("{}", Q(fp1));
    double xx = 1501209.12123092929;
    std::println("{}", *Q(xx).fits_into<double>());

    std::cout << std::boolalpha
              << (std::bitset<64>(inttest).to_string() == std::format("{:2}", inthandletest))
              << '\n';

    static const Q constexpr_test1 = Q(1.0);

    [[maybe_unused]] static const double test_constexpr2 = *Q(1.0).fits_into<double>();

    // static_assert(test_constexpr2 == 2.0);

    [[maybe_unused]] auto conversion_test = [](const N & n) {
        std::println("{}", n);
    };

    Z some_Z(-193);
    std::println("{}", ~~some_Z);

    conversion_test((N &)some_Z);

    N * ptr_Z = (N *)new Z(-19879878);
    ((Z *)ptr_Z)->flip_sign();
    std::println("ptr_Z: {}", *(Z *)ptr_Z);

    std::println("Random N: {}", jmaths::rand<N>::generate(3));

    std::println("Literals: {}, {}, {}", -11_Z, 12_N, 144_N);

    std::println("Sqrt whole: {}", calc::sqrt_whole(15_N));
    std::println("Pow: {}", calc::pow(15_N, 3_N));
    std::println("Sqrt whole: {}", calc::sqrt_whole(15_N));

    auto pow1 = static_cast<N (*)(N &&, N &&)>(calc::pow);
    N n123(13);
    std::println("Pow: {}", pow1(std::move(n123), 2));

    // static_assert(timed_func());
    timed_func();
    timed_func();

    using my_N_type = basic_N<std::uint8_t, std::uint16_t>;

    for (unsigned char i = 110; i < 130; ++i)
        std::println("{}", +my_N_type{((unsigned char)i)}.fits_into<signed char>().value_or(17));

    using hash_N = std::hash<my_N_type>;
    hash_N my_hash;
    (void)my_hash;

    static_assert((my_N_type{12} == 12));
    static_assert((my_N_type{12}[0] == 0));
    static_assert((my_N_type{12}[0] = 0) == 0);

    std::println("{} and {}", Q(12.0), Q(11.9));

    std::ignore = Q(12.0) > Q(11.9);
    std::ignore = Q(12.0) != Q(11.9);
    std::ignore = Q(12.0) + Q(11.9);
    std::ignore = Q(12.0) - Q(11.9);
    std::ignore = Q(12.0) * Q(11.9);
    std::ignore = Q(12.0) / Q(11.9);
    try {
        std::ignore = Q(12.0) & Q(11.9);
    } catch (const std::exception & e) { std::println("{}", e.what()); }
    std::ignore = Q(12.0) | Q(11.9);
    std::ignore = Q(12.0) ^ Q(11.9);

    std::println("{}, {}, {}", 12_Q, 141_Q, 10123234_Q);
#endif

    std::println("{:10}", N(17));
    std::println("{:10}", Z(-18));
    std::println("{:10}", Q(-22.7));

    std::println("{:10}", (N("24") / N("6")).first);
}
