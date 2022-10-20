#pragma once

#include <EASTL/string_view.h>

class ResourceType
{
public:
    static const uint8_t KMaxSize = 4;

    static uint32_t encodeType(const eastl::string_view& type);
    static bool decodeType(char outType[KMaxSize + 1], uint32_t hash);
    static bool isValidHash(uint32_t hash);

public:
    ResourceType() = default;
    ResourceType(const eastl::string_view& type);

    uint32_t hash() const { return _hash; }
    void decode(char outType[5]) const { decodeType(outType, _hash); }
    bool isValid() const { return isValidHash(_hash); }

	eastl::string_view getExtension() const;

private:
    uint32_t _hash;
};