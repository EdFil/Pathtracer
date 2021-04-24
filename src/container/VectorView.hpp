#pragma once

#include <cstddef>

template<typename T>
class VectorView {
public:
    VectorView() = default;
    VectorView(const T* data, size_t size) : _data(data), _size(size) {}

    size_t size() const { return _size; }
    bool empty() const { return _data == nullptr || _size == 0; }
    
    const T* cbegin() const { return _data; }
    T* begin() { return _data; }
    const T* cend() const { return _data + _size; }
    T* end() const { return _data + _size; }


private:
    const T* _data = nullptr;
    size_t _size = 0;
};