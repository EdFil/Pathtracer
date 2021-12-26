#pragma once

#include <vector>

#include "base/Mesh.hpp"
#include "base/Material.hpp"


class IRenderingDevice;
class IProgram;

class ResourceManager {
public:
    static ResourceManager& instance();
    bool init(IRenderingDevice& renderingDevice);

	MaterialManager& materialManager() { return _materialManager; }

    Mesh* createMesh(Mesh::Primitive primitive);
    Mesh* createMesh(const char* fileName);

private:
    IRenderingDevice* _renderingDevice;
	MaterialManager _materialManager;
    std::vector<Mesh*> _meshes;
};