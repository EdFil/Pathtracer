#include "FileManager.hpp"

#include <SDL_filesystem.h>
#include <sys/stat.h>
#include <fstream>

#include "Logger.hpp"

#define RESOURCES_FOLDER "res"
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
    if (s_fileManager != nullptr) {
        delete s_fileManager;
    }
}

FileManager* FileManager::instance() {
    return s_fileManager;
}

FileManager::FileManager() {
    char* basePath = SDL_GetBasePath();
    if (basePath != nullptr) {
        _rootDir.assign(basePath);
        SDL_free(basePath);
    }
}

std::vector<char> FileManager::loadFile(const char fileName[]) const {
    char fullPath[MAX_PATH];
    fullPathForFile(fileName, fullPath, MAX_PATH);
    std::vector<char> contents;

    std::FILE* fp = std::fopen(fullPath, "rb");
    if (fp) {
        std::fseek(fp, 0, SEEK_END);
        size_t size = std::ftell(fp);
        std::rewind(fp);

        contents.resize(size + 1);
        std::fread(contents.data(), size, size, fp);
        std::fclose(fp);
        contents[size] = '\0';
    } else {
        LOG_WARN("[FileManager] Could not find/open file \"%s\"", fullPath);
    }

    return contents;
}

void FileManager::fullPathForFile(const char fileName[], char* buffer, size_t bufferSize) const {
    snprintf(buffer, bufferSize, "%s%s%s%s", _rootDir.c_str(), RESOURCES_FOLDER, FOLDER_SEP, fileName);
}

void FileManager::setRootDir(const char rootDir[]) {
    if (rootDir != nullptr) {
        _rootDir.assign(rootDir);
    }
}
