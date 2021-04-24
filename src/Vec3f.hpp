#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>

class Vec3f {
public:
    float x, y, z;

    Vec3f() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3f(float value) : x(value), y(value), z(value) {}
    Vec3f(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3f(const Vec3f& other) : x(other.x), y(other.y), z(other.z) {}
    Vec3f(Vec3f&& other) noexcept : x(other.x), y(other.y), z(other.z) {}

    Vec3f& operator=(const Vec3f& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Vec3f& operator=(Vec3f&& other) noexcept {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    float operator[](uint32_t i) const {
        assert(i < 3);
        return *(&x + i);
    }

    Vec3f& operator=(float rhs) {
        x = y = z = rhs;
        return *this;
    }

    bool operator==(const Vec3f& rhs) const { return (x == rhs.x && y == rhs.y && z == rhs.z); }

    bool operator!=(const Vec3f& rhs) const { return (x != rhs.x || y != rhs.y || z != rhs.z); }

    Vec3f operator+(const Vec3f& rhs) const { return Vec3f(x + rhs.x, y + rhs.y, z + rhs.z); }

    Vec3f operator-(const Vec3f& rhs) const { return Vec3f(x - rhs.x, y - rhs.y, z - rhs.z); }

    Vec3f operator*(float rhs) const { return Vec3f(x * rhs, y * rhs, z * rhs); }

    Vec3f operator/(float val) const {
        assert(val != 0.0);

        float fInv = 1.0f / val;
        return Vec3f(x * fInv, y * fInv, z * fInv);
    }

    const Vec3f& operator+() const { return *this; }

    Vec3f operator-() const { return Vec3f(-x, -y, -z); }

    friend Vec3f operator*(float lhs, const Vec3f& rhs) { return Vec3f(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z); }

    friend Vec3f operator/(float lhs, const Vec3f& rhs) { return Vec3f(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z); }

    friend Vec3f operator+(const Vec3f& lhs, float rhs) { return Vec3f(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs); }

    friend Vec3f operator+(float lhs, const Vec3f& rhs) { return Vec3f(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z); }

    friend Vec3f operator-(const Vec3f& lhs, float rhs) { return Vec3f(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs); }

    friend Vec3f operator-(float lhs, const Vec3f& rhs) { return Vec3f(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z); }

    Vec3f& operator+=(const Vec3f& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;

        return *this;
    }

    Vec3f& operator+=(float rhs) {
        x += rhs;
        y += rhs;
        z += rhs;

        return *this;
    }

    Vec3f& operator-=(const Vec3f& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;

        return *this;
    }

    Vec3f& operator-=(float rhs) {
        x -= rhs;
        y -= rhs;
        z -= rhs;

        return *this;
    }

    Vec3f& operator*=(float rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;

        return *this;
    }

    Vec3f& operator/=(float rhs) {
        assert(rhs != 0.0f);

        x /= rhs;
        y /= rhs;
        z /= rhs;

        return *this;
    }

    static float dot(const Vec3f& lhs, const Vec3f& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }

    Vec3f cross(const Vec3f& other) const {
        return Vec3f(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }

    float length_squared() const { return x * x + y * y + z * z; }

    float length() const { return sqrtf(length_squared()); }

    void normalize() {
        float len = length();
        x /= len;
        y /= len;
        z /= len;
    }
};