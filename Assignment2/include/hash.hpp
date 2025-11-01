// hash.hpp
#pragma once
#include <string_view>
#include <cstdint>

uint64_t fnv1a_64(const uint64_t len, const void* src);
uint64_t fnv1a_64(std::string_view source);