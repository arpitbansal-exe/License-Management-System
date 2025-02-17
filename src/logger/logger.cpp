// Logger.cpp
#include "Logger.h"
#include <iomanip>
#include <filesystem>
namespace LOGGING {

    // Configure Logger

    void Logger::configure(const std::string& logFilePath, bool consoleOutput) {
        std::lock_guard<std::mutex> lock(logMutex);
        logFile = logFilePath;
        logToConsole = consoleOutput;

        // Create directory if not exists
        std::filesystem::path logPath = std::filesystem::path(logFilePath).parent_path();
        if (!logPath.empty() && !std::filesystem::exists(logPath)) {
            std::filesystem::create_directories(logPath);
        }

        // Open log file
        fileStream.open(logFile, std::ios::app);
        if (!fileStream.is_open()) {
            std::cerr << RED << "Failed to open log file: " << logFile << RESET << "\n";
        }
        else {
            std::cout << GREEN << "Log file opened : " << logFile << RESET << "\n";
        }
    }

    // Log Message with Colored Console Output
    void Logger::log(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(logMutex);

        std::ostringstream logStream;
        logStream << "[" << getCurrentTime() << "] "
            << message << "\n";

        // Write to Console with Color
        if (logToConsole) {
            switch (level) {
            case LogLevel::INFO:
                std::cout << GREEN << logStream.str() << RESET;
                break;
            case LogLevel::WARNING:
                std::cout << YELLOW << logStream.str() << RESET;
                break;
            case LogLevel::LOG_ERROR:
                std::cerr << RED << logStream.str() << RESET;
                break;
            case LogLevel::DEBUG:
                std::cout << BLUE << logStream.str() << RESET;
                break;
            }
        }

        // Write to File
        if (fileStream.is_open()) {
            fileStream << logStream.str();
            fileStream.flush();
        }
    }

    // Convert LogLevel Enum to String
    std::string Logger::logLevelToString(LogLevel level) {
        switch (level) {
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::LOG_ERROR:   return "ERROR";
        case LogLevel::DEBUG:   return "DEBUG";
        default:                return "UNKNOWN";
        }
    }

    // Get Current Time
    std::string Logger::getCurrentTime() {
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    // Destructor to Close File Stream
    Logger::~Logger() {
        if (fileStream.is_open()) {
            fileStream.close();
        }
    }

} // namespace LOGGING
