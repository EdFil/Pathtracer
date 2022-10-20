#pragma once

#include "ResourceType.hpp"

#include <cctype>

uint32_t ResourceType::encodeType(const eastl::string_view& type)
{
    const size_t length = type.length();
    if (length == 0 || length > KMaxSize) return 0;

    uint32_t hash = 0;
    for (size_t i = 0; i < length; ++i) {
        const char elem = type[i];
        const bool isLowerCase = eastl::CharToLower(elem) != elem;
        if (isLowerCase) return 0;

        constexpr size_t byteSize = 8;
        hash |= elem << (length - 1 - i) * byteSize;
    }

    return hash;
}

bool ResourceType::decodeType(char outType[KMaxSize + 1], uint32_t hash)
{
    if (!isValidHash(hash)) return false;

    int idx = 0;

    outType[idx] = (uint8_t)(hash >> 24);
    if (outType[idx] != 0) {
        idx++;
    }

    outType[idx] = (uint8_t)((hash & 0x00FF0000) >> 16);
    if (outType[idx] != 0) {
        idx++;
    }

    outType[idx] = (uint8_t)((hash & 0x0000FF00) >> 8);
    if (outType[idx] != 0) {
        idx++;
    }

    outType[idx] = (uint8_t)((hash & 0x000000FF));
    if (outType[idx] != 0) {
        idx++;
    }

    outType[idx] = 0;
    return true;
}

bool ResourceType::isValidHash(uint32_t hash)
{
    if (hash == 0) return false;

    const char result[4]{
        static_cast<char>((hash & 0xff000000) >> 24),
        static_cast<char>((hash & 0x00ff0000) >> 16),
        static_cast<char>((hash & 0x0000ff00) >> 8),
        static_cast<char>((hash & 0x000000ff)),
    };

    // Find first non-zero value
    size_t i = 0;
    for (; i < KMaxSize; ++i)
        if (result[i] != 0) break;

    // All values from here should be either characters or non-zero decimal numbers
    for (; i < KMaxSize; ++i) {
        if (result[i] == 0) return false;
        if (!std::islower(result[i]) && !std::isdigit(result[i])) return false;
    }

    return true;
}

ResourceType::ResourceType(const eastl::string_view& type) : _hash(encodeType(type))
{
}

eastl::string_view ResourceType::getExtension() const
{
    return eastl::string_view();
}
