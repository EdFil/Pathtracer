#pragma once

#include <EASTL/string.h>
#include <EASTL/fixed_string.h>

class ResourcePath
{
public:
    static const eastl::string SPathPrefix;
    
    static bool isValidPath(const eastl::string_view& path);

public:
    ResourcePath() = default;
    explicit ResourcePath(const eastl::string_view& path);

    bool isValid() const { return _hash != 0 && isValidPath(_path); }
    uint32_t hash() const { return _hash; }
    const eastl::string& path() { return _path; }

private:
    eastl::string _path;
    uint32_t _hash = 0;

    void setPath(const eastl::string_view& path);
};

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
    void decode(char outType[5]) const;
    bool isValid() const { return isValidHash(_hash); }

private:
    uint32_t _hash;
};