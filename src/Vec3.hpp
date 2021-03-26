#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>

class Vec3 {
public:
    float x, y, z;

    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3(float value) : x(value), y(value), z(value) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3(const Vec3& other) : x(other.x), y(other.y), z(other.z) {}
    Vec3(Vec3&& other) noexcept : x(other.x), y(other.y), z(other.z) {}

    Vec3& operator=(const Vec3& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Vec3& operator=(Vec3&& other) noexcept {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    float operator[](uint32_t i) const {
        assert(i < 3);
        return *(&x + i);
    }

    Vec3& operator=(float rhs) {
        x = y = z = rhs;
        return *this;
    }

    bool operator==(const Vec3& rhs) const { return (x == rhs.x && y == rhs.y && z == rhs.z); }

    bool operator!=(const Vec3& rhs) const { return (x != rhs.x || y != rhs.y || z != rhs.z); }

    Vec3 operator+(const Vec3& rhs) const { return Vec3(x + rhs.x, y + rhs.y, z + rhs.z); }

    Vec3 operator-(const Vec3& rhs) const { return Vec3(x - rhs.x, y - rhs.y, z - rhs.z); }

    Vec3 operator*(float rhs) const { return Vec3(x * rhs, y * rhs, z * rhs); }

    Vec3 operator/(float val) const {
        assert(val != 0.0);

        float fInv = 1.0f / val;
        return Vec3(x * fInv, y * fInv, z * fInv);
    }

    const Vec3& operator+() const { return *this; }

    Vec3 operator-() const { return Vec3(-x, -y, -z); }

    friend Vec3 operator*(float lhs, const Vec3& rhs) { return Vec3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z); }

    friend Vec3 operator/(float lhs, const Vec3& rhs) { return Vec3(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z); }

    friend Vec3 operator+(const Vec3& lhs, float rhs) { return Vec3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs); }

    friend Vec3 operator+(float lhs, const Vec3& rhs) { return Vec3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z); }

    friend Vec3 operator-(const Vec3& lhs, float rhs) { return Vec3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs); }

    friend Vec3 operator-(float lhs, const Vec3& rhs) { return Vec3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z); }

    Vec3& operator+=(const Vec3& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;

        return *this;
    }

    Vec3& operator+=(float rhs) {
        x += rhs;
        y += rhs;
        z += rhs;

        return *this;
    }

    Vec3& operator-=(const Vec3& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;

        return *this;
    }

    Vec3& operator-=(float rhs) {
        x -= rhs;
        y -= rhs;
        z -= rhs;

        return *this;
    }

    Vec3& operator*=(float rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;

        return *this;
    }

    Vec3& operator/=(float rhs) {
        assert(rhs != 0.0f);

        x /= rhs;
        y /= rhs;
        z /= rhs;

        return *this;
    }

    float dot(const Vec3& other) const { return x * other.x + y * other.y + z * other.z; }

    Vec3 cross(const Vec3& other) const {
        return Vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }

    float length_squared() const { return x * x + y * y + z * z; }

    float length() const { return std::sqrtf(length_squared()); }

    void normalize() {
        float len = length();
        x /= len;
        y /= len;
        z /= len;
    }
};