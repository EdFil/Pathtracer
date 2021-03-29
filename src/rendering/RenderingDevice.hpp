#pragma once

class RenderingDeviceInfo;
class ShaderManager;

class RenderingDevice {
public:
    virtual bool init() = 0;
    virtual void preRender() = 0;
    virtual void postRender() = 0;

    virtual RenderingDeviceInfo* deviceInfo() = 0;
    virtual ShaderManager* shaderManager() = 0;
};