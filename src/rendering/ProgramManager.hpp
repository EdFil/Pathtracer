#pragma once

#include <string>
#include <vector>

class Program;
class Renderer;

class ProgramManager {
public:
    ProgramManager(Renderer& renderer);

    bool init();

    Program* createProgram(const std::string& vertexShader, const std::string& fragmentShader);

private:
    Renderer& _renderer;
    std::vector<Program*> _programs;
};