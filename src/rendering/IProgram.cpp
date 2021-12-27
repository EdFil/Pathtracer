#include "rendering/IProgram.hpp"

#include "Logger.hpp"

const char* Program::k_position = "position";
const char* Program::k_positionTexture = "positionTexture";
const char* Program::k_positionNormalTexture = "posNormTex";

unsigned int Program::uniformSizeInBytes(IProgram* program)
{
	if (program == nullptr || !program->isValid()) {
		LOG_ERROR("[Program] Program is invalid. Program(%p)", program);
		return 0;
	}

	unsigned int sizeInBytes = 0;
	const std::map<std::string, UniformData> activeUniforms = program->activeUniforms();
	for (const auto& it : activeUniforms) {
		sizeInBytes += UniformData::sizeInBytes(it.second.type);
	}

	return sizeInBytes;
}
