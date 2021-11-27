#include "rendering/opengl/UniformBufferGL.hpp"

#include <glad/glad.h>

#include "Logger.hpp"
#include "rendering/opengl/UtilsGL.hpp"

// ---------------------------------------------
// -------------  UniformBufferGL  -------------
// ---------------------------------------------

bool UniformBufferGL::init(uint32_t bindingPoint, uint32_t sizeInBytes) {
    _bindingPoint = bindingPoint;
    _sizeInBytes = sizeInBytes;

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

bool UniformBufferGL::updateData(const void* data, uint32_t sizeInBytes) {
    glBindBuffer(GL_UNIFORM_BUFFER, _handle);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeInBytes, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    return !UtilsGL::CheckGLError("[UniformBufferGL] updateData()");
}

void UniformBufferGL::bind() {
    glBindBufferBase(GL_UNIFORM_BUFFER, _bindingPoint, _handle);
}

// ----------------------------------------------------
// -------------  UniformBufferManagerGL  -------------
// ----------------------------------------------------

IUniformBuffer* UniformBufferManagerGL::createUniformBuffer(uint32_t bindingPoint, uint32_t sizeInBytes) {
    std::unique_ptr<UniformBufferGL> uniformBuffer = std::make_unique<UniformBufferGL>();
    if (uniformBuffer == nullptr || !uniformBuffer->init(bindingPoint, sizeInBytes)) {
        LOG_ERROR("[UniformBufferManagerGL] Failed to create UniformBuffer(%p). BindingPoint(%u) Size(%u)", uniformBuffer.get(), bindingPoint, sizeInBytes);
        return nullptr;
    }

    const auto it = _buffers.insert({uniformBuffer->handle(), std::move(uniformBuffer)});
    if (!it.second) {
        LOG_ERROR("[UniformBufferManagerGL] Failed to insert UniformBuffer into map. Memory allocation failed?");
        return nullptr;
    }

    return it.first->second.get();
}

void UniformBufferManagerGL::deleteUniformBuffer(IUniformBuffer* uniformBuffer) {
    if (uniformBuffer == nullptr) {
        LOG_WARN("[UniformBufferManagerGL] Trying to delete nullptr uniformBuffer. Skipping...");
        return;
    }

    _buffers.erase(uniformBuffer->handle());
}
