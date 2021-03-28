#include "Sphere.hpp"

bool Sphere::hit(const Ray& ray, float min, float max, hit_record& rec) {
    Vec3f oc = ray.origin() - _position;
    auto a = ray.dir().length_squared();
    auto half_b = Vec3f::dot(oc, ray.dir());
    auto c = oc.length_squared() - _radius * _radius;
    auto discriminant = half_b * half_b - a * c;

    return discriminant > 0;
}