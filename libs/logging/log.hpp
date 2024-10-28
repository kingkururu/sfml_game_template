#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h> 
#include <filesystem> 
#include <spdlog/sinks/stdout_color_sinks.h>

#include <csignal>

void init_logging();

void log_error(const std::string& message);
void log_warning(const std::string& message);
void log_info(const std::string& message);
void signal_handler(int signal);
void setup_signal_handlers();


