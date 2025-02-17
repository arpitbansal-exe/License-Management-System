// Logger.hpp
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <mutex>


#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

template<typename... Args>
std::string formatArgs(Args&&... args) {
    std::ostringstream oss;
    (oss << ... << std::forward<Args>(args));
    return oss.str();
}

// Use Fully Qualified Namespace in Macros
#define LOG_INFO(...) LOGGING::Logger::getInstance().log(LOGGING::LogLevel::INFO, formatArgs(__VA_ARGS__))
#define LOG_WARNING(...) LOGGING::Logger::getInstance().log(LOGGING::LogLevel::WARNING, formatArgs(__VA_ARGS__))
#define LOG_ERROR(...) LOGGING::Logger::getInstance().log(LOGGING::LogLevel::LOG_ERROR, formatArgs(__VA_ARGS__))
#define LOG_DEBUG(...) LOGGING::Logger::getInstance().log(LOGGING::LogLevel::DEBUG, formatArgs(__VA_ARGS__))


namespace LOGGING {
    // Log Levels
    enum class LogLevel { INFO, WARNING, LOG_ERROR, DEBUG };

    // Singleton Logger Class
    class Logger {
    public:
        // Get Singleton Instance
        static Logger& getInstance() {
            static Logger instance;
            return instance;
        }

        // Configure Logger (Optional: Call before logging)
        void configure(const std::string& logFilePath = "logs/application.log", bool consoleOutput = true);

        // Log Message
        void log(LogLevel level, const std::string& message);


// Destructor to Close File Stream
        ~Logger();

    private:
        std::string logFile;
        std::ofstream fileStream;
        bool logToConsole = true;
        std::mutex logMutex;

        // Private Constructor for Singleton Pattern
        Logger() = default;
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        // Helper to Get Current Time
        std::string getCurrentTime();
        // Convert LogLevel Enum to String
        std::string logLevelToString(LogLevel level);
    };
}