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
    };

    bool init(RenderingDevice& renderingDevice, const Mesh::Params& params);
    
    Buffer* buffer() const { _buffer; }

private:
    Buffer* _buffer = nullptr;
};