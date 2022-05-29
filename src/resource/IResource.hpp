#pragma once

#include <EASTL/string.h>

#include "algorithm/Hash.hpp"

class BaseResource {
public:
    virtual ~BaseResource() = default;
    
    uint32_t id() const { return _id; }
    eastl::string_view path() const { return _path; }

    virtual void save() = 0;
    virtual void load() = 0;

protected:
    uint32_t _id = 0;
    eastl::string _path;
};

class Mesh2 : public BaseResource {
public:
    Mesh2(eastl::string path) {

    }
    Mesh2() {

    }

    void save() override {

    }

    void load() override { 
        _id = Hash::Hash32(_path.data(), _path.size());
    }
};