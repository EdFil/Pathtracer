#include "base/Material.hpp"

#include <cstring>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#include "Logger.hpp"
#include "rendering/IProgram.hpp"
#include "rendering/ITexture.hpp"

bool Material::init(IProgram* program) {
    if (program == nullptr || !program->isValid()) return false;

    const std::map<std::string, UniformData>& uniforms = program->activeUniforms();
    unsigned int uniformDataSizeInBytes = 0;
    for (const auto& it : uniforms) {
        uniformDataSizeInBytes += UniformData::sizeInBytes(it.second.type);
    }

    _program = program;
    _uniformData.resize(uniformDataSizeInBytes);
    memset(_uniformData.data(), 0, uniformDataSizeInBytes);

    return true;
}

void Material::bind() {
    _program->bind();
    const std::map<std::string, UniformData>& uniforms = _program->activeUniforms();
    unsigned int dataIndex = 0;
    for (const auto& it : uniforms) {
        switch (it.second.type) {
            case UniformData::Type::Mat4:
                _program->setUniform(it.first.c_str(), *(glm::mat4*)(_uniformData.data() + dataIndex));
                break;
            case UniformData::Type::Float:
                _program->setUniform(it.first.c_str(), *(float*)(_uniformData.data() + dataIndex));
                break;
            case UniformData::Type::Texture: {
                ITexture* texture = nullptr;
                memcpy(&texture, _uniformData.data() + dataIndex, sizeof(void*));
                texture->bind();
                break;
            }
            default:
                break;
        }
        dataIndex += UniformData::sizeInBytes(it.second.type);
    }
}

bool Material::setTexture(const char* uniformName, ITexture* texture) {
    const std::map<std::string, UniformData>& uniforms = _program->activeUniforms();
    unsigned int dataBegin = 0;
    for (const auto& it : uniforms) {
        if (it.first == uniformName) break;
        dataBegin += UniformData::sizeInBytes(it.second.type);
    }

    if (dataBegin + sizeof(void*) > _uniformData.size()) {
        LOG_ERROR("[Material] Cannot fit value in uniform data");
        return false;
    }

    memcpy(_uniformData.data() + dataBegin, &texture, 8);
    return true;
}

bool Material::setValue(const char* uniformName, const glm::mat4& value) {
    const std::map<std::string, UniformData>& uniforms = _program->activeUniforms();
    unsigned int dataBegin = 0;
    for (const auto& it : uniforms) {
        if (it.first == uniformName) break;
        dataBegin += UniformData::sizeInBytes(it.second.type);
    }

    if (dataBegin + sizeof(float) * 16 > _uniformData.size()) {
        LOG_ERROR("[Material] Cannot fit value in uniform data");
        return false;
    }

    memcpy(_uniformData.data() + dataBegin, glm::value_ptr(value), sizeof(float) * 16);
    return true;
}

Material* MaterialManager::material(const std::string& name)
{
	const auto it = _materials.find(name);
	if (it != _materials.cend()) {
		return it->second.get();
	}

	return nullptr;
}

Material* MaterialManager::createMaterial(const std::string& name, IProgram* program)
{
	if (Material* cachedMaterial = material(name)) {
		return cachedMaterial;
	}

	if (program == nullptr || !program->isValid()) {
		LOG_ERROR("[MaterialManagerGL] Invalid Program. Program(%p)", program);
		return nullptr;
	}

	std::unique_ptr<Material> material = std::make_unique<Material>();
	if (!material || !material->init(program)) {
		LOG_ERROR("[MaterialManagerGL] Failed to create Material. Material(%p) Name(%s)", material.get(), name.c_str());
		return nullptr;
	}

	const auto it = _materials.emplace(name, std::move(material));
	if (!it.second) {
		LOG_ERROR("[MaterialManagerGL] Failed to insert material into map. Memory allocation failed?");
		return nullptr;
	}

	Material* createdMaterial = it.first->second.get();
	LOG("[ProgramManagerGL] Create Material success. Program(%p) Name(%s)", createdMaterial, name.c_str());
	return createdMaterial;
}
