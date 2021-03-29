#include "rendering/OpenGL/ProgramGL.hpp"

#include <glad/glad.h>
#include "rendering/RenderingDevice.hpp"
#include "rendering/OpenGL/ShaderGL.hpp"

ProgramGL::ProgramGL(const std::string& vertexShader, const std::string& fragmentShader) : Program(vertexShader, fragmentShader) {
    
}

ProgramGL::~ProgramGL() {
}

void ProgramGL::createProgram(Device& device) {
    //_vertexShader = (ShaderGL*)device.createShader(Shader::Type::Vertex, _vertexShaderSource);
    //_fragmentShader = (ShaderGL*)device.createShader(Shader::Type::Fragment, _fragmentShaderSource);

    //_id = glCreateProgram();
    //if (_id == 0) {
    //    LOG_ERROR("Error creating program");
    //    return;
    //}

    //glAttachShader(_id, _vertexShader->id());
    //glAttachShader(_id, _fragmentShader->id());
    //glLinkProgram(_id);

    //GLint status = 0;
    //glGetProgramiv(_id, GL_LINK_STATUS, &status);
    //if (GL_FALSE == status) {
    //    LOG_ERROR("{}: failed to link program");
    //    deleteProgram();
    //}
}

void ProgramGL::deleteProgram() {
    if (_id) {
        glDeleteProgram(_id);
        _id = 0;
    }
}