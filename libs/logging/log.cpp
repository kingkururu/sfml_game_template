#include "log.hpp"

void init_logging() {
    // Define the log file paths
    std::string info_log_file = "libs/logging/info.txt";
    std::string error_log_file = "libs/logging/errors.txt";

    // Create sinks for info and error logs
    auto info_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(info_log_file);
    auto error_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(error_log_file);

    // Set the logging levels for each sink
    info_sink->set_level(spdlog::level::info);  // Info messages go to info.txt
    error_sink->set_level(spdlog::level::err);  // Error messages go to errors.txt

    // Create loggers for different levels
    auto info_logger = std::make_shared<spdlog::logger>("info_logger", info_sink);
    auto error_logger = std::make_shared<spdlog::logger>("error_logger", error_sink);

    // Set the logging levels for the loggers
    info_logger->set_level(spdlog::level::info);
    error_logger->set_level(spdlog::level::err);

    // Register the loggers
    spdlog::register_logger(info_logger);
    spdlog::register_logger(error_logger);

    // Set the default logger
    spdlog::set_default_logger(info_logger);
}

void log_error(const std::string& message) {
    auto logger = spdlog::get("error_logger");
    if (logger) {
        logger->error(message);
    }
}

void log_warning(const std::string& message) {
    auto logger = spdlog::get("error_logger");
    if (logger) {
        logger->warn(message);
    }
}

void log_info(const std::string& message) {
    auto logger = spdlog::get("info_logger");
    if (logger) {
        logger->info(message);
    }
}
