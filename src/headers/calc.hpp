#pragma once

#include <utility>

#include "jmaths_declarations.hpp"

// declarations of namespace structs calc and detail
namespace jmaths {

struct calc {
    
    static N gcd (N a, N b);
    static std::pair<N, N> sqrt (const N & num);
    static N sqrt_whole (const N & num);
    static N pow (N base, N exponent);
    static N pow_mod (N base, N exponent, const N & mod);
    static Z pow (Z base, N exponent);
    
}; // /namespace struct calc

} // /namespace jmaths
