#pragma once

#include "ResourcePath.hpp"
#include "ResourceType.hpp"

class ResourceID
{
public:
    ResourceID() = default;
    ResourceID(const eastl::string_view& path);

private:
    ResourcePath _resourcePath;
    ResourceType _resourceType;

	void setPath(const eastl::string_view& path);
};