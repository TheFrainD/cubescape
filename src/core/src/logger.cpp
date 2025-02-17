#include "core/logger.h"

#include <assert.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <vector>

#include "core/assert.h"

namespace core {
std::shared_ptr<spdlog::logger> Logger::logger_;
bool Logger::initialized_ = false;

void Logger::Init(const std::string &log_file) {
    ASSERT(initialized_);

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    sinks.push_back(
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file, true));

    logger_ =
        std::make_shared<spdlog::logger>("cubescape", begin(sinks), end(sinks));
    spdlog::register_logger(logger_);
    logger_->set_level(spdlog::level::trace);
    logger_->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
    initialized_ = true;
}

std::shared_ptr<spdlog::logger> &Logger::GetLogger() {
    ASSERT(initialized);
    return logger_;
}
}  // namespace core