#pragma once

#include <cstdint>

#include "rendering/Types.hpp"
#include "Renderer.hpp"

class IRenderingDeviceInfo {
public:
    virtual ~IRenderingDeviceInfo() = default;
    virtual bool init() = 0;

    virtual const char* vendor() const = 0;
    virtual const char* renderer() const = 0;
    virtual const char* version() const = 0;
    virtual void printInfo() const = 0;

    virtual Renderer::API renderingAPI() const = 0;
    virtual int32_t maxVertexAttributes() const = 0;
    virtual int32_t maxTextureSize() const = 0;
    virtual int32_t maxTextureUnits() const = 0;
};