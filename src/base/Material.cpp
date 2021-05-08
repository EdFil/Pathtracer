#include "base/Material.hpp"

#include <cstring>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#include "Logger.hpp"
#include "rendering/Program.hpp"
#include "rendering/Texture.hpp"

bool Material::init(Program* program) {
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
                Texture* texture = nullptr;
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

bool Material::setTexture(const char* uniformName, Texture* texture) {
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
