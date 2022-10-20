#pragma once

#include "Resource.hpp"

ResourceID::ResourceID(const eastl::string_view& path)
{
    setPath(path);
}

void ResourceID::setPath(const eastl::string_view& path)
{
    _resourcePath = ResourcePath(path);
    if (_resourcePath.isValid()) {

	}

	_resourceType = ResourceType();
}
