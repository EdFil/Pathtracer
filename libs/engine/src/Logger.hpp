#pragma once

struct SDL_RWops;

class Logger {
public:
    static bool init();
    static void destroy();

    static void log(const char* fileName, int lineNumber, const char* format, ...);
    static void logError(const char* fileName, int lineNumber, const char* format, ...);

private:
    SDL_RWops* _logFileHandle;
};

#define LOG(MESSAGE, ...) Logger::log(__FILE__, __LINE__, MESSAGE, ##__VA_ARGS__)
#define LOG_WARN(MESSAGE, ...) Logger::log(__FILE__, __LINE__, "\x1b[33m" MESSAGE "\x1b[0m", ##__VA_ARGS__)
#define LOG_ERROR(MESSAGE, ...) Logger::logError(__FILE__, __LINE__, "\x1b[31m" MESSAGE "\x1b[0m", ##__VA_ARGS__)