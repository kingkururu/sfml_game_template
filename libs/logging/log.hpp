#ifndef LOG_HPP
#define LOG_HPP

#include <spdlog/spdlog.h>

void init_logging();
void log_error(const std::string& message);

#endif // LOG_HPP
