#pragma once

#include <EASTL/string.h>
#include <EASTL/vector.h>

class FileManager {
public:
    static void init();
    static void destroy();
    static FileManager* instance();

    FileManager();

    /** Load file into memory */
    eastl::vector<char> loadFile(const char fileName[]) const;

    /** Constructs a full path for a given filename inside the provided array */
    void fullPathForFile(const char fileName[], char* buffer, size_t bufferSize) const;

    /** */
    void setRootDir(const char rootDir[]);

private:
    eastl::string _rootDir;
};
