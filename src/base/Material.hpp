#pragma once

#include <glm/fwd.hpp>
#include <vector>

class Program;
class Texture;

class Material {
public:
    bool init(Program* program);

    void bind();
    bool setTexture(const char* uniformName, Texture* texture);
    bool setValue(const char* uniformName, const glm::mat4& value);

private:
    Program* _program;
    std::vector<char> _uniformData;
};