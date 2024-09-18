#pragma once

#ifndef LOG_HPP
#define LOG_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h> 
#include <filesystem> 

void init_logging();

void log_error(const std::string& message);
void log_warning(const std::string& message);
void log_info(const std::string& message);


#endif // LOG_HPP
