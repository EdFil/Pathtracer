#include "rendering/opengl/UniformBufferGL.hpp"

#include <glad/glad.h>

#include "Logger.hpp"
#include "rendering/opengl/UtilsGL.hpp"

bool UniformBufferGL::init(unsigned int bindingPoint, unsigned int sizeInBytes) {
    _bindingPoint = bindingPoint;

    // Create and allocate buffer
    glGenBuffers(1, &_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, _handle);
    glBufferData(GL_UNIFORM_BUFFER, sizeInBytes, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    if (UtilsGL::CheckGLError("[UniformBufferGL] init()")) return false;

    // Link binding point to uniform buffer and define it's size
    glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, _handle, 0, sizeInBytes);
    if (UtilsGL::CheckGLError("[UniformBufferGL] init()")) return false;

    LOG("[UniformBufferGL] UniformBuffer created. Handle(%d) BindingPoint(%u) Size(%u)", _handle, bindingPoint, sizeInBytes);
    return true;
}
bool UniformBufferGL::updateData(const void* data, unsigned int sizeInBytes) {
    UtilsGL::CheckGLError("[UniformBufferGL] updateData()");
    glBindBuffer(GL_UNIFORM_BUFFER, _handle);
    UtilsGL::CheckGLError("[UniformBufferGL] updateData()");
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeInBytes, data);
    UtilsGL::CheckGLError("[UniformBufferGL] updateData()");
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    return !UtilsGL::CheckGLError("[UniformBufferGL] updateData()");
}
