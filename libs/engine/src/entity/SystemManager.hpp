#pragma once

#include <cstdint>

class ArchetypeChunk {
    const uint8_t k_ChunkSize = 32;
};

class Archetype {

};

class ArchetypeManager {

};

class EntityManager {

};

class ISystem {
    virtual void initialize() = 0;
    virtual void start() = 0;
    virtual void run() = 0;
    virtual void cleanUp() = 0;
};
