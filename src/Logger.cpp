#include "Logger.hpp"

#include <SDL_stdinc.h>
#include <SDL_assert.h>
#include <cstdarg>
#include <fstream>
#include <string>

namespace {
const int k_maxSizeLogMessage = 4096;
const char k_loggerFileName[] = "log.txt";
Logger* s_instance = nullptr;
}  // namespace

bool Logger::init() {
    destroy();
    s_instance = new Logger();

    s_instance->_logFile.open(k_loggerFileName, std::ofstream::trunc);
    return s_instance->_logFile.is_open();
}

void Logger::destroy() {
    if (s_instance != nullptr) {
        s_instance->_logFile.close();
        delete s_instance;
        s_instance = nullptr;
    }
}

void Logger::log(const char* fileName, int lineNumber, const char* format, ...) {
    if (!s_instance) return;

    char buffer[k_maxSizeLogMessage];
    uint32_t usedBytes = 0;

    va_list args;
    va_start(args, format);
    usedBytes = SDL_vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    usedBytes += 2;
    if (usedBytes > sizeof(buffer)) usedBytes = sizeof(buffer) - 2;

    buffer[usedBytes - 2] = '\n';
    buffer[usedBytes - 1] = '\0';

    printf("%s", buffer);
    s_instance->_logFile.write(buffer, usedBytes - 1);
    s_instance->_logFile.flush();
}

void Logger::logError(const char* fileName, int lineNumber, const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(fileName, lineNumber, format, args);
    va_end(args);
}
