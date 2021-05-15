#include "base/Mesh.hpp"

#include <cstring>
#include <vector>

#include "Logger.hpp"
#include "rendering/RenderingDevice.hpp"

bool Mesh::init(RenderingDevice& renderingDevice, const Mesh::Params& params) {
    if (params.vertices.empty()) {
        LOG_ERROR("[Mesh] init: Cannot create a mesh with no vertices");
        return false;
    }

    Buffer::Mode bufferMode = params.indices.size() == 0 ? Buffer::Mode::Vertex : Buffer::Mode::VertexElementPair;
    _buffer = renderingDevice.createBuffer(bufferMode);
    if (_buffer == nullptr) {
        LOG_ERROR("[Mesh] init: Rendering buffer creation failed");
        return false;
    }

    buffer()->bind();
    if (!_buffer->updateData(Buffer::Target::Array, Buffer::Usage::Static, params.vertices.cbegin(), params.vertices.size() * sizeof(float)) ||
        (bufferMode == Buffer::Mode::VertexElementPair && !_buffer->updateData(Buffer::Target::Element, Buffer::Usage::Static, params.indices.cbegin(), params.indices.size() * sizeof(unsigned int)))) {
        LOG_ERROR("[Mesh] init: Could not set vertex data to GPU");
        return false;
    }

    _count = bufferMode == Buffer::Mode::Vertex ? params.vertices.size() : params.indices.size();
    _buffer->updateAttribute(0, 3, 8 * sizeof(float), 0);
    _buffer->updateAttribute(1, 2, 8 * sizeof(float), 3);
    _buffer->updateAttribute(2, 3, 8 * sizeof(float), 5);
    return true;
}