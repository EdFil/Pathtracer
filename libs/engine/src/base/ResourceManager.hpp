#pragma once

#include <EASTL/vector.h>

#include "base/Mesh.hpp"

class IRenderingDevice;
class Material;
class IProgram;

class ResourceManager {
public:
    static ResourceManager& instance();
    bool init(IRenderingDevice& renderingDevice);

    Material* createMaterial(const char* programName);

    Mesh* createMesh(Mesh::Primitive primitive);
    Mesh* createMesh(const char* fileName);

private:
    IRenderingDevice* _renderingDevice;
    eastl::vector<Material*> _materials;
    eastl::vector<Mesh*> _meshes;
};