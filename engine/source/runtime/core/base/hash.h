#pragma once

#include <cstddef>
#include <functional>

template <typename T>
inline void hash_combine(std::size_t& seed, const T& val){
    seed ^= std::hash<T> {}(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename T, typename... Ts>
inline void hash_combine(std::size_t& seed, const T& val, Ts... rest){
    hash_combine(seed, val);
    if constexpr (sizeof...(Ts) > 1){
        hash_combine(seed, rest...);
    }
}