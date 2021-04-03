#pragma once

class Shader {
public:
    enum class Type { Vertex, Fragment };

    virtual ~Shader() = default;

    virtual bool init(Type type, const char* source) = 0;
    virtual bool reload(const char* source) = 0;

    unsigned int handle() const { return _handle; }
    Type type() const { return _type; }

protected:
    unsigned int _handle = 0;
    Type _type;
};