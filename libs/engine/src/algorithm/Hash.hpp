#pragma once

#include <eastl/string_view.h>

namespace Hash {
    uint32_t Hash32(eastl::string_view input);
    uint64_t Hash64(eastl::string_view input);
}
