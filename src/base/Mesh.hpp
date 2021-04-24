#pragma once

#include "container/VectorView.hpp"

class RenderingDevice;
class Buffer;

class Mesh {
public:
    struct Params {
        VectorView<float> vertices;
        VectorView<float> normals;
        VectorView<float> uvs;
		VectorView<float> indices;
    };

    bool init(RenderingDevice& renderingDevice, const Mesh::Params& params);
    
    Buffer* buffer() const { return _buffer; }
    size_t count() const { return _count; }

private:
    Buffer* _buffer = nullptr;
    size_t _count = 0;
};