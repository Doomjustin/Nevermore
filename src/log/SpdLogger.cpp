#include "SpdLogger.h"

#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace sf::log {

SpdLogger::SpdLogger(std::string_view name)
  : logger_{ spdlog::stdout_color_mt(std::string{ name }) }
{
    logger_->set_pattern("[%H:%M:%S %z] [%n] [%^%l%$] [%t]: %v");
}

void SpdLogger::error(std::string_view message) noexcept
{
    logger_->error(message);
}

void SpdLogger::warning(std::string_view message) noexcept
{
    logger_->warn(message);
}

void SpdLogger::debug(std::string_view message) noexcept
{
    logger_->debug(message);
}

void SpdLogger::info(std::string_view message) noexcept
{
    logger_->info(message);
}

void SpdLogger::trace(std::string_view message) noexcept
{
    logger_->trace(message);
}

void SpdLogger::level(LogLevel level) noexcept
{
    switch (level) {
    case LogLevel::Off:
        logger_->set_level(spdlog::level::off);
        break;
    case LogLevel::Error:
        logger_->set_level(spdlog::level::err);
        break;
    case LogLevel::Warning:
        logger_->set_level(spdlog::level::warn);
        break;
    case LogLevel::Debug:
        logger_->set_level(spdlog::level::debug);
        break;
    case LogLevel::Info:
        logger_->set_level(spdlog::level::info);
        break;
    case LogLevel::Trace:
        logger_->set_level(spdlog::level::trace);
        break;
    }
}

void SpdLogger::pattern(const std::string& fmt) noexcept
{
    logger_->set_pattern(fmt);
}

} // namespace sf::log