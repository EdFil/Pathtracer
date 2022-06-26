#pragma once

#include <stddef.h>
#include <cstdint>

namespace Hash {
    uint32_t Hash32(const void* input, size_t length);
    uint64_t Hash64(const void* input, size_t length);
}
