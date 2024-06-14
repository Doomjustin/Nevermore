#include "nevermore/Log.h"

#include <spdlog/spdlog.h>
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace sf {

class SpdLogger: public sf::Logger {
public:
    explicit SpdLogger(std::string_view name)
      : logger_{ spdlog::stdout_color_mt(std::string{ name }) }
    {
        logger_->set_pattern("[%H:%M:%S %z] [%n] [%^%l%$] [%t]: %v");
    }

    void error(std::string_view message) noexcept override
    {
        logger_->error(message);
    }

    void warning(std::string_view message) noexcept override
    {
        logger_->warn(message);
    }

    void debug(std::string_view message) noexcept override
    {
        logger_->debug(message);
    }

    void info(std::string_view message) noexcept override
    {
        logger_->info(message);
    }

    void trace(std::string_view message) noexcept override
    {
        logger_->trace(message);
    }

    void level(LogLevel level) noexcept override
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

    void pattern(const std::string& fmt) noexcept override
    {
        logger_->set_pattern(fmt);
    }
    
private:
    std::shared_ptr<spdlog::logger> logger_;
};


std::unique_ptr<Logger> make_logger(std::string_view name)
{
    return std::make_unique<SpdLogger>(name);
}

namespace log {

Logger& default_logger()
{
    static std::unique_ptr<Logger> default_logger_ = make_logger("Nevermore Logger");

    return *default_logger_;
}

void pattern(const std::string& fmt)
{
    default_logger().pattern(fmt);
}

void level(LogLevel level)
{
    default_logger().level(level);
}

} // namespace sf::log

} // namespace sf