#pragma once

#include "rendering/RenderingDeviceInfo.hpp"

class RenderingDeviceInfoGL final : public RenderingDeviceInfo {
public:
    bool init() override;

    const char* vendor() const override;
    const char* renderer() const override;
    const char* version() const override;
    void printInfo() const override;
};