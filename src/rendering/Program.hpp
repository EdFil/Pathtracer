#pragma once

class Shader;

class Program {
public:
    static const char* k_positionColor;

    virtual ~Program() = default;

    virtual bool init(const Shader& vertexShader, const Shader& fragmentShader) = 0;
    virtual bool setShader(const Shader& vertexShader) = 0;
    virtual void bind() const = 0;

    unsigned int handle() const { return _handle; }
    const Shader* vertexShader() const { return _vertexShader; }
    const Shader* fragmentShader() const { return _fragmentShader; }

protected:
    unsigned int _handle;
    const Shader* _vertexShader = nullptr;
    const Shader* _fragmentShader = nullptr;
};