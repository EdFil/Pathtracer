#pragma once

#include "Ray.hpp"
#include "Vec3f.hpp"

struct hit_record {
    Vec3f hitPoint;
    Vec3f normal;
    float t;
};

class Object {
    virtual bool hit(const Ray& ray, float min, float max, hit_record& rec) = 0;
};