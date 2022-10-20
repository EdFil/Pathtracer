#include "Hash.hpp"

#include <xxhash.h>

namespace Hash {
    constexpr static XXH32_hash_t gHashSeed = 0xE378E795; // https://www.random.org/cgi-bin/randbyte?nbytes=4&format=h
}

uint32_t Hash::Hash32(eastl::string_view input)
{
    return XXH32(input.data(), input.length(), gHashSeed);
}

uint64_t Hash::Hash64(eastl::string_view input)
{
    return XXH64(input.data(), input.length(), gHashSeed);
}
