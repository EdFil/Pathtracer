#pragma once

#include <cstring>
#include "container/VectorView.hpp"

class IRenderingDevice;
class IBuffer;

class Mesh {
public:
    enum class Primitive { Plane, Sphere, Cube, Suzanne };

    struct Vertex {
        float px, py, pz;
        float tx, ty;
        float nx, ny, nz;

        bool operator==(const Vertex& rhs) const { return memcmp(this, &rhs, sizeof(Vertex)) == 0; }

        bool operator!=(const Vertex& rhs) const { return memcmp(this, &rhs, sizeof(Vertex)) != 0; }
    };

    struct Params {
        VectorView<float> vertices;
        VectorView<unsigned int> indices;
    };

    bool init(IRenderingDevice& renderingDevice, const Mesh::Params& params);

    IBuffer* buffer() const { return _buffer; }
    size_t count() const { return _count; }

private:
    IBuffer* _buffer = nullptr;
    size_t _count = 0;
};