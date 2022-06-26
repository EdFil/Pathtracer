#include "Hash.hpp"

#include <xxhash.h>

namespace Hash {
    constexpr static XXH32_hash_t gHashSeed = 0xE378E795; // https://www.random.org/cgi-bin/randbyte?nbytes=4&format=h
}

uint32_t Hash::Hash32(const void* input, size_t length) {
    return XXH32(input, length, gHashSeed);
}

uint64_t Hash::Hash64(const void* input, size_t length) {
    return XXH64(input, length, gHashSeed);
}
