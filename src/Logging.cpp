#include "Logging.h"

// Built-in includes
#include <iostream>
#include <chrono>
#include <iomanip>

// Third-party includes


// Local includes


void Logging::SanityCheck() {
    LogInfo("Logging system is working correctly.");
    LogWarning("This is a warning message for sanity check.");
    LogError("This is an error message for sanity check.");
}


void Logging::LogInfo(const char* message, ...) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    va_list args;
    va_start(args, message);
    std::cout << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") << "][INFO] ";
    vprintf(message, args);
    std::cout << "\n";
    va_end(args);
}

void Logging::LogWarning(const char* message, ...) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    va_list args;
    va_start(args, message);
    std::cout << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") << "][WARNING] ";
    vprintf(message, args);
    std::cout << "\n";
    va_end(args);
}

void Logging::LogError(const char* message, ...) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    va_list args;
    va_start(args, message);
    std::cout << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") << "][ERROR] ";
    vprintf(message, args);
    std::cout << "\n";
    va_end(args);
}