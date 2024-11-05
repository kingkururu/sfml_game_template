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


/*
code by cherno, from: https://gist.github.com/TheCherno/b2c71c9291a4a1a29c889e76173c8d14
*/

#include <chrono>
#include <iostream>
#include <string_view>

class Timer
{
public:
	Timer() { Reset(); }
	void Reset() { m_Start = std::chrono::high_resolution_clock::now(); }
	float Elapsed() const { return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f; }
	float ElapsedMillis() const { return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f; }
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_Start; 
};

class ScopedTimer
{
public:
    ScopedTimer(std::string_view name) : m_Name(name) {}
    ~ScopedTimer()
    {
        float time = m_Timer.ElapsedMillis();
        //std::cout << m_Name << " - " << time << "ms\n";
        log_info(std::string(m_Name) + " - " + std::to_string(time) + "ms"); // added this line to go into logging 
    }
private:
    Timer m_Timer;
    std::string m_Name;
};
