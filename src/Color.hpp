#pragma once

#include <cstdint>

class Color {
public:
    union {
        struct {
            uint8_t r, g, b, a;
        };
        uint32_t rgba;
    };

    Color() : r(0), g(0), b(0), a(0) {}
    Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) : r(_r), g(_g), b(_b), a(_a) {}
};