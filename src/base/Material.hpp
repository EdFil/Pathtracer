#pragma once

#include <glm/fwd.hpp>
#include <vector>
#include <map>
#include <memory>

#include "rendering/IProgram.hpp"

class IProgram;
class ITexture;

class Material {
public:
    bool init(IProgram* program);

    void bind();
	bool setProgram(IProgram* program);

	template<typename T>
	const bool setValue(const std::string& uniformName, const T& value);
	template<typename T>
	const T* getValue(const std::string& uniformName) const;

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

// ------------------------------------

template<typename T>
inline const bool Material::setValue(const std::string& uniformName, const T& value)
{
	const std::map<std::string, UniformData>& uniforms = _program->activeUniforms();
	unsigned int dataBegin = 0;
	for (const auto& it : uniforms) {
		if (it.first == uniformName) break;
		dataBegin += UniformData::sizeInBytes(it.second.type);
	}

	if (dataBegin + sizeof(value) > _uniformData.size()) {
		LOG_ERROR("[Material] Cannot fit value in uniform data");
		return false;
	}

	memcpy(_uniformData.data() + dataBegin, &value, sizeof(value));
	return true;
}

template<typename T>
inline const T* Material::getValue(const std::string& name) const {
	if (_program == nullptr || !_program->isValid()) return nullptr;

	unsigned int currentOffset = 0;
	for (const auto& it : _program->activeUniforms()) {
		if (it.first == uniformName) {
			return reinterpret_cast<const T*>(_uniformData.data() + currentOffset);
		}
	}
	return nullptr;
}