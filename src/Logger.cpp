#include "Logger.hpp"

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_assert.h>
#include <SDL2/SDL_rwops.h>
#include <SDL2/SDL_log.h>
#include <EASTL/string.h>

namespace {
const int k_maxSizeLogMessage = 4096;
const char k_loggerFileName[] = "log.txt";
Logger* s_instance = nullptr;
}  // namespace

bool Logger::init() {
    destroy();
    s_instance = new Logger();

#if DEBUG
    SDL_LogSetAllPriority(SDL_LogPriority::SDL_LOG_PRIORITY_VERBOSE);
#endif

    s_instance->_logFileHandle = SDL_RWFromFile(k_loggerFileName, "w+");
    return s_instance->_logFileHandle != nullptr;
}

void Logger::destroy() {
    if (s_instance != nullptr) {
        SDL_RWclose(s_instance->_logFileHandle);
        delete s_instance;
        s_instance = nullptr;
    }
}

void Logger::log(const char* /*fileName*/, int /*lineNumber*/, const char* format, ...) {
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
    SDL_RWwrite(s_instance->_logFileHandle, buffer, usedBytes - 1, 1);
}

void Logger::logError(const char* fileName, int lineNumber, const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(fileName, lineNumber, format, args);
    va_end(args);
}
