#pragma once

#include <string>

class Shader {
public:
    enum class Type { Vertex, Fragment };

    Shader(Type type, const std::string& source);
    virtual ~Shader();

protected:
    Type _type;
    std::string _source;
};