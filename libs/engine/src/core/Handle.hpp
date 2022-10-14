#pragma once

#include "Logger.hpp"

struct IHandle {
};

template <typename T>
class Handle final : public IHandle
{
public:
    Handle(T* value);
    ~Handle();

    T* value() { return _value; }

private:
    T* _value = nullptr;
};

template <typename T>
inline Handle<T>::Handle(T* value)
{
    if (value == nullptr) {
        LOG_WARN("[Handle::Handle] Creating handle with \"nullptr\" value");
        return;
    }

    _value = value;
    _value->add_ref(this);
}

template <typename T>
inline Handle<T>::~Handle()
{
    if (_value != nullptr) {
        _value->rem_ref(this);
    }
}
