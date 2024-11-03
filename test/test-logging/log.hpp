#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <csignal>
#include <string>

// Define a macro to enable or disable logging
#define ENABLE_LOGGING 1  // Change to 1 to enable logging, 0 to disable logging

#if ENABLE_LOGGING
void init_logging();
void log_error(const std::string& message);
void log_warning(const std::string& message);
void log_info(const std::string& message);
void signal_handler(int signal);
void setup_signal_handlers();
#else 
inline void init_logging() {}
inline void log_error(const std::string& message) {}
inline void log_warning(const std::string& message) {}
inline void log_info(const std::string& message) {}
inline void signal_handler(int signal) {}
inline void setup_signal_handlers() {}
#endif // ENABLE_LOGGING
