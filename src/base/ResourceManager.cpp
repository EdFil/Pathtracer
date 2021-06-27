#include "ResourceManager.hpp"

#define FAST_OBJ_IMPLEMENTATION
#include <fast_obj.h>
#include <algorithm>

#include "Logger.hpp"
#include "base/Material.hpp"
#include "base/Mesh.hpp"
#include "file/FileManager.hpp"
#include "rendering/RenderingDevice.hpp"

namespace {
static ResourceManager* s_instance = nullptr;

constexpr Mesh::Vertex k_planeVertices[] = {
    Mesh::Vertex{-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}, Mesh::Vertex{1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    Mesh::Vertex{-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f}, Mesh::Vertex{1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f}};
constexpr unsigned int k_planeIndices[] = {0, 1, 2, 1, 0, 3};

}  // namespace

ResourceManager& ResourceManager::instance() {
    return *s_instance;
}

bool ResourceManager::init(RenderingDevice& renderingDevice) {
    if (s_instance != nullptr) {
        LOG_ERROR("[ResourceManager] Was already initialised");
        return false;
    }

    _renderingDevice = &renderingDevice;
    s_instance = this;
    LOG("[ResourceManager] Successfully initialised");
    return true;
}

Material* ResourceManager::createMaterial(const char* programName) {
    Program* program = _renderingDevice->getProgram(programName);
    if (program == nullptr) {
        LOG_ERROR("[ResourceManager] Could not find program with name \"%s\".");
        return nullptr;
    }

    Material* material = new Material();
    if (material->init(program)) {
        _materials.push_back(material);
        return material;
    }

    delete material;
    return nullptr;
}

Mesh* ResourceManager::createMesh(Mesh::Primitive primitive) {
    switch (primitive) {
        case Mesh::Primitive::Plane: return createMesh("models/plane.obj");
        case Mesh::Primitive::Sphere: return createMesh("models/sphere.obj");
        case Mesh::Primitive::Cube: return createMesh("models/cube.obj");
        case Mesh::Primitive::Suzanne: return createMesh("models/suzanne.obj");
    }

    return nullptr;
}

Mesh* ResourceManager::createMesh(const char* relativePath) {
    char fullPath[256];
    FileManager::instance()->fullPathForFile(relativePath, fullPath, sizeof(fullPath));
    fastObjMesh* objMesh = fast_obj_read(fullPath);
    if (objMesh == nullptr) {
        LOG_ERROR("[ResourceManager] Could not load \"%s\".", fullPath);
        return nullptr;
    }

    // Process and unwrap fastObjMesh to a faster memory layout to render
    std::vector<unsigned int> indices;
    std::vector<bool> dataExists(objMesh->position_count, false);
    std::vector<Mesh::Vertex> vertexData(objMesh->position_count);
    std::vector<Mesh::Vertex> duplicatedVertices;
    indices.reserve(objMesh->face_count * 3);
    duplicatedVertices.reserve(objMesh->position_count);

    for (unsigned int i = 0; i < objMesh->face_count * 3; ++i) {
        const fastObjIndex& index = objMesh->indices[i];
        Mesh::Vertex vertex;

        memcpy(&vertex.px, objMesh->positions + index.p * 3, sizeof(float) * 3);
        memcpy(&vertex.tx, objMesh->texcoords + index.t * 2, sizeof(float) * 2);
        memcpy(&vertex.nx, objMesh->normals + index.n * 3, sizeof(float) * 3);

        if (!dataExists[index.p]) {
            // New vertex
            vertexData[index.p] = vertex;
            dataExists[index.p] = true;
            indices.push_back(index.p - 1);
        } else if (vertexData[index.p] != vertex) {
            // We need to duplicate vertex so we don't break uvs
            auto it = std::find(duplicatedVertices.cbegin(), duplicatedVertices.cend(), vertex);
            if (it != duplicatedVertices.cend()) {
                // It was already duplicated, let's use it
                const int index = objMesh->position_count + (it - duplicatedVertices.cbegin()) - 1;
                indices.push_back(index);
            } else {
                // Let's create and add it to the duplicated list
                duplicatedVertices.push_back(vertex);
                const int index = objMesh->position_count + duplicatedVertices.size() - 2;
                indices.push_back(index);
            }
        } else {
            // Vertex already exists in the main vertex data
            indices.push_back(index.p - 1);
        }
    }

    vertexData.insert(vertexData.end(), duplicatedVertices.begin(), duplicatedVertices.end());
    fast_obj_destroy(objMesh);

    Mesh::Params meshParams;
    meshParams.vertices = VectorView<float>(&vertexData[1].px, (vertexData.size() - 1) * 8);
    meshParams.indices = VectorView<unsigned int>(indices.data(), indices.size());

    Mesh* mesh = new Mesh();
    if (mesh->init(*_renderingDevice, meshParams)) {
        _meshes.push_back(mesh);
        return mesh;
    }

    delete mesh;
    return nullptr;
}
