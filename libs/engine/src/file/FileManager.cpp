#include "FileManager.hpp"

#include <SDL2/SDL_filesystem.h>
#include <SDL2/SDL_rwops.h>

#include "Logger.hpp"
#include "generated/CMakeConstants.h"

#define MAX_PATH 512

#ifdef _WIN32
#define FOLDER_SEP "\\"
#else
#define FOLDER_SEP "/"
#endif  // _WIN32

static FileManager* s_fileManager = nullptr;

void FileManager::init() {
    if (s_fileManager == nullptr) {
        s_fileManager = new FileManager();
    }
}

void FileManager::destroy() {
    delete s_fileManager;
}

FileManager* FileManager::instance() {
    return s_fileManager;
}

FileManager::FileManager() {
    _rootDir = CMakeConstants::EngineResourcesDir;
}

eastl::vector<char> FileManager::loadFile(const char fileName[]) const {
    char fullPath[MAX_PATH];
    fullPathForFile(fileName, fullPath, MAX_PATH);
    eastl::vector<char> contents;

    SDL_RWops* Handle = SDL_RWFromFile(fullPath, "rb");
    if (Handle) {
        int64_t size = SDL_RWsize(Handle);
        contents.resize(size + 1);

        int read = SDL_RWread(Handle, contents.data(), size, 1);
        SDL_RWclose(Handle);
    } else {
        LOG_WARN("[FileManager] File open error. %s", SDL_GetError());
    }

    return contents;
}

void FileManager::fullPathForFile(const char fileName[], char* buffer, size_t bufferSize) const {
    snprintf(buffer, bufferSize, "%s%s%s", _rootDir.c_str(), FOLDER_SEP, fileName);
}

void FileManager::setRootDir(const char rootDir[]) {
    if (rootDir != nullptr) {
        _rootDir.assign(rootDir);
    }
}
