#pragma once

#include <glm/vec3.hpp>

class Material;
class Mesh;

class Entity {
public:
    glm::vec3 _position, _scale, _rotation;

private:
    Material* _material;
    Mesh* _mesh;
};