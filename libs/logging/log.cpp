#include "log.hpp"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

void init_logging() {
    // Define the log file paths
    std::string info_log_file = "libs/logging/info.txt";
    std::string error_log_file = "libs/logging/errors.txt";

    // Create sinks for info and error logs
    auto info_console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto error_console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    auto info_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(info_log_file, true);
    auto error_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(error_log_file, true);
    
    // Set color for console sinks
    info_console_sink->set_pattern("%^[%T] [info] %v%$");
    error_console_sink->set_pattern("%^[%T] [error] %v%$");
    
    // Set logging levels for sinks
    info_console_sink->set_level(spdlog::level::info);  // Info messages
    error_console_sink->set_level(spdlog::level::err);  // Error messages

    info_file_sink->set_level(spdlog::level::info);  // Info messages
    error_file_sink->set_level(spdlog::level::err);  // Error messages

    // Create loggers for different levels
    auto info_logger = std::make_shared<spdlog::logger>("info_logger", spdlog::sinks_init_list{info_console_sink, info_file_sink});
    auto error_logger = std::make_shared<spdlog::logger>("error_logger", spdlog::sinks_init_list{error_console_sink, error_file_sink});

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
