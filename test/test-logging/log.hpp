#pragma once

#include <string>

// Define a macro to enable or disable logging
#define ENABLE_LOGGING 1  // Set to 1 to enable logging, 0 to disable logging

#if ENABLE_LOGGING
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <chrono>
#include <string_view>
#include <csignal>


void init_logging();
void log_info(const std::string& message);
void log_warning(const std::string& message);
void log_error(const std::string& message);
void cleanup_logging();

class Timer { // code by cherno, from: https://gist.github.com/TheCherno/b2c71c9291a4a1a29c889e76173c8d14 
public:
    Timer() { Reset(); }
    void Reset() { m_Start = std::chrono::high_resolution_clock::now(); }
    float Elapsed() const { return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f; }
    float ElapsedMillis() const { return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f; }
    void End(const std::string& message) {
        float elapsed = ElapsedMillis();
        log_info(message + ": " + std::to_string(elapsed) + "ms");
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};
inline Timer globalTimer;

class ScopedTimer {
public:
    ScopedTimer(std::string_view name) : m_Name(name) {}
    ~ScopedTimer() {
        float time = m_Timer.ElapsedMillis();
        log_info(std::string(m_Name) + " - " + std::to_string(time) + "ms");
    }

private:
    Timer m_Timer;
    std::string m_Name;
};

#else

inline void init_logging() {}
inline void log_info(const std::string& message) {}
inline void log_warning(const std::string& message) {}
inline void log_error(const std::string& message) {}
inline void cleanup_logging() {}

class Timer {
public:
    Timer() {}
    void Reset() {}
    float Elapsed() { return 0.0f; }
    float ElapsedMillis() { return 0.0f; }
    void End(const std::string& message) {}
};
inline Timer globalTimer;

class ScopedTimer {
public:
    ScopedTimer(std::string_view name) {}
};

#endif // ENABLE_LOGGING
