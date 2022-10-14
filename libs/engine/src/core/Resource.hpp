#pragma once

#include "EASTL/string.h"
#include "EASTL/vector.h"
#include "Handle.hpp"

class IResourceManager;

class IResource
{
public:
    enum class State { Invalid = -1, Unused = 0, Used };

    virtual ~IResource() = default;

    virtual State state() const = 0;
    virtual eastl::string_view name() const = 0;
    virtual void add_ref(IHandle* handle) = 0;
    virtual void rem_ref(IHandle* handle) = 0;

private:
};

class IResourceManager
{
public:
    virtual void changeState(IResource* resource, IResource::State state) = 0;
};

class BaseResource final : public IResource
{
public:
    BaseResource(IResourceManager* resourceManager, eastl::string name);

    State state() const override { return _state; }
    eastl::string_view name() const override { return _name; } 
    void add_ref(IHandle* handle) override;
    void rem_ref(IHandle* handle) override;

private:
    IResourceManager* _resourceManager = nullptr;
    eastl::string _name;
    State _state = State::Invalid;
    eastl::vector<IHandle*> _references;
};

class NewResourceManager final : public IResourceManager
{
public:
    void changeState(IResource* resource, IResource::State state) override;

private:
    eastl::vector<IResource*> _resources;
    eastl::vector<IResource*> _resourcesToDelete;
};