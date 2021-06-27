#pragma once

#include <vector>

#include "base/Mesh.hpp"

class RenderingDevice;
class Material;
class Program;

class ResourceManager {
public:
    static ResourceManager& instance();
    bool init(RenderingDevice& renderingDevice);

    Material* createMaterial(const char* programName);

    Mesh* createMesh(Mesh::Primitive primitive);
    Mesh* createMesh(const char* fileName);

private:
    RenderingDevice* _renderingDevice;
    std::vector<Material*> _materials;
    std::vector<Mesh*> _meshes;
};