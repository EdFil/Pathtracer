#pragma once

#include <EASTL/string.h>

class ResourcePath
{
public:
    static const eastl::string SPathPrefix;

    static bool isValidPath(const eastl::string_view& path);
    static eastl::string::const_iterator findExtensionStart(const eastl::string& path);

public:
    ResourcePath() = default;
    explicit ResourcePath(const eastl::string_view& path);

    bool isValid() const { return _hash != 0 && isValidPath(_path); }
    uint32_t hash() const { return _hash; }
    const eastl::string& path() { return _path; }

	eastl::string_view getExtension() const;

private:
    eastl::string _path;
    uint32_t _hash = 0;

    void setPath(const eastl::string_view& path);
};