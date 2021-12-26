#pragma once

#include <glm/fwd.hpp>
#include <vector>
#include <map>
#include <memory>

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

class MaterialManager {
public:
	Material* material(const std::string& name);
	Material* createMaterial(const std::string& name, IProgram* program);

private:
	std::map<std::string, std::unique_ptr<Material>> _materials;
};