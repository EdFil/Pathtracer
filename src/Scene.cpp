#include "Scene.hpp"

#include "Color.hpp"
#include "rendering/Renderer.hpp"
#include "Sphere.hpp"

Scene::Scene(Renderer& renderer) : _renderer(renderer) {
}

Scene::~Scene() {
    for (Object* object : _objects) {
        delete object;
    }
}

bool Scene::init() {
    //Vec2i size = _renderer.bufferSize();

    //_objects.push_back(new Sphere(Vec3f{size.x / 2.0f, size.y / 2.0f, 10.0f}, size.x * 0.1f));

    //_index = 0;
    //_pixelsToTrace.resize(size.x * size.y);
    //for (int i = 0; i < _pixelsToTrace.size(); ++i) {
    //    _pixelsToTrace[i] = i;
    //}

    //std::random_shuffle(_pixelsToTrace.begin(), _pixelsToTrace.end());
    return true;
}

void Scene::draw() {
    //int width = _renderer.bufferSize().x;
    //for (int i = 0; i < width; ++i) {
    //    if (_index >= _pixelsToTrace.size()) return;

    //    int flatPixelPos = _pixelsToTrace[_index++];
    //    Vec2i size = _renderer.bufferSize();
    //    Vec2i pixel{flatPixelPos % size.x, int(flatPixelPos / size.x)};
    //    Ray ray(Vec3f{float(pixel.x), float(pixel.y), -10.0f}, Vec3f{0.0f, 0.0f, 1.0f});

    //    for (Object* object : _objects) {
    //        hit_record rec;
    //        if (object->hit(ray, 0.0f, 100.0f, rec)) {
    //            _renderer.drawPixel(Color(255, 0, 0, 255), pixel.x, pixel.y);
    //        } else {
    //            _renderer.drawPixel(Color(255, 255, 255, 100), pixel.x, pixel.y);
    //        }
    //    }
    //}
}