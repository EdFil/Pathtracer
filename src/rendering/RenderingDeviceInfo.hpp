#pragma once

#include <cstdint>

#include "rendering/Types.hpp"

class RenderingDeviceInfo {
public:
    virtual ~RenderingDeviceInfo() = default;
    virtual bool init() = 0;

    virtual const char* vendor() const = 0;
    virtual const char* renderer() const = 0;
    virtual const char* version() const = 0;
    virtual void printInfo() const = 0;

    rendering::API renderingAPI() const { return _api; }
    int32_t maxVertexAttributes() const { return _maxVertexAttributes; };
    int32_t maxTextureSize() const { return _maxTextureSize; };
    int32_t maxTextureUnits() const { return _maxTextureUnits; };

protected:
    rendering::API _api = rendering::API::Invalid;
    int32_t _maxVertexAttributes = 0;
    int32_t _maxTextureSize = 0;
    int32_t _maxTextureUnits = 0;
};