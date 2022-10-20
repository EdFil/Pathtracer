#pragma once

#include "ResourcePath.hpp"

#include "algorithm/Hash.hpp"

const eastl::string ResourcePath::SPathPrefix = "data://";

bool ResourcePath::isValidPath(const eastl::string_view& path)
{
    return !path.empty() && path.starts_with(SPathPrefix) && eastl::find(path.cbegin() + SPathPrefix.size(), path.cend(), '\\') == path.cend();
}

eastl::string::const_iterator ResourcePath::findExtensionStart(const eastl::string& path)
{
    auto index = path.find_last_of('.');
    if (index == eastl::string::npos) return path.cend();
    return path.cbegin() + index;
}

ResourcePath::ResourcePath(const eastl::string_view& path)
{
    setPath(path);
}

eastl::string_view ResourcePath::getExtension() const
{
    auto it = findExtensionStart(_path);
    if (it == _path.cend()) return {_path.cend(), 0};
	
	it++;
    return eastl::string_view(it, (_path.cend() - it));
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