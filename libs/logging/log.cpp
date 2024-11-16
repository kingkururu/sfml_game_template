#include "log.hpp"

#if ENABLE_LOGGING

struct LogEntry {
    std::string message;
    spdlog::level::level_enum level;
};

class LogQueue {
public:
    void push(LogEntry entry) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(entry));
        condition_.notify_one();
    }

    bool pop(LogEntry& entry) {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this] { return !queue_.empty() || stop_; });
        if (stop_ && queue_.empty())
            return false;
        entry = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    void stop() {
        std::lock_guard<std::mutex> lock(mutex_);
        stop_ = true;
        condition_.notify_all();
    }

private:
    std::queue<LogEntry> queue_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool stop_ = false;
};

class AsyncLogger {
public:
    AsyncLogger() : logging_thread_(&AsyncLogger::processLogQueue, this), stop_thread_(false) {}

    ~AsyncLogger() {
        stop_thread_ = true;
        log_queue_.stop();
        if (logging_thread_.joinable()) {
            logging_thread_.join();
        }
    }

    void log(const std::string& message, spdlog::level::level_enum level) {
        log_queue_.push({message, level});
    }

private:
    void processLogQueue() {
        LogEntry entry;
        while (!stop_thread_ && log_queue_.pop(entry)) {
            auto logger = spdlog::get(entry.level == spdlog::level::err ? "error_logger" : "info_logger");
            if (logger) {
                if (entry.level == spdlog::level::info) {
                    logger->info(entry.message);
                } else if (entry.level == spdlog::level::warn) {
                    logger->warn(entry.message);
                } else if (entry.level == spdlog::level::err) {
                    logger->error(entry.message);
                }
                logger->flush();
            }
        }
    }

    LogQueue log_queue_;
    std::thread logging_thread_;
    std::atomic<bool> stop_thread_;
};

// Singleton instance for AsyncLogger
inline AsyncLogger asyncLogger;

// Logging helper functions
void log_info(const std::string& message) {
    asyncLogger.log(message, spdlog::level::info);
}

void log_warning(const std::string& message) {
    asyncLogger.log(message, spdlog::level::warn);
}

void log_error(const std::string& message) {
    asyncLogger.log(message, spdlog::level::err);
}

// Logging initialization and cleanup
void init_logging() {
    std::string info_log_file = "test/test-logging/loggingFiles/info.txt";
    std::string error_log_file = "test/test-logging/loggingFiles/errors.txt";

    auto info_console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto error_console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    auto info_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(info_log_file, true);
    auto error_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(error_log_file, true);

    info_console_sink->set_pattern("%^[%T] [info] %v%$");
    error_console_sink->set_pattern("%^[%T] [error] %v%$");

    info_console_sink->set_level(spdlog::level::info);
    error_console_sink->set_level(spdlog::level::err);

    info_file_sink->set_level(spdlog::level::info);
    error_file_sink->set_level(spdlog::level::err);

    auto info_logger = std::make_shared<spdlog::logger>("info_logger", spdlog::sinks_init_list{info_console_sink, info_file_sink});
    auto error_logger = std::make_shared<spdlog::logger>("error_logger", spdlog::sinks_init_list{error_console_sink, error_file_sink});

    info_logger->set_level(spdlog::level::info);
    error_logger->set_level(spdlog::level::err);

    spdlog::register_logger(info_logger);
    spdlog::register_logger(error_logger);
    spdlog::set_default_logger(info_logger);
}

void cleanup_logging() {
    spdlog::shutdown();
}

// RAII wrapper for managing logging lifecycle
class LoggerManager {
public:
    LoggerManager() { init_logging(); } // Initialize logging on creation
    ~LoggerManager() { cleanup_logging(); } // Cleanup logging on destruction
};

// Global instance to manage logging lifecycle
inline LoggerManager loggerManager;

#endif // ENABLE_LOGGING
