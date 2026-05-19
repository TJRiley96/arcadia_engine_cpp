#pragma once

//const char* LOG_DIRECTORY = "logs";

// Built-in includes
#include <string>
#include <cstdarg>

// Third-party includes

// Local includes

class Logging {
public:
    static void LogInfo(const char* message, ...);
    static void LogWarning(const char* message, ...);
    static void LogError(const char* message, ...);
    static void LogInfo(const std::string& message) { LogInfo(message.c_str()); }
    static void LogWarning(const std::string& message) { LogWarning(message.c_str()); }
    static void LogError(const std::string& message) { LogError(message.c_str()); }

    static void SanityCheck();
};