#pragma once

#include <cstdint>

struct EntityID {
public:
    uint64_t Value = 0;

    EntityID() = default;
    explicit EntityID(uint64_t id) : Value(id) { /* Empty */ }

    bool isValid() const { return Value != 0; }
    bool operator==(const EntityID& rhs) const { return Value == rhs.Value; }
    bool operator!=(const EntityID& rhs) const { return Value == rhs.Value; }
};