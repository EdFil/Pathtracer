#pragma once

#include "rendering/Buffer.hpp"

#include <glad/glad.h>

class BufferGL : public Buffer {
public:
    BufferGL(Type type, Usage usage);
    ~BufferGL();

    void updateData(void* data, uint32_t size) override;

private:
    GLuint* _buffer = nullptr;
    char* _data = nullptr;
    uint32_t _dataSize = 0;
};