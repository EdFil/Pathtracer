#include "base/Mesh.hpp"

#include <vector>
#include <cstring>

#include "Logger.hpp"
#include "rendering/RenderingDevice.hpp"

namespace {
	void packData(std::vector<float>& vec, const VectorView<float>& data, size_t& index) {
		if (!data.empty()) {
			memcpy(vec.data() + index, data.cbegin(), sizeof(float) * data.size());
			index += data.size();
		}
	}

	void setAttribute(Buffer* buffer, VectorView<float> data, unsigned int size, unsigned int& attribute, unsigned int& dataBegin) {
		if (!data.empty()) {
			buffer->updateAttribute(attribute++, size, 0, dataBegin);
			dataBegin += data.size();
		}
	}
}

bool Mesh::init(RenderingDevice& renderingDevice, const Mesh::Params& params) {
    if (params.vertices.empty()) {
        LOG_ERROR("[Mesh] init: Cannot create a mesh with no vertices");
        return false;
    }

    _buffer = renderingDevice.createBuffer(Buffer::Mode::Vertex);
    if (_buffer == nullptr) {
        LOG_ERROR("[Mesh] init: Rendering buffer creation failed");
        return false;
    }

    std::vector<float> unifiedData;
	size_t currentIndex = 0;
	unifiedData.resize(params.vertices.size() + params.normals.size() + params.uvs.size());
	packData(unifiedData, params.vertices, currentIndex);
	packData(unifiedData, params.normals, currentIndex);
	packData(unifiedData, params.uvs, currentIndex);

	buffer()->bind();
	if (!_buffer->updateData(Buffer::Target::Array, Buffer::Usage::Static, unifiedData.data(), unifiedData.size() * sizeof(float))) {
		LOG_ERROR("[Mesh] init: Could not set vertex data to GPU");
		return false;
	}

	_count = params.vertices.size() / 3;
	unsigned int attribute = 0;
	unsigned int dataBegin = 0;
	setAttribute(_buffer, params.vertices, 3, attribute, dataBegin);
	setAttribute(_buffer, params.normals, 3, attribute, dataBegin);
	setAttribute(_buffer, params.uvs, 2, attribute, dataBegin);
    buffer()->unbind();
    return true;
}