#include "log.hpp"
#include <spdlog/sinks/basic_file_sink.h>
#include <filesystem>

void init_logging() {
    // Define the log directory and file name
    std::string log_dir = "logging";  // Directory where logs will be stored
    std::string log_file = "logs/errors.txt";  // Log file path

    // Ensure the directory exists
    std::filesystem::create_directories(log_dir);

    // Create a file sink that writes to "logs/errors.txt"
    auto file_logger = spdlog::basic_logger_mt("file_logger", log_file);
    // Set the logging level to error
    file_logger->set_level(spdlog::level::err);
}

void log_error(const std::string& message) {
    auto logger = spdlog::get("file_logger");
    if (logger) {
        logger->error(message);
    }
}
