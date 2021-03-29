#pragma once

#include <string>

class Program {
public:
    Program(const std::string& vertexShader, const std::string& fragmentShader);
    virtual ~Program();

protected:
    std::string _vertexShaderSource;
    std::string _fragmentShaderSource;
};