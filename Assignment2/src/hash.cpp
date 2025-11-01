// hash.cpp
#include "hash.hpp"
#include <cstdint>

// note: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
uint64_t fnv1a_64(const uint64_t len, const void* src)
{
    const uint8_t* data = (const uint8_t*)src;
    uint64_t h = 14695981039346656037ull;
    for (uint64_t i = 0; i < len; ++i) {
        h ^= uint64_t(data[i]);
        h *= 1099511628211ull;
    }
    return h;
}

uint64_t fnv1a_64(std::string_view source)
{
    return fnv1a_64(source.length(), source.data());
}