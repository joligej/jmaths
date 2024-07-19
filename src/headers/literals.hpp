#pragma once

#include "declarations.hpp"

// declarations of literals
namespace jmaths {

inline namespace literals {

N operator""_N(const char * num_str);

Z operator""_Z(const char * num_str);

}  // namespace literals

}  // namespace jmaths
