//#pragma once
//
//#include "Resource.hpp"
//
//Resource::Resource(IResourceManager* resourceManager) : _resourceManager(resourceManager) {
//    if (resourceManager == nullptr) {
//        LOG_ERROR("[Resource] Creating resource with \"nullptr\" manager");
//	}
//}
//
//void Resource::add_ref(IHandle* handle) {
//    if (handle == nullptr) {
//        LOG_WARN("[Resource] Adding a \"nullptr\ handle");
//        return;
//    }
//
//    _references.push_back(handle);
//}
//
//void Resource::rem_ref(IHandle* handle) {
//    _references.erase_first(handle);
//}
//
//void ResourceManager::validateResource(IResource* resource) {
//}
//
//void ResourceManager::invalidateResource(IResource* resource) {
//}
