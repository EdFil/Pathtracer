#include "base/Mesh.hpp"

#include <vector>

#include "Logger.hpp"
#include "rendering/RenderingDevice.hpp"

bool Mesh::init(RenderingDevice& renderingDevice, const Mesh::Params& params) {
    if (params.vertices.empty()) {
        LOG_ERROR("[Mesh] init: Cannot create a mesh with no vertices");
        return false;
    }

    Buffer::Params bufferParams;
    std::vector<float> unifiedData;
    if (!params.normals.empty() && !params.uvs.empty()) {
        bufferParams.layout = Buffer::DataLayout::Pos3Norm3Uv2;
        unifiedData.resize(params.vertices.size() + params.normals.size() + params.uvs.size());
        for (size_t vertexIndex = 0, uvIndex = 0; vertexIndex < params.vertices.size() && uvIndex < params.uvs.size(); vertexIndex += 3, uvIndex += 2) {
            memcpy(unifiedData.data() + vertexIndex, params.vertices.cbegin() + vertexIndex, sizeof(float) * 3);
            memcpy(unifiedData.data() + vertexIndex, params.normals.cbegin() + vertexIndex, sizeof(float) * 3);
            memcpy(unifiedData.data() + vertexIndex, params.uvs.cbegin() + vertexIndex, sizeof(float) * 2);
        }

    } else if (!params.normals.empty()) {
        bufferParams.layout = Buffer::DataLayout::Pos3Norm3;
        unifiedData.resize(params.vertices.size() + params.normals.size());
        for (size_t vertexIndex = 0; vertexIndex < params.vertices.size(); vertexIndex += 3) {
            memcpy(unifiedData.data() + vertexIndex, params.vertices.cbegin() + vertexIndex, sizeof(float) * 3);
            memcpy(unifiedData.data() + vertexIndex, params.normals.cbegin() + vertexIndex, sizeof(float) * 3);
        }
    } else {
        unifiedData.resize(params.vertices.size());
        memcpy(unifiedData.data(), params.vertices.cbegin(), sizeof(float) * params.vertices.size());
    }

    bufferParams.data = unifiedData.data();
    bufferParams.size = unifiedData.size() * sizeof(float);
    _buffer = renderingDevice.createBuffer(bufferParams);
    if (_buffer == nullptr) {
        LOG_ERROR("[Mesh] init: Rendering buffer creation failed");
        return false;
    }

    return true;
}