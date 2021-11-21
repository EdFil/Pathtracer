#pragma once

#include <glm/fwd.hpp>
#include <vector>

class Program;
class ITexture;

class Material {
public:
    bool init(Program* program);

    void bind();
    bool setTexture(const char* uniformName, ITexture* texture);
    bool setValue(const char* uniformName, const glm::mat4& value);

private:
    Program* _program;
    std::vector<char> _uniformData;
};