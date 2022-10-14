#pragma once

#include "rendering/IRenderingDeviceInfo.hpp"

class RenderingDeviceInfoGL final : public IRenderingDeviceInfo {
public:
    bool init() override;

    virtual const char* vendor() const override;
    virtual const char* renderer() const override;
    virtual const char* version() const override;
    virtual void printInfo() const override;

    virtual Renderer::API renderingAPI() const override { return Renderer::API::OpenGL; }
    virtual int32_t maxVertexAttributes() const override { return _maxVertexAttributes; };
    virtual int32_t maxTextureSize() const override { return _maxTextureSize; };
    virtual int32_t maxTextureUnits() const override { return _maxTextureUnits; };

protected:
    int32_t _maxVertexAttributes = 0;
    int32_t _maxTextureSize = 0;
    int32_t _maxTextureUnits = 0;
};