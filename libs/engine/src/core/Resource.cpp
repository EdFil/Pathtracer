#pragma once

#include "Resource.hpp"

#include <cctype>

#include "algorithm/Hash.hpp"

const eastl::string ResourcePath::SPathPrefix = "data:\\";

bool ResourcePath::isValidPath(const eastl::string_view& path)
{
    return !path.empty() && path.starts_with(SPathPrefix) && path[0] != '\\';
}

ResourcePath::ResourcePath(const eastl::string_view& path)
{
    setPath(path);
}

void ResourcePath::setPath(const eastl::string_view& path)
{
    if (isValidPath(path)) {
        _path = path;
        _hash = Hash::Hash32(path);
    } else {
        _path.clear();
        _hash = 0;
    }
}

// -----------------

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
        (hash & 0xff000000) >> 24,
        (hash & 0x00ff0000) >> 16,
        (hash & 0x0000ff00) >> 8,
        (hash & 0x000000ff),
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

void ResourceType::decode(char outType[5]) const
{
    decodeType(outType, _hash);
}