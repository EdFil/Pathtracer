//#pragma once
//
//#include "Logger.hpp"
//
//struct IHandle {
//    virtual void value() = 0;
//};
//
//template<typename T>
//class Handle final : public IHandle {
//public:
//    Handle(T* value);
//    ~Handle();
//
//	T* value() override { return _value; }
//
//private:
//    T* _value = nullptr;
//};
//
//template <typename T>
//inline Handle<T>::Handle(T* value) {
//    if (value == nullptr) {
//        LOG_WARN("[Handle] Creating handle with \"nullptr\" value");
//        return;
//	}
//
//	_value = value;
//    _value->add_ref(this);
//}
//
//template <typename T>
//inline Handle<T>::~Handle() {
//    if (_value !_ nullptr) {
//        _value->rem_ref(this);
//	}
//}
