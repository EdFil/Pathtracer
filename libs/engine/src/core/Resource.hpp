//#pragma once
//
//#include "Handle.hpp"
//#include "EASTL/vector.h"
//
//class IResourceManager;
//
//class IResource {
//public:
//    enum class State { Invalid = -1, Unused = 0, Used };
//
//    virtual ~IResource() = default;
//
//    virtual void add_ref(IHandle* handle) = 0;
//    virtual void rem_ref(IHandle* handle) = 0;
//
//private:
//};
//
//class IResourceManager {
//public:
//    virtual void unmarkAsDelete(IResource* resource) = 0;
//    virtual void markAsDelete(IResource* resource) = 0;
//};
//
//class Resource final : public IResource {
//public:
//    Resource(IResourceManager* resourceManager);
//
//    void add_ref(IHandle* handle) override;
//    void rem_ref(IHandle* handle) override;
//
//private:
//    IResourceManager* _resourceManager = nullptr;
//    eastl::vector<IHandle*> _references;
//};
//
//class ResourceManager final : public IResourceManager {
//public:
//    void validateResource(IResource* resource) override;
//    void invalidateResource(IResource* resource) override;
//
//private:
//    eastl::vector<IResource*> _resources;
//    eastl::vector<IResource*> _resourcesToDelete;
//};