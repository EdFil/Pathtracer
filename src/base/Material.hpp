#pragma once

#include <glm/fwd.hpp>
#include <vector>

class IProgram;
class ITexture;

class Material {
public:
    bool init(IProgram* program);

    void bind();
    bool setTexture(const char* uniformName, ITexture* texture);
    bool setValue(const char* uniformName, const glm::mat4& value);

private:
    IProgram* _program;
    std::vector<char> _uniformData;
};