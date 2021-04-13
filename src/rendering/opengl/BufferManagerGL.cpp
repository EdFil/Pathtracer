#include "rendering\opengl\BufferManagerGL.hpp"

Buffer* BufferManagerGL::createBuffer(const Buffer::Params& params)
{
	BufferGL* buffer = new BufferGL();
	if (!buffer->init(params))
		return nullptr;

	_buffers[buffer->id()] = buffer;
	return buffer;
}

Buffer* BufferManagerGL::buffer(uint32_t id) const
{
	const auto it = _buffers.find(id);
	if (it == _buffers.cend())
		return nullptr;

	return it->second;
}
