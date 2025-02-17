#pragma once

#include <spdlog/spdlog.h>

#include <memory>
#include <string>

#define LOG_TRACE(...)    core::Logger::GetLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)    core::Logger::GetLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)     core::Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)     core::Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)    core::Logger::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) core::Logger::GetLogger()->critical(__VA_ARGS__)

namespace core {
class Logger {
public:
    static void Init(const std::string &log_file);
    static std::shared_ptr<spdlog::logger> &GetLogger();

private:
    Logger()                          = delete;
    ~Logger()                         = delete;
    Logger(const Logger &)            = delete;
    Logger &operator=(const Logger &) = delete;

    static bool initialized_;
    static std::shared_ptr<spdlog::logger> logger_;
};

}  // namespace core
