#pragma once

#include "Object.hpp"

class Sphere : public Object {
public:
    Sphere() = default;
    Sphere(const Vec3f& position, float radius) : _position(position), _radius(radius) {}

    bool hit(const Ray& ray, float min, float max, hit_record& rec) override;

private:
    Vec3f _position{0.0f};
    float _radius{0.0f};
};