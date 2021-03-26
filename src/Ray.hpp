#pragma once

#include "Vec3.hpp"

class Ray {
public:
    Vec3 origin;
    Vec3 dir;

    Ray(Vec3 origin, Vec3 dir) : origin(origin), dir(dir) {}

    Vec3 at(float t) { return origin + t * dir; }
};