#pragma once

#include <string>
#include <vector>

class FileManager {
public:
    static void init();
    static void destroy();
    static FileManager* instance();

    FileManager();

    /** Load file into memory */
    std::vector<char> loadFile(const char fileName[]) const;

    /** */
    void setRootDir(const char rootDir[]);

private:
    std::string _rootDir;

    /** Constructs a full path for a given filename inside the provided array */
    void fullPathForFile(const char fileName[], char* buffer, size_t bufferSize) const;
};
