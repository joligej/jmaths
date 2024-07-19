#pragma once

#include <cstddef>
#include <functional>

#include "declarations.hpp"

// declarations of std::hash specialisations
namespace std {

template <> struct hash<jmaths::N> {
    std::size_t operator()(const jmaths::N & n) const;
};

template <> struct hash<jmaths::Z> {
    std::size_t operator()(const jmaths::Z & z) const;
};

template <> struct hash<jmaths::Q> {
    std::size_t operator()(const jmaths::Q & q) const;
};

}  // namespace std
