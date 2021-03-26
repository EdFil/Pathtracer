#pragma once

#include <fstream>

class Logger {
public:
    static bool init();
    static void destroy();
    
    static void log(const char* fileName, int lineNumber, const char* format, ...);

private:
    std::ofstream _logFile;
};

#define LOG(MESSAGE, ...) Logger::log(__FILE__, __LINE__, MESSAGE, ##__VA_ARGS__)
#define LOG_WARN(MESSAGE, ...) Logger::log(__FILE__, __LINE__, MESSAGE, ##__VA_ARGS__)
#define LOG_ERROR(MESSAGE, ...) Logger::log(__FILE__, __LINE__, MESSAGE, ##__VA_ARGS__)