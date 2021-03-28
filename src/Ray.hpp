#pragma once

#include "Vec3f.hpp"

class Ray {
public:
    Ray() = default;
    Ray(Vec3f origin, Vec3f dir) : _origin(origin), _dir(dir) {}

    const Vec3f& origin() const { return _origin; }
    const Vec3f& dir() const { return _dir; }
    Vec3f at(float t) { return _origin + t * _dir; }

private:
    Vec3f _origin{0.0f};
    Vec3f _dir{0.0f};
};