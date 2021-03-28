#pragma once

#include <vector>

class Object;
class Renderer;

class Scene {
public:
    Scene(Renderer& renderer);
    ~Scene();

    bool init();
    void draw();
private:
    Renderer& _renderer;
    std::vector<int> _pixelsToTrace;
    int _index = 0;
    std::vector<Object*> _objects;
};