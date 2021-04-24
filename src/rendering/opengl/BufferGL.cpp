#include "rendering/opengl/BufferGL.hpp"

#include <glad/glad.h>
#include <cstring>

#include "Logger.hpp"

namespace {
	GLenum toGLUsage(Buffer::Usage usage) {
		switch (usage) {
		case Buffer::Usage::Stream:
			return GL_STREAM_DRAW;
		case Buffer::Usage::Static:
			return GL_STATIC_DRAW;
		case Buffer::Usage::Dynamic:
		default:
			return GL_DYNAMIC_DRAW;
		}
	}
}  // namespace

BufferGL::~BufferGL() {
	if (_handle != 0) glDeleteVertexArrays(1, &_handle);
	if (_vbo != 0) glDeleteBuffers(1, &_vbo);
	if (_ibo != 0) glDeleteBuffers(1, &_ibo);
}

bool BufferGL::init(const Buffer::Params& params) {
	if (params.type == Buffer::Type::Constant) {
		glGenBuffers(1, &_handle);
	}
	else {
		glGenVertexArrays(1, &_handle);
		glGenBuffers(1, &_vbo);
	}

	_type = params.type;
	_usage = params.usage;

	return true;
}

bool BufferGL::updateData(void* data, uint32_t size) {
	if (_handle == 0 || (_type != Buffer::Type::Constant && _vbo == 0)) {
		LOG_ERROR("[BufferGL] Trying to call updateData without valid buffer objects. Failing...");
		return false;
	}

	if (_type == Buffer::Type::Constant) {
		glBindVertexArray(_handle);
		glBufferData(GL_UNIFORM_BUFFER, size, data, toGLUsage(_usage));
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, _handle, 0, size);
	}
	else {
		glBindVertexArray(_handle);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, size, data, toGLUsage(_usage));
	}

	return true;
}

void BufferGL::updateAttribute(uint32_t index, uint32_t size, uint32_t stride, uint32_t dataBegin) {
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)(dataBegin * sizeof(float)));
}

void BufferGL::bind() {
	glBindVertexArray(_handle);
}