#pragma once

#include <vector>

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
    std::vector<Material*> _materials;
    std::vector<Mesh*> _meshes;
};