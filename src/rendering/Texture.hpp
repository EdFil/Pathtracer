#pragma once

class Texture {
public:
    enum class WrapMode { Repeat = 0, MirroredRepeat, ClampToEdge, ClampToBorder };

    struct Params {
        WrapMode wrapMode = WrapMode::Repeat;
    };

    virtual ~Texture() = default;

    virtual bool init(const char* filePath, const Params& params) = 0;
    virtual void bind() const = 0;

    unsigned int handle() const { return _handle; }

protected:
    unsigned int _handle = 0;
};