#include "base/Material.hpp"

#include <cstring>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <rapidjson/document.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Logger.hpp"
#include "file/FileManager.hpp"
#include "rendering/IProgram.hpp"
#include "rendering/ITexture.hpp"

bool Material::init(IProgram* program) {
	if (program == nullptr || !program->isValid()) {
		LOG_ERROR("[Material] Init: Invalid program. Program(%p)", program);
		return false;
	}

	unsigned int uniformDataSizeInBytes = Program::uniformSizeInBytes(program);
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

bool Material::setProgram(IProgram* program)
{
	if (program == nullptr || !program->isValid()) {
		LOG_ERROR("[Material] setProgram: Invalid program. Program(%s)");
		return false;
	}

	std::vector<char> newUniformData(Program::uniformSizeInBytes(program));

	// Copy compatible values
	if (_program != nullptr) {
		const std::map<std::string, UniformData>& currentUniforms = _program->activeUniforms();
		const std::map<std::string, UniformData>& newUniforms = program->activeUniforms();
		
		unsigned int newOffset = 0;
		for (const auto& newIt : newUniforms) {
			const auto compatibleUniform = currentUniforms.find(newIt.first);
			if (compatibleUniform != currentUniforms.cend() && compatibleUniform->second.type == newIt.second.type) {
				// Found compatible uniform... Lets get the data offset
				unsigned int byteOffset = 0;
				for (const auto& currentIt : currentUniforms) {
					if (currentIt.first == newIt.first) {
						memcpy(newUniformData.data() + newOffset, _uniformData.data() + byteOffset, UniformData::sizeInBytes(newIt.second.type));
						break;
					}

					byteOffset += UniformData::sizeInBytes(currentIt.second.type);
				}
			}

			newOffset += UniformData::sizeInBytes(newIt.second.type);
		}
	}

	_uniformData = std::move(newUniformData);
	_program = program;
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

Material* MaterialManager::createMaterial(const std::string& materialFilename)
{
	/*FileManager* fileManager = FileManager::instance();
	std::vector<char> file = fileManager->loadFile(materialFilename.c_str());
*/
	// 1. Parse a JSON string into DOM.
	const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
	rapidjson::Document d;
	d.Parse(json);

	// 2. Modify it by DOM.
	rapidjson::Value& s = d["stars"];
	s.SetInt(s.GetInt() + 1);

	// 3. Stringify the DOM
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);

	// Output {"project":"rapidjson","stars":11}
	LOG("%s", buffer.GetString());

	return nullptr;
}
