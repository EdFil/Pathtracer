#pragma once

#include "Resource.hpp"

BaseResource::BaseResource(IResourceManager* resourceManager, eastl::string name) 
	: _resourceManager(resourceManager)
	, _name(std::move(name)), _state(State::Unused)
{
    if (resourceManager == nullptr) {
        LOG_ERROR("[BaseResource::BaseResource] Creating resource with \"nullptr\" manager");
    }
}

void BaseResource::add_ref(IHandle* handle)
{
    if (handle == nullptr) {
        LOG_WARN("[BaseResource::add_ref] Adding a \"nullptr\" handle");
        return;
    }

    _references.push_back(handle);
    if (_references.size() == 1) {
        _resourceManager->changeState(this, State::Used);
	}
}

void BaseResource::rem_ref(IHandle* handle)
{
    _references.erase_first_unsorted(handle);
    if (_references.empty()) {
        _resourceManager->changeState(this, State::Unused);
    }
}

void NewResourceManager::changeState(IResource* resource, IResource::State newState)
{
    if (resource == nullptr) {
        LOG_ERROR("[ResourceManager::changeState] Resource is nullptr");
        return;
    }

    const IResource::State currentState = resource->state();
    switch (newState) {
        case IResource::State::Invalid:
            LOG_WARN("[ResourceManager::changeState] Resource(%s) is invalid.", resource->name().data());
            break;
        case IResource::State::Used: {
            auto removeIt = _resourcesToDelete.erase_first_unsorted(resource);
            if (removeIt == _resourcesToDelete.end()) {
                LOG_WARN("[ResourceManager::changeState] Could not remove Resource(%s) in _resourcesToDelete", resource->name().data());
                auto findIt = eastl::find(_resources.cbegin(), _resources.cend(), resource);
                if (findIt == _resources.cend()) {
                    LOG_ERROR("[ResourceManager::changeState] Could not find Resource(%s) in _resources", resource->name().data());
                }
            }

            _resources.push_back(resource);
            break;
        }
        case IResource::State::Unused: {
            auto removeIt = _resources.erase_first_unsorted(resource);
            if (removeIt == _resources.end()) {
                LOG_WARN("[ResourceManager::changeState] Could not remove Resource(%s) in _resources", resource->name().data());
                auto findIt = eastl::find(_resourcesToDelete.cbegin(), _resourcesToDelete.cend(), resource);
                if (findIt == _resourcesToDelete.cend()) {
                    LOG_ERROR("[ResourceManager::changeState] Could not find Resource(%s) in _resourcesToDelete", resource->name().data());
                }
            }

            _resourcesToDelete.push_back(resource);
            break;
        }
        default:
            break;
    }
}
